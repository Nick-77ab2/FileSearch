/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date 2021-2022
 @section Description
 
 This header defines a classic textbook Merge Sort Algorithm.
 */

//--You may ADD to this file but you may not change any code given---

#ifndef _MERGESORT_H_
#define _MERGESORT_H_

#include <stdlib.h>
#include <string>
#include <iostream>

/**
 Sort Array A using Merge Sort. Array is sorted in place.
 @param A is the array to sort
 @param size is the number of elements in the array
 */
void mergesort(int* A, int size);
/**
 Merge Sort Array values between index start and stop
 @param A is the array to sort
 @param start is the first index to sort
 @param stop is the last index to sort
 */
void msortSec(int* A, int start, int stop);
/**
 Merge two sorted sections of the array. Specifically,
 merge A[start to middle] an A[middle+1 to stop].
 @param A is the array to merge elements in
 @param start is the first value in the first section
 @param middle is the last value in the first section
 @param stop is the last element in the second section
 */
void merge(int* A, int start, int middle, int stop);

#endif
