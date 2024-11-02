
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "util.h"
#include "pointcloud.h"
#include "bmp.h"


int main(int argc, char* argv[])
{
    const char* input_filename = argv[1];

    if (argc != 2) 
    {
        return 1; //error if there are less is anything other than the two arguments ./display <input_file>
    }
  
    FILE* input_file = fopen(input_filename, "r");
    if (input_file == NULL) 
    {
        fprintf(stderr, "Error opening input file: %s", input_filename);
        return 1; //error if the file given is null
    }

   pointcloud_t *pc = readPointCloudData(input_file); //read the data from the input file
  
   imagePointCloud(pc, "out.gif"); //create the out.gif



    fclose(input_file);
    free(pc->points.data);
    free(pc);

    return 0;
}
