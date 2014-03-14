/*
 * array.c
 *
 *  Created on: Feb 21, 2014
 *      Author: thar0l
 */

#include "array.h"

struct array
{
	int *items;
	int size;
};

int arr_sort(struct array *arr, int index);


struct array *arr_create(int size)
{
	struct array *temp = NULL;
    if (size <= 0)
    {
    	errno = EINVAL;
        return NULL;
    }
    temp = malloc (sizeof(struct array));
    if (temp == NULL)
    {
    	errno = ENOMEM;
        return NULL;
    }
    temp->size = size;
    temp->items = calloc(size, sizeof(int));
    if (temp->items == NULL)
    {
    	free (temp);
    	errno = ENOMEM;
        return NULL;
    }
	errno = 0;
    return temp;
}


void arr_delete(struct array *arr)
{
	if (arr == NULL)
	{
    	errno = EBADR;
        return;
	}
    free(arr->items);
    free(arr);
	errno = 0;
	return;
}


int arr_resize(struct array *arr, int size)
{
	if (size <= 0)
	{
		errno = EINVAL;
		return -1;
	}
	int *temp = calloc(size,sizeof(int));
	if (temp == NULL)
	{
		errno = ENOMEM;
		return -1;
	}
	int min = arr->size;
	if (size < min) min = size;
	int i;
	for (i = 0; i < min; i++) temp[i] = arr->items[i];
    free(arr->items);
    arr->size = size;
    arr->items = calloc(size, sizeof(int));
	if (arr->items == NULL)
	{
		return -1;
	}
	for (i = 0; i < min; i++) arr->items[i] = temp[i];
    free(temp);
    temp=NULL;
	arr_sort(arr,0);
	errno = 0;
	return size;
}

int arr_getitem(struct array *arr, int index, int *value)
{
	*value = 0;
    if (arr == NULL)
    {
    	errno = EBADR;
    	return -1;
    }
    if (index < 0)
    {
    	errno = EADDRNOTAVAIL;
        return -1;
    }
    if (index >= (arr->size))
    {
    	errno = EADDRNOTAVAIL;
        return -1;
    }
    *value = arr->items[index];
	errno = 0;
    return index;
}

int arr_setitem(struct array *arr, int index, int value)
{
    if (arr == NULL)
    {
    	errno = EBADR;
    	return -1;
    }
    if (index < 0)
    {
    	errno = EADDRNOTAVAIL;
        return -1;
    }
    if (index >= (arr->size))
    {
    	errno = EADDRNOTAVAIL;
        return -1;
    }
    arr->items[index] = value;
	errno = 0;
    return arr_sort(arr, index);
}

 int arr_sort(struct array *arr, int index)
{
	int newindex = index;
	int i,j;
	for (i = 0; i < arr->size; i++)
	for (j = i; j < arr->size; j++)
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

int arr_for_each (struct array *arr, int (*foo) (int item, void *data), void *data)
{
    if (arr == NULL)
    {
    	errno = EBADR;
    	return -1;
    }

    int i = 0;
    for (i = 0; i < arr->size; i++)
    	{
    		arr->items[i] = (*foo) (arr->items[i], data);
    	}
	errno = 0;
	return 0;
}
