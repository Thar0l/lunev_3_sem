/*
 * array.c
 *
 *  Created on: Feb 21, 2014
 *      Author: thar0l
 */

#include "array.h"

array arr_create(int size)
{
	arr_error = ERR_NoError;
    array temp;

    if (size <= 0)
    {
        arr_error = ERR_InvalidSize;
        return NULL;
    }

    temp = malloc (sizeof(struct array));

    if (temp == NULL)
    {
        arr_error = ERR_NoMemory;
        return NULL;
    }

    temp->count = size;
    temp->items = calloc(size, sizeof(int));

    if (temp->items == NULL)
    {
    	free (temp);
        arr_error = ERR_NoMemory;
        return NULL;
    }

    return temp;
}

array arr_resize(array arr, int size)
{
	arr_error = ERR_NoError;

	array temp = arr_create(size);
	if (temp == NULL)
	{
			return NULL;
	}

	int count = arr->count;
	if (size < count) count = size;

	int i;
	for (i = 0; i < count; i++) temp->items[i] = arr->items[i];
	arr_sort(temp,0);
	arr = arr_delete(arr);
	return temp;
}

array arr_delete(array arr)
{
	arr_error = ERR_NoError;
    if (arr == NULL)
    {
        arr_error = ERR_ArrNotExist;
        return NULL;
    }

    free(arr->items);
    free(arr);
    return NULL;
}

int arr_getitem(array arr, int index)
{
	arr_error = ERR_NoError;
    if (arr == NULL)
    {
    	arr_error = ERR_ArrNotExist;
    	return -1;
    }
    if (index < 0)
    {
        arr_error = ERR_InvalidIndex;
        return -1;
    }
    if (index >= (arr->count))
    {
        arr_error = ERR_NoItem;
        return -1;
    }

    return arr->items[index];
}

int arr_setitem(array arr, int index, int value)
{
	arr_error = ERR_NoError;
    if (arr == NULL)
    {
    	arr_error = ERR_ArrNotExist;
    	return -1;
    }
    if (index < 0)
    {
        arr_error = ERR_InvalidIndex;
        return -1;
    }
    if (index >= (arr->count))
    {
        arr_error = ERR_NoItem;
        return -1;
    }

    arr->items[index] = value;
    return arr_sort(arr, index);
}

 int arr_sort(array arr, int index)
{
	int newindex = index;
	int i,j;
	for (i = 0; i < arr->count; i++)
	for (j = i; j < arr->count; j++)
		if (arr->items[j] < arr->items[i])
		{
			int temp = arr->items[i];
			arr->items[i] = arr->items[j];
			arr->items[j] = temp;
			if (i == newindex)
				newindex = j;
			if (j == newindex)
				newindex = i;
		}
	return newindex;
}

array arr_for_each (array arr, int (*foo) (int a, int index, int data), int a)
{
	arr_error = ERR_NoError;
    if (arr == NULL)
    {
    	arr_error = ERR_ArrNotExist;
    	return NULL;
    }

    int i;
    for (i=0; i < arr->count; i++)
    	{
    		arr->items[i] = (*foo) (a, i, arr->items[i]);
    	}
	return arr;
}
