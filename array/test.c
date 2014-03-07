/*
 * test.c
 *
 *  Created on: Feb 21, 2014
 *      Author: thar0l
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "array.h"

typedef struct array  *array;

int inc(int a, int index, int data)
{
	return a+data;
}

int print(int a, int index, int data)
{
	printf("%d \t : \t %d\n",index,data);
	return data;
}

int main()
{
	srand(time(NULL));
    array arr=NULL;
    int i;

    arr = arr_create(-2);
    arr_setitem(arr,0,0);
    i = arr_getitem(arr,0);
    arr = arr_for_each(arr,(*print),0);
    arr = arr_delete(arr);

    arr = arr_create(0);
    arr = arr_delete(arr);

    arr = arr_create(30);
    for (i = 0; i < 30; i++)
    {
    	int x = rand()%100;
    	arr_setitem(arr,0,x);
    }

    arr_setitem(arr,-1,0);
    arr_setitem(arr,31,0);

    i = arr_getitem(arr,0);
    i = arr_getitem(arr,-1);
    i = arr_getitem(arr,31);

    arr = arr_for_each(arr,(*print),0);
    arr = arr_for_each(arr,(*inc),2);
    printf("\n\n\n");
    arr = arr_for_each(arr,(*print),0);
    printf("\n\n\n");

    arr = arr_delete(arr);
    arr = arr_delete(arr);


    arr = arr_create(10000000);
    printf("err: %d\n",arr_error);
    arr = arr_delete(arr);

    arr = arr_create(5);
    arr = arr_for_each(arr,(*print),0);
    printf("\n\n\n");
    for (i = 0; i < 5; i++)
    {
    	int x = rand()%100;
    	arr_setitem(arr,0,x);
    }
    arr = arr_for_each(arr,(*print),0);
    printf("\n\n\n");
    arr = arr_resize(arr,10);
    arr = arr_for_each(arr,(*print),0);
    printf("\n\n\n");

    return 0;
}
