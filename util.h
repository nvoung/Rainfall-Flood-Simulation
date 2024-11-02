#ifndef UTIL_H
#define UTIL_H


#define ARRAYMACRO(array, row, column, columns) (array[(row * columns + column)]) //given row column and columns it finds the index of
																				  //the 1-d array that represents the element from the 2-d array
void* allocateArray(int rows, int columns);

typedef struct pcd
{
	double x; //x coord
	double y; //y cord
	double z; // height
	double wd;
	struct pcd* north; //struct pointer north
	struct pcd* east; //struct pointer east
	struct pcd* south; //struct pointer south
	struct pcd* west; //struct pointer west

} pcd_t;

//Given Structs 
typedef struct
{
	int max_size;
	int max_element_size;
	void* data;
	int size;
}List;

//Given Function prototypes
int listInit(List* l, int max_elmt_size);
void listAddEnd(List* l, void* elmt);
void* listGet(List* l, int index);


#endif
