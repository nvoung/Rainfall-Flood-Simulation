#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "pointcloud.h"
#include "bmp.h"

//Helper method that helped me find the amount of water at any given point and could be adjusted to show more or less information
//based on what I needed while debugging, to add more data just add another parameter for the fprintf statement.
void writePointsToFile(pointcloud_t* pc, const char* filename) 
{
    if (pc == NULL ) {
        return;
    }

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return;
    }

    pcd_t* points = (pcd_t*)pc->points.data;
    for (int i = 0; i < pc->points.size; i++) {
        fprintf(file, "Point[%d]: x = %f, y = %f, z = %f, wd = %f\n", i, points[i].x, points[i].y, points[i].z, points[i].wd); //add extra parameters here if needed for debugging
    }

    fclose(file);
}

int main(int argc, char* argv[])
{
    if (argc < 8 || argc > 9)
    {
        fprintf(stderr, "Error not enough, or to many arguments given");
        return 1;
    }


    const char* input_filename = argv[1];
    int iter;
    float iwater;
    float wcoef;
    float ecoef;
    char* ofilebase = argv[6];
    int seq;
    double maxwd;

    //assign variables with arguments given on command line
    sscanf(argv[2], "%d", &iter);
    sscanf(argv[3], "%f", &iwater);
    sscanf(argv[4], "%f", &wcoef);
    sscanf(argv[5], "%f", &ecoef);
    sscanf(argv[7], "%lf", &maxwd);
    sscanf(argv[8], "%d", &seq);

    FILE* input_file = fopen(input_filename, "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "Error opening input file: %s", input_filename);
        return 1; //error if the file given is null
    }

    pointcloud_t* pc = readPointCloudData(input_file);
    //store ecoef and wcoef into pc in order to use them for watershed step.
    pc->ecoef = ecoef;
    pc->wcoef = wcoef;

    initializeWatershed(pc);

    watershedAddUniformWater(pc, iwater);

    for (int i = 0; i < iter; i++)
    {
        
        if (seq > 0 && (i % seq == 0 ))
        {
            watershedStep(pc);
            char filename[50];
            sprintf(filename, "%s%d.gif", ofilebase, i);
            imagePointCloudWater(pc, maxwd, filename);
        }

    }

    // Used to debug writePointsToFile(pc, "tester.txt");

    if (seq <= 0 || iter % seq != 0) {
        char filename[50];
        sprintf(filename, "%s.gif", ofilebase);
        imagePointCloudWater(pc, maxwd, filename);
    }

    free(pc->points.data);
    free(pc);

    return 0;

}