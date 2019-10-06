/****************************************
 * Title  : Sorting    					*
 * Author : Ran Shieber                 *
 * Group  : OL712                        *
 * Date   : 08/09/2019               	*
 ****************************************/

#ifndef __ILRD_OL712_SORTING_H
#define __ILRD_OL712_SORTING_H

#include <stddef.h> /* size_t */

/*
 * Sorts @arr using insertion sort.
 * @arr: array to sort.
 * @size: number of elements in arr.
 */
void Insertion(int *arr, size_t size);

/*
 * Sorts @arr using selection sort.
 * @arr: array to sort.
 * @size: number of elements in arr.
 */
void Selection(int *arr, size_t size);

/*
 * Sorts @arr using bubble sort.
 * @arr: array to sort.
 * @size: number of elements in arr.
 */
void Bubble(int *arr, size_t size);

/*
 * Sorts @arr using counting sort
 * @arr: array to sort.
 * @size: number of elements in arr (must be at least 1).
 * @min: at most the smallest element in @arr.
 * @max: at least the biggest element in @arr.
 * return: 0 if success, non-zero otherwise
 */
int Counting(int arr[], size_t size, int min, int max);

/*
 * Sorts @arr using radix sort
 * @arr: array to sort.
 * @size: number of elements in arr (must be at least 1).
 * return: 0 if success, non-zero otherwise
 */
int Radix(int arr[], size_t size, unsigned int num_of_bits);

/*
 * Sorts @arr using merge sort
 * @arr: array to sort.
 * @size: number of elements in arr (must be at least 1).
 * return: 0 if success, non-zero otherwise
 */
int Merge(int arr[], size_t arr_length, int(*is_before)(int num1, int num2));

/*
 * Sorts @arr using quick sort
 * @arr: array to sort.
 * @size: number of elements in arr (must be at least 1).
 * return: 0 if success, non-zero otherwise
 */
void QSort(void *base, size_t num_of_elements, size_t element_size, 
           int(*cmp_func)(const void *, const void *));

#endif /* __ILRD_OL712_SORTING_H */
