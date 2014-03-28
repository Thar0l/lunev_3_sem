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

const unsigned int SIZE1=12;
const unsigned int SIZE3=10;
const unsigned int SIZE41=10;
const unsigned int SIZE42=20;
const unsigned int SIZE43=5;
const unsigned int SIZE5=1000000000;

const unsigned int MAXELEM=1000;

unsigned int print(unsigned int item, void *data)
{
	printf("\t %d\n",item);
	return 0;
}

unsigned int sqr(unsigned int item, void *data)
{
	item = item*item;
	return 0;
}

int main(int argc, char *argv[])
{
	struct array *arr = NULL;
	unsigned int i = 0;
	unsigned int x = 0;
	unsigned int errors = 0;
	srand(time(NULL));
	printf("*********************************************\n");
	printf(" TEST 1\n");
	printf(" Normal array work. Size = %d\n",SIZE1);
	printf("*********************************************\n");
	errors = 0;
	printf("\tCreating ...\n");
	arr = arr_create(SIZE1);
	if (arr == NULL) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tSetting %d elements ...\n",SIZE1+4);
    for (i = 0; i < SIZE1; i++)
    {
    	x = rand()%MAXELEM;
    	if (arr_setitem(arr, 0, x) < 0) errors++;
    }
    if (arr_setitem(arr, SIZE1-1, 0) < 0) errors++;
    if (arr_setitem(arr, 1, 1) < 0) errors++;
    if (arr_setitem(arr, 1, 1) < 0) errors++;
    if (arr_setitem(arr, 1, MAXELEM+1) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tGetting %d elements ...\n",SIZE1);
    for (i = 0; i < SIZE1; i++)
    {
    	if (arr_getitem(arr,i,&x) < 0) errors++;
    }
	printf("\tDone with %d errors.\n\n",errors);
	errors=0;
	printf("\tIterator ...\n");
	if (arr_for_each(arr,(*sqr), NULL) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	arr_delete(arr);


	printf("*********************************************\n");
	printf(" TEST 2\n");
	printf(" Zero array size. Working with empty array. \n");
	printf("*********************************************\n");
	errors = 0;
	printf("\tCreating ...\n");
	arr = arr_create(0);
	if (arr == NULL) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tSetting 1 element ...\n");
	if (arr_setitem(arr,0,x) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tGetting 1 element ...\n");
	if (arr_getitem(arr,0,&x) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors=0;
	printf("\tIterator ...\n");
	if (arr_for_each(arr,(*sqr), NULL) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	arr_delete(arr);


	printf("*********************************************\n");
	printf(" TEST 3\n");
	printf(" Invalid indexes. Size = %d\n",SIZE3);
	printf("*********************************************\n");
	errors = 0;
	printf("\tCreating ...\n");
	arr = arr_create(SIZE3);
	if (arr == NULL) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tSetting 3 elements ...\n");
	if (arr_setitem(arr,-1,x) < 0) errors++;
	if (arr_setitem(arr,SIZE3,x) < 0) errors++;
	if (arr_setitem(arr,SIZE3+1,x) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tGetting 3 elements ...\n");
	if (arr_getitem(arr,-1,&x) < 0) errors++;
	if (arr_getitem(arr,SIZE3,&x) < 0) errors++;
	if (arr_getitem(arr,SIZE3+1,&x) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	arr_delete(arr);


	printf("*********************************************\n");
	printf(" TEST 4\n");
	printf(" Normal resizing. Size1 = %d, Size2 = %d, Size3 = %d\n", SIZE41, SIZE42, SIZE43);
	printf("*********************************************\n");
	errors = 0;
	printf("\tCreating ...\n");
	arr = arr_create(SIZE41);
	if (arr == NULL) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tSetting %d elements ...\n",SIZE41);
    for (i = 0; i < SIZE41; i++)
    {
    	x = rand()%MAXELEM;
    	if (arr_setitem(arr, 0, x) < 0) errors++;
    }
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tPrinting Array ...\n");
	if (arr_for_each(arr,(*print), NULL) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tResizing Array from %d to %d ...\n", SIZE41, SIZE42);
	if (arr_resize(arr, SIZE42) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tPrinting Array ...\n");
	if (arr_for_each(arr,(*print), NULL) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tResizing Array from %d to %d ...\n", SIZE42, SIZE43);
	if (arr_resize(arr, SIZE43) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tPrinting Array ...\n");
	if (arr_for_each(arr,(*print), NULL) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	arr_delete(arr);


	printf("*********************************************\n");
	printf(" TEST 5\n");
	printf(" Memory limit. \n");
	printf("*********************************************\n");
	struct rlimit r;
	r.rlim_cur=SIZE5;
	r.rlim_max=2*SIZE5;
	setrlimit(RLIMIT_AS, &r);
	errors = 0;
	printf("\tCreating with size = %d ...\n",SIZE5*5);
	arr = arr_create(SIZE5*5);
	if (arr == NULL) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	arr_delete(arr);
	printf("\tCreating with size = %d ...\n",SIZE1);
	arr = arr_create(SIZE1);
	if (arr == NULL) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tSetting %d elements ...\n",SIZE1);
	for (i = 0; i < SIZE1; i++)
	{
	   	x = rand()%MAXELEM;
	   	if (arr_setitem(arr, 0, x) < 0) errors++;
	}
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tPrinting Array ...\n");
	if (arr_for_each(arr,(*print), NULL) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tResizing Array from %d to %d ...\n", SIZE1, SIZE5*5);
	if (arr_resize(arr, SIZE5*5) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tResizing Array from %d to %d ...\n", SIZE1, SIZE5/2);
	if (arr_resize(arr, SIZE5) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tPrinting Array ...\n");
	if (arr_for_each(arr,(*print), NULL) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	arr_delete(arr);


	printf("*********************************************\n");
	printf(" TEST 6\n");
	printf(" Resizing to zero size.\n");
	printf("*********************************************\n");
	errors = 0;
	printf("\tCreating with size = %d ...\n",SIZE1);
	arr = arr_create(SIZE1);
	if (arr == NULL) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tSetting %d elements ...\n",SIZE1);
    for (i = 0; i < SIZE1; i++)
    {
    	x = rand()%MAXELEM;
    	if (arr_setitem(arr, 0, x) < 0) errors++;
    }
    printf("\tDone with %d errors.\n\n",errors);
    errors = 0;
	printf("\tPrinting Array ...\n");
	if (arr_for_each(arr,(*print), NULL) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tResizing Array from %d to %d ...\n", SIZE1, 0);
	if (arr_resize(arr, 0) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	errors = 0;
	printf("\tPrinting Array ...\n");
	if (arr_for_each(arr,(*print), NULL) < 0) errors++;
	printf("\tDone with %d errors.\n\n",errors);
	arr_delete(arr);
    return 0;
}

