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
#include <sys/time.h>
#include <sys/resource.h>

typedef struct array  *array;


int print(int item, int index, void *data)
{
	printf("%d \t : \t %d\n",index,item);
	return item;
}

int main(int argc, char *argv[])
{
	if ((argc > 1)&&(argv[1][0] == 'm'))
	{
	struct rlimit r;
	r.rlim_cur=10000000;
	r.rlim_max=10000001;
	setrlimit(RLIMIT_AS, &r);
	}
	srand(time(NULL));
    struct array *arr = NULL;
    int i;

    arr = arr_create(-2);
    arr_setitem(arr,0,0);
    arr_getitem(arr,0,&i);
    arr_for_each(arr,(*print),&i);
    arr_delete(arr);

    arr = arr_create(0);
    arr_delete(arr);

    arr = arr_create(30);
    for (i = 0; i < 30; i++)
    {
    	int x = rand()%100;
    	arr_setitem(arr,0,x);
    }

    arr_setitem(arr,-1,0);
    arr_setitem(arr,31,0);

    arr_getitem(arr,0,&i);
    arr_getitem(arr,-1,&i);
    arr_getitem(arr,31,&i);

    arr_for_each(arr,(*print),&i);
    printf("\n\n\n");

    arr_delete(arr);


    arr = arr_create(10000000);
    printf("err: %d\n",errno);
    arr_delete(arr);

    arr = arr_create(5);
    arr_for_each(arr,(*print),&i);
    printf("\n\n\n");
    for (i = 0; i < 5; i++)
    {
    	int x = rand()%100;
    	arr_setitem(arr,0,x);
    }
    arr_for_each(arr,(*print),&i);
    printf("\n\n\n");
    arr_resize(arr,10);
    arr_for_each(arr,(*print),&i);
    printf("\n\n\n");
    arr_resize(arr,-1);
    arr_resize(arr,6);
    arr_for_each(arr,(*print),&i);
    printf("\n\n\n");
	if ((argc > 1)&&(argv[1][0] == 'm'))
	{
    if (arr_resize(arr,10000000)<0) printf("No mem\n");
    if (arr_resize(arr,100000000)<0) printf("No mem\n");
	}
    arr_delete(arr);

	if ((argc > 1)&&(argv[1][0] == 'm'))
	{
	struct rlimit r;
	r.rlim_cur=22;
	r.rlim_max=2;
	setrlimit(RLIMIT_AS, &r);
	}

	struct array **arr1 = NULL;
    arr = arr_create(1);
    arr1 = malloc(10*sizeof(struct array *));
    for (i=0; i<10; i++) arr1[i]=arr_create(1);
    arr_delete(arr);
    arr_delete(arr1);
    return 0;
}
