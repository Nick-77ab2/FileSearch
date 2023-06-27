/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date 2021-2022
 @section Description
 
 Functions to help test sorts
 */

//--You may not change this file--
#ifndef _TEST_LIB_H_
#define _TEST_LIB_H_

/**
 Generate a Random Array
 @param size is the number of random elements in the array
 @return Pointer to the newly created array
*/
int* randomArray(int size);
/**
 Print an Array
 @param A is the array to print
 @param size is the number of elements in the array
*/
void printArray(int* A, int size);
/**
 Copy an Array
 @param A is the array to make a copy of
 @param size is the number of elements in A
 @return A pointer to a new array with the same contents
*/
int* copyArray(int* A, int size);
/**
 Check If Sorted
 @param A is the array to check
 @param size is the number of elements in the array
 @return 1 if the array is sorted and 0 otherwise
*/
int isSorted(int* A, int size);
/**
 Check that two arrays contain all the same values.
 @param A is the first array
 @param X is the second array
 @param sizeA is the number of elements in A
 @param sizeX is the number of elements in X
 @return 1 if the array is sorted and 0 otherwise
*/
int hasSameValues(int* A, int* X, int sizeA, int sizeX);
/**
 Count how many times target appears in A.
 @param A is the array to search
 @param size is the number of elements in A
 @param target is the number to search for
 @return The number of times target appears in A
 */
int count(int*A, int size, int target);
/**
 Test a merge function for a specific array size
 @param n is the size of the array to test with
 @param func is the function to test sorting with
 @return 1 for success and 0 for failure
 */
int runTest(int n, void (*func)(int* A, int size));
/**
 Run k tests all with the same size
 @param n is the size of the array to test
 @param k is the number of tests to run
 @param func is the sort function to test with
 @return 1 for success and 0 for failure
 */
int runMultTests(int n, int k, void (*func)(int* A, int size));
/**
 Test all sizes from 0 to 2^maxExp going in powers of 2
 @param maxExp is the largest power of 2 to test with
 @param func is the function to run all the tests on
*/
int fullTestBed(int maxExp, void (*func)(int* A, int size) );
/**
 Compute the exponent base^exp quickly.
 @param base is the base of the exponent
 @param exp is the exponent to raise the base to
 @return the result of base^exp
 */
int fastPow(int base, int exp);

#endif
