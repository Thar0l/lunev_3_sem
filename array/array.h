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
#include <errno.h>


struct array;

struct array *arr_create(unsigned int size);
void arr_delete(struct array *arr);
int arr_resize(struct array *arr, unsigned int size);
int arr_getitem(struct array *arr, unsigned int index, unsigned int *value);
int arr_setitem(struct array *arr, unsigned int index, unsigned int value);
int arr_for_each (struct array *arr, unsigned int (*foo) (unsigned int a, void *data), void *data);

#endif /* ARRAY_H_ */
