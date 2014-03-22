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



unsigned int print(unsigned int item, void *data)
{
	printf("\t %d\n",item);
	return item;
}

int main(int argc, char *argv[])
{

	if ((argc > 1)&&(argv[1][0] == 'm'))
	{
	struct rlimit r;
	r.rlim_cur=1000000000;
	r.rlim_max=5000000000;
	setrlimit(RLIMIT_AS, &r);
	}

	srand(time(NULL));
    struct array *arr = NULL;
    unsigned int i;





    arr = arr_create(-2);
    arr_setitem(arr,0,0);
    arr_getitem(arr,0,&i);
    arr_for_each(arr,(*print), NULL);
    arr_delete(arr);

    arr = arr_create(0);
    arr_delete(arr);

    arr = arr_create(30);
    for (i = 0; i < 30; i++)
    {
    	unsigned int x = rand()%1000;
    	arr_setitem(arr,15,x);
    }

    arr_setitem(arr,-1,0);
    arr_setitem(arr,31,0);

    arr_getitem(arr,0,&i);
    arr_getitem(arr,-1,&i);
    arr_getitem(arr,31,&i);

    arr_for_each(arr,(*print), NULL);
    printf("\n\n\n");

    arr_delete(arr);

	if ((argc > 1)&&(argv[1][0] == 'm'))
	{
    arr = arr_create(1000000000);
    printf("err: %d\n",errno);
    arr_delete(arr);
	}
    arr = arr_create(5);
    arr_for_each(arr,(*print), NULL);
    printf("\n\n\n");
    for (i = 0; i < 6; i++)
    {
    	int x = rand()%1000;
    	arr_setitem(arr,2,x);
    }
    arr_for_each(arr,(*print), NULL);
    printf("\n\n\n");
    arr_resize(arr,12);
    arr_for_each(arr,(*print), NULL);
    printf("\n\n\n");
    arr_resize(arr,3);
    arr_for_each(arr,(*print), NULL);
    printf("\n\n\n");
//    arr_resize(arr,-1);
    arr_resize(arr,6);
    arr_for_each(arr,(*print), NULL);
    printf("\n\n\n");
	if ((argc > 1)&&(argv[1][0] == 'm'))
	{
    if (arr_resize(arr,1000000000)<0) printf("No memory\n");
    if (arr_resize(arr,1000000000)<0) printf("No memory\n");
	}
    arr_delete(arr);

	if ((argc > 1)&&(argv[1][0] == 'm'))
	{
	struct rlimit r;
	r.rlim_cur=25;
	r.rlim_max=30;
	setrlimit(RLIMIT_AS, &r);
	}
	if ((argc > 1)&&(argv[1][0] == 'm'))
	{
	struct array **arr1 = NULL;
    arr = arr_create(1);
    arr1 = malloc(100*sizeof(struct array *));
    if (arr1 != NULL)
    {
    for (i=0; i<100; i++) arr1[i]=arr_create(100000000);
    for (i=0; i<100; i++) arr_delete(arr1[i]);
    free(arr1);
    }
    arr_delete(arr);
	}
    return 0;
}
