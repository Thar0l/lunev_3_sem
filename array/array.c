/*
 * array.c
 *
 *  Created on: Feb 21, 2014
 *      Author: thar0l
 */

#include "array.h"

struct array
{
	unsigned int *items;
	unsigned int size;
};



struct array *arr_create(unsigned int size)
{
	struct array *temp = NULL;
    if (size == 0)
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
    temp->items = calloc(size, sizeof(unsigned int));
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


int arr_resize(struct array *arr, unsigned int size)
{
	if (size == 0)
	{
		errno = EINVAL;
		return -1;
	}
	int *temp = calloc(size,sizeof(unsigned int));
	if (temp == NULL)
	{
		errno = ENOMEM;
		return -1;
	}
	unsigned int min = arr->size;
	if (size < min)
		min = size;
	unsigned int i;
	for (i = 0; i < min; i++)
		temp[i] = arr->items[arr->size-1-i];
    free(arr->items);
    arr->size = size;
    arr->items = calloc(size, sizeof(unsigned int));
	if (arr->items == NULL)
	{
		errno = ENOMEM;
		return -1;
	}
	for (i = 0; i < min; i++)
		arr_setitem(arr,0,temp[i]);

    free(temp);
    temp = NULL;
	errno = 0;
	return size;
}

int arr_getitem(struct array *arr, unsigned int index, unsigned int *value)
{
	*value = 0;
    if (arr == NULL)
    {
    	errno = EBADR;
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

int arr_setitem(struct array *arr, unsigned int index, unsigned int value)
{
    if (arr == NULL)
    {
    	errno = EBADR;
    	return -1;
    }
    if (index >= (arr->size))
    {
    	errno = EADDRNOTAVAIL;
        return -1;
    }

    if (value == arr->items[index])
    {
    	errno = 0;
        return index;
    }

    if (value < arr->items[index])
    {
    	unsigned int i = index;
    	unsigned int newindex = 0;
    	while (i >= 0)
    	{
    		int found = 0;
    		if (i != 0)
    		{
    			if ((arr->items[i] >= value) && (value >= arr->items [i-1])){
    				found = 1; }
    		} else
    		{
    			if (arr->items[i] >= value)
    				found = 1;
    		}
    		if (found)
    		{
    			newindex = i;
    			unsigned int j;
    				for (j = index-1; j > i; j--)
    					arr->items[j+1] = arr->items[j];
    			break;
    		}
    		i--;
    	}
    	arr->items[newindex] = value;
    }

    if (value > arr->items[index])
    {
    	unsigned int i = index;
    	unsigned int newindex = arr->size - 1;
    	while (i < arr->size)
    	{
    		int found = 0;
    		if (i != arr->size-1)
    		{
    			if ((arr->items[i+1] > value) && (value >= arr->items [i]))
    				found = 1;
    		} else
    		{
    			if (value >= arr->items [i])
    				found = 1;
    		}
    		if (found)
    		{
    			newindex = i;
    			unsigned int j;
    				for (j = index + 1; j <= i; j++)
    					arr->items[j-1] = arr->items[j];
    			break;
    		}
    		i++;
    	}
    	arr->items[newindex] = value;
    }


	errno = 0;
    return 0;
}


int arr_for_each (struct array *arr, unsigned int (*foo) (unsigned int item, void *data), void *data)
{
    if (arr == NULL)
    {
    	errno = EBADR;
    	return -1;
    }

    unsigned int i = 0;
    for (i = 0; i < arr->size; i++)
    	{
    		arr->items[i] = (*foo) (arr->items[i], data);
    	}
	errno = 0;
	return 0;
}
