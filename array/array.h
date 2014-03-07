/*
 * array.h
 *
 *  Created on: Feb 21, 2014
 *      Author: thar0l
 */

#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdlib.h>
#include <stdio.h>

//error codes
#define ERR_NoError 0
#define ERR_ArrNotExist -1
#define ERR_InvalidIndex -2
#define ERR_NoMemory -3
#define ERR_NoItem -4
#define ERR_ArrFull -5
#define ERR_InvalidSize -6

int arr_error;

typedef struct array *array;

struct array
{
    int *items;
    int count;
};

array arr_create(int size);
array arr_resize(array arr, int size);
array arr_delete(array arr);


int arr_getitem(array arr, int index);
int arr_setitem(array arr, int index, int value);
static int arr_sort(array arr, int index);

array arr_for_each (array arr, int (*foo) (int a, int index, int data), int a);

#endif /* ARRAY_H_ */
