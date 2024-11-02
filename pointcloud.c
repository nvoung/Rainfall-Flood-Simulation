#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "pointcloud.h"
#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "pointcloud.h"
#include "bmp.h"

void imagePointCloudWater(pointcloud_t* pc, double maxwd, char* filename) {
	if (pc == NULL ) 
	{
		return;
	}

	pcd_t* points = (pcd_t*)pc->points.data;
	int point_count = pc->points.size;
	int width = pc->cols;
	int height = pc->rows;
	double minHeight = DBL_MAX;
	double maxHeight = DBL_MIN;
	double minWd = DBL_MAX;

	for (int i = 0; i < pc->points.size; i++)
	{
		pcd_t* point = (pcd_t*)listGet(&pc->points, i);
		if (point == NULL) 
		{
			return; //List get failed to get a point
		}

		if (point->z < minHeight) minHeight = point->z;
		if (point->z > maxHeight) maxHeight = point->z;
		if (point->wd < minWd) minWd = point->wd;
	}

	Bitmap* b = bm_create(width, height);

	for (int i = 0; i < point_count; i++)
	{
		pcd_t* point = (pcd_t*)listGet(&pc->points, i);
		if (point == NULL)
		{
			return; //List get failed to get a point
		}

		double shade = (point->z - minHeight) / (maxHeight - minHeight);
		unsigned char grayVal = (unsigned char)(shade * 255);

		/*calculates the shade of blue given the points water, if it is greater or equal to maxwd it gets 1 which makes it 
		pure blue otherwise it divides the water against maxwd to get the percentage to shade the point blue*/
		double wdPercent = (point->wd >= maxwd) ? 1.0 : point->wd / maxwd;
		unsigned char blueVal = (unsigned char)(wdPercent * 255);

		/*mixes the colors together in order to keep the depth of the map showing while also giving blue tints where needed
		adjusts grey value to blue value according on the wdPercent we calculated*/
		unsigned char mixedVal = (unsigned char)((grayVal * (1.0 - wdPercent)) + (blueVal * wdPercent));
		int row = i / width;
		int col = i % width;

		if (point->wd == 0) //if no water in point set color like before
		{
			bm_set_color(b, bm_rgb(grayVal, grayVal, grayVal));
		}
		else // sets color with mixedVal to give blue tint
		{
			bm_set_color(b, bm_rgb(grayVal, grayVal, mixedVal));
		}
		bm_putpixel(b, col, row);

	}
	bm_save(b, filename);

	bm_free(b);
}


void watershedStep(pointcloud_t* pc)
{
	if (pc == NULL)
	{
		return; //input is Null or not valid for pc
	}
	pcd_t* points = (pcd_t*)pc->points.data;
	double* newWd = (double*)malloc(pc->points.size * sizeof(double)); //allocate memory for an array of doubles for water amount
	if (newWd == NULL)
	{
		return; //allocation failed
	}

	/*finds the amount of water around the neighboring points
	and uses the formula given 𝑓(𝑡1, 𝑤1, 𝑡2, 𝑤2) = (𝑡2 + 𝑤2) − (𝑡1 + 𝑤1) ∗ 𝑤𝑐𝑜𝑒𝑓 and 
	𝑓(𝐶𝑒, 𝐶𝑤, 𝑊𝑒 , 𝑊𝑤) + 𝑓(𝐶𝑒, 𝐶𝑤, 𝐸𝑒 , 𝐸𝑤 ) +𝑓(𝐶𝑒, 𝐶𝑤, 𝑁𝑒, 𝑁𝑤) + 𝑓(𝐶𝑒, 𝐶𝑤, 𝑆𝑒 , 𝑆𝑤)– 𝐶𝑤 ∗ 𝑒𝑐𝑜𝑒𝑓 
	to calculate the new water amount at that point. Then repeats until it goes through all points  */
	for (int i = 0; i < pc->points.size; i++)
	{
		pcd_t* P = &points[i];
		double Nwd = (P->north != NULL) ? P->north->wd : 0; //finds the north neighbors water amount if null then sets water amount to 0
		double Swd = (P->south != NULL) ? P->south->wd : 0;
		double Ewd = (P->east != NULL) ? P->east->wd : 0;
		double Wwd = (P->west != NULL) ? P->west->wd : 0;
		double Nz = (P->north != NULL) ? P->north->z : P->z;// finds height of north neighboors and if null sets height to points height to ensure no water goes or comes from it
		double Sz = (P->north != NULL) ? P->north->z : P->z;
		double Ez = (P->north != NULL) ? P->north->z : P->z;
		double Wz = (P->north != NULL) ? P->north->z : P->z;

		double Nf = ((Nz + Nwd) - (P->z + P->wd)) * pc->wcoef; // 𝑓(𝑡1, 𝑤1, 𝑡2, 𝑤2) = (𝑡2 + 𝑤2) − (𝑡1 + 𝑤1) ∗ 𝑤𝑐𝑜𝑒𝑓
		double Sf = ((Sz + Swd) - (P->z + P->wd)) * pc->wcoef;
		double Ef = ((Ez + Ewd) - (P->z + P->wd)) * pc->wcoef;
		double Wf = ((Wz + Wwd) - (P->z + P->wd)) * pc->wcoef;

		newWd[i] = P->wd + Nf + Sf + Ef + Wf - (P->wd * pc->ecoef); //Changed the formula 𝑓(𝐶𝑒, 𝐶𝑤, 𝑊𝑒 , 𝑊𝑤) + 𝑓(𝐶𝑒, 𝐶𝑤, 𝐸𝑒 , 𝐸𝑤 ) +𝑓(𝐶𝑒, 𝐶𝑤, 𝑁𝑒, 𝑁𝑤) + 𝑓(𝐶𝑒, 𝐶𝑤, 𝑆𝑒 , 𝑆𝑤)– 𝐶𝑤 ∗ 𝑒𝑐𝑜𝑒𝑓 a bit to make it work for my code
		if (newWd[i] < 0) //makes sure we cant have negative water in a point
		{
			newWd[i] = 0;
		}
	}
	
	for (int i = 0; i < pc->points.size; i++) //Assigns the new water amount to each point after all cells have been updated
	{
		points[i].wd = newWd[i];
	}

	free(newWd);
}

void watershedAddUniformWater(pointcloud_t* pc, double amount)
{
	if (pc == NULL || pc->points.data == NULL)
	{
		return; // input is NULL or not valid for pc
	}

	pcd_t* points = (pcd_t*)pc->points.data;

	for (int i = 0; i < pc->points.size; i++)
	{
		points[i].wd += amount; //adds the given amount of water to each point
	}

}

int initializeWatershed(pointcloud_t* pc) {
	if (pc == NULL)
	{
		return -1; // input is NULL or not valid for pc
	}

	pcd_t* points = (pcd_t*)pc->points.data;
	int point_count = pc->points.size;
	int cols = pc->cols;

	double min_x = DBL_MAX;
	double min_y = DBL_MAX;
	double max_x = DBL_MIN;
	double max_y = DBL_MIN;

	/* Calculate min and max x and y*/
	for (int i = 0; i < point_count; i++) {
		if (points[i].x < min_x) min_x = points[i].x;
		if (points[i].x > max_x) max_x = points[i].x;
		if (points[i].y < min_y) min_y = points[i].y;
		if (points[i].y > max_y) max_y = points[i].y;
	}

	
	double spacing_x = (max_x - min_x) / (cols - 1); // Calculate the spacing between each cell
	double spacing_y = (max_y - min_y) / (cols - 1);

	
	pcd_t** grid = (pcd_t**)malloc(point_count * sizeof(pcd_t*)); // Create an array of pointers to pcd_t
	if (grid == NULL)
	{
		return -1;
	}
	for (int i = 0; i < point_count; i++) 
	{
		grid[i] = NULL;
	}

	/* Populate the grid with pointers to points. The point with the minimum x
coordinate and the minimum y coordinate is the very most southwest point. The
point with the maximum x coordinate and maximum y coordinate is the very most
northeast point. */
	for (int i = 0; i < point_count; i++) {
		int Ix = (int)((points[i].x - min_x) / spacing_x);
		int Iy = (int)((points[i].y - min_y) / spacing_y);
		grid[Iy * cols + Ix] = &points[i];
	}

	/* Initialize all points with no water and make all the neighbors NULL*/
	for (int i = 0; i < point_count; i++) {
		points[i].wd = 0.0;
		points[i].north = NULL;
		points[i].east = NULL;
		points[i].south = NULL;
		points[i].west = NULL;
	}

	
	for (int i = 0; i < point_count; i++) {
		int Ix = (int)((points[i].x - min_x) / spacing_x);
		int Iy = (int)((points[i].y - min_y) / spacing_y);

		if (Iy > 0)
		{
			points[i].north = grid[(Iy - 1) * cols + Ix]; //sets north neighboor
		}
		if (Iy < cols - 1)
		{
			points[i].south = grid[(Iy + 1) * cols + Ix]; //sets south neighboor
		}
		if (Ix > 0)
		{
			points[i].west = grid[Iy * cols + (Ix - 1)]; //sets west neighboor
		}
		if (Ix < cols - 1)
		{
			points[i].east = grid[Iy * cols + (Ix + 1)]; //sets east neighboor
		}
	}

	free(grid);

	return 0;
}


//Worked for my smaller files but took forever to initialize a large file like testb.xyz so 
//I made the method above based off some tips from the professor about using a grid
//int initializeWatershed(pointcloud_t* pc)
//{
//	if (pc == NULL || pc->points.data == NULL)
//	{
//		fprintf(stderr, "Error: pc or pc->points.data is NULL\n");
//		return -1; // input is NULL or not valid for pc
//	}
//
//	pcd_t* points = (pcd_t*)pc->points.data;
//	if (points == NULL)
//	{
//		return -1; // points data is NULL
//	}
//
//	/*initialize all points in list with no water and make all the Neighbors Null
//	to make it easier to compare points and assign neighbors in the next loop*/
//	for (int i = 0; i < pc->points.size; i++)
//	{
//		points[i].wd = 0.0;
//		points[i].north = NULL;
//		points[i].east = NULL;
//		points[i].south = NULL;
//		points[i].west = NULL;
//	}
//
//	for (int i = 0; i < pc->points.size; i++)
//	{
//		pcd_t* a = &points[i]; //creates a pointer to the first point
//		/*As the loop below itterates it will assign b a new pcd_t type pointer until we go
//		through all the points in the List. Each time b will get compared to the pointer a and
//		if the point is 1 away then it assigns it as a neighbor based on which direction it is 1
//		away from. NOTE: This could be a problem if the data points are not 1 away in values
//		for the x and y coordinates*/
//		for (int j = 0; j < pc->points.size; j++)
//		{
//			pcd_t* b = &points[j];
//			if (a != b)
//			{
//				if (b->x == a->x && b->y == ((a->y) + 1)) //North Neighbor
//				{
//					a->north = b;
//				}
//				if (b->x == a->x && b->y == ((a->y) - 1)) //South Neighbor
//				{
//					a->south = b;
//				}
//				if (b->x == ((a->x) + 1) && b->y == a->y) //East Neighbor
//				{
//					a->east = b;
//				}
//				if (b->x == ((a->x) - 1) && b->y == a->y) //West Neighbor
//				{
//					a->west = b;
//				}
//			}
//		}
//	}
//
//	return 0;
//}

pointcloud_t *readPointCloudData(FILE* stream)
{
	int rasterWidth;
	pointcloud_t* pc = (pointcloud_t*)malloc(sizeof(pointcloud_t));
	if (pc == NULL)
	{
		return NULL;
	}

	if (fscanf(stream, "%d", &rasterWidth) != 1)  
	{
		return NULL; //fails if no integer to read at the start
	}

	if (listInit(&pc->points, sizeof(pcd_t)) != 0) //initialize list
	{
		return NULL; //initalizing failed 
	}

	pcd_t point;
	while (fscanf(stream, "%lf %lf %lf", &point.x, &point.y, &point.z) == 3) //read points and store them in the list
	{
		listAddEnd(&pc->points, &point);
	}

	pc->rows = (pc->points.size + rasterWidth - 1) / rasterWidth;
	pc->cols = rasterWidth;
	return pc;
}


void imagePointCloud(pointcloud_t *pc, char* filename)
{
	if (pc == NULL || pc->points.data == NULL) {
		return; // input is NULL or not valid
	}

	pcd_t* points = (pcd_t*)pc->points.data;
	int width = pc->cols;
	int height = pc->rows;
	double minHeight = DBL_MAX;
	double maxHeight = DBL_MIN;
	for (int i = 0; i < pc->points.size; i++)
	{
		pcd_t* point = (pcd_t*)listGet(&pc->points, i); // get the point
		if (point == NULL) {
			return;
		}

		if (point->z < minHeight) minHeight = point->z;
		if (point->z > maxHeight) maxHeight = point->z;
	}


	Bitmap* b = bm_create(width, height); //Create the bmp Image
	if (b == NULL) 
	{
		fprintf(stderr, "Error: failed to create bitmap\n");
		return;
	}
	/*Determine the minimum and maximum z (height) over all points in the point-cloud data.
	Divide the range of heights into 256 equal lengths.
	Assign heights in the lowest (smallest) height-range to black (0, 0, 0) pixel value.
	Assign heights in the highest (largest) height-range to white (255, 255, 255) pixel value.
	Heights in other ranges are distributed proportionally. For example, a height in the
	middle between the highest and lowest would have a pixel value of (128, 128, 128). Note
	that this is not a hard computation and is a very simple expression. Don’t over think it*/

	for (int i = 0; i < pc->points.size; i++)
	{
		pcd_t* point = (pcd_t*)listGet(&pc->points, i);

		/*To get the Pixels RGB value we first take the maxHeight - the minHeight this gives us
		our range. From here we can take each point and subtract it by the minHeight so it fits in the
		range. Next we divide that value by the range to get a percentage relative to the max and min
		height. From here I was able to multiply that percentage by 255 to get the grayscale RGB value.*/
		double shade = (point->z - minHeight) / (maxHeight - minHeight);
		unsigned char pixelVal = (unsigned char)(shade * 255);

		int row = i / width; //finds the row we are in
		int col = i % width; //finds the column we are in
		bm_set_color(b, bm_rgb(pixelVal, pixelVal, pixelVal)); //set the color of pixel
		bm_putpixel(b, col, row); //place pixel
	}

	bm_save(b, filename);
	bm_free(b);
}

void stat1()
{
	Point point; // initialize point for scanning
	double minHeight = DBL_MAX; //initializes min height to a the highest possible number
	double maxHeight = -DBL_MAX;//intializes max height to lowest possible number.
	double combinedHeight = 0;
	int count = 0;
	Point minPoint; //holds lowest point data
	Point maxPoint; //holds highest point data
	double avgHeight;

	while (scanf("%lf %lf %lf", &(point.x), &(point.y), &(point.z)) == 3)
	{
		// printf("Read values: %lf %lf %lf\n", point.x, point.y, point.z); // Debug used to see if data was being
		// read from the file correctly.

		if (point.z < minHeight)
		{
			minHeight = point.z;
			minPoint = point;
		}
		if (point.z > maxHeight)
		{
			maxHeight = point.z;
			maxPoint = point;
		}
		combinedHeight += point.z;
		count++;
	}
	//printf("Made it out of Loop"); Used for debug
	//printf("%d",count); // Used fro debug

	if (count > 0) //Statement to print results if the loop has read information from at least one point
	{
		avgHeight = combinedHeight / count;
		printf("Min Height: %lf at (%lf, %lf)\n", minPoint.z, minPoint.x, minPoint.y);
		printf("Max Height: %lf at (%lf, %lf)\n", maxPoint.z, maxPoint.x, maxPoint.y);
		printf("Average Height: %lf \n", avgHeight);
	}
}


