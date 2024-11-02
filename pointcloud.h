#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include "util.h" //using in the pointcloud.c messed up my makefile so moved into the header

typedef struct
{
	double x;
	double y;
	double z;
} Point; // Holds the three data points from test.xyz

typedef struct pointcloud
{
	List points;
	int rows;
	int cols;
	double wcoef;
	double ecoef;
}pointcloud_t;

void stat1();

void imagePointCloudWater(pointcloud_t* pc, double maxwd, char* filename);
pointcloud_t* readPointCloudData(FILE* stream);
void imagePointCloud(pointcloud_t *pc, char* filename);
int initializeWatershed(pointcloud_t*); 
void watershedAddUniformWater(pointcloud_t* pc, double amount);
void watershedStep(pointcloud_t* pc);

#endif