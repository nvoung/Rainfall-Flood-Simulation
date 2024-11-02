#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "util.h"

#define IMAX_SIZE 10



void *allocateArray(int rows, int columns)
{
	double** array = (double**)calloc(rows, sizeof(double*));
	if (array == NULL)
	{
		return NULL; //allocation fails
	}
	for (int i = 0; i < rows; i++)
	{
		array[i] = (double*)calloc(columns, sizeof(double));
		if (array[i] == NULL)
		{
			for (int j = 0; j < i; j++)
			{
				free(array[j]);
				return NULL; //allocation fails
			}
		}
	}
	return array;
}

int listInit(List* l, int max_elmt_size)
{
	l->size = 0;
	l->max_size = IMAX_SIZE;
	l->max_element_size = max_elmt_size;
	l->data = (void *)malloc(l->max_size * l->max_element_size);
	if (l->data == NULL)
	{
		return -1; // allocation fails
	}
	return 0; //Data has been allocated
}

void listAddEnd(List* l, void* elmt) {
	if (l->size == l->max_size)
	{
		int newSize = l->max_size * 2; //Double the array size
		void* new_data = malloc(newSize * l->max_element_size); //allocate memory for new array
		if (new_data == NULL) 
		{
			return; //allocation fails
		}
		memcpy(new_data, l->data, l->size * l->max_element_size); //copy data from old to new
		free(l->data); // free old data
		l->max_size = newSize; //change the max size to the new max size
		l->data = new_data; //put new data array into old one
	}
	memcpy(l->data + l->size * l->max_element_size, elmt, l->max_element_size); //add the new element to end of the list
	l->size++; //increase the size of the list
}

void* listGet(List* l, int index)
{
	if (index < 0 || index >= l->size)
	{
		return NULL; //Index is out of bounds
	}
	return l->data + (index * l->max_element_size); //returns a pointer to the data from the list at the index
}
