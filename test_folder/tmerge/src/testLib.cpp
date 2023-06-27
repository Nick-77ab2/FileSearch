/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date 2021-2022
 @section Description
 
 Implementation of sort unit testing library.
 */

#include <stdio.h> //For printf, malloc, and free
#include <stdlib.h> //For rand

//Functions to help test sorts

//--You may not change this file--

#include "testLib.h"

//Generate a random array
//Input: Size to generate
//Output: Pointer to array
//Side Effects: none
//Picks numbers from 0 to (5*size)-1 for
//easy readability
int* randomArray(int size){
	//Make the Array
	int* X = (int*)malloc(size*sizeof(int));
	for(int i=0; i < size; i++){
		X[i] = rand()%(5*size);
	}
	return X;
}

//Print an array to help students debug
//Input: Array of integers and size of array
//Output: None
//Side Effects: Prints the array
void printArray(int* A, int size){
	printf("[");
	for(int i=0; i < size; i++){
		if(i+1 < size){
			printf("%d, ",A[i]);
		}else{//no comma for last value
			printf("%d",A[i]);
		}
	}
	printf("]\n");
}

//Make A Copy of an array to compare against
//Input: Int Array and Size to copy
//Output: Copy of Array
//Side Effects: None
int* copyArray(int* A, int size){
	int* B = (int*)malloc(size*sizeof(int));
	for(int i=0; i < size; i++){
		B[i] = A[i];
	}
	return B;
}

//Check if an array is sorted
//Input: Int Array and Size
//Output: 1 if sorted, 0 is unsorted
//Side Effects: None
int isSorted(int* A, int size){
	for(int i=1; i < size; i++){
		if(A[i] < A[i-1]){
			return 0;
		}
	}
	return 1;
}

//Check all the same values appear in the array
//Input: Two arrays and their sizes
//Output: 0 if they contain different numbers
//		1 if all numbers appear in both
//Side Effects: None
int hasSameValues(int* A, int* X, int sizeA, int sizeX){
	if(sizeA != sizeX){
		return 0;
	}
	for(int i=0; i < sizeA; i++){
		//Count how many time each number appears
		int c1 = count(A,sizeA,X[i]);
		int c2 = count(X,sizeX,X[i]);
		//Check it is the same
		if(c1!=c2){
			return 0;
		}
	}
	return 1; //No Errors Found
}

//Count how many times target appears in A
//Input: Array and size
//		target to count
//Output: number of times target appears
//Side Effects: none
int count(int*A, int size, int target){
	int total=0;
	for(int i=0; i < size; i++){
		if(A[i]==target){
			total++;
		}
	}
	return total;
}

//Test a sort function
//Input: Size n to test and function to test
//Output: 0 if failed or 1 if passed
//Side Effects: Prints out why failure happened
int runTest(int n, void (*func)(int* A, int size)){
	int* Input = randomArray(n);
	int* Org = copyArray(Input,n);
	//Run the Sort
	func(Input,n);
	//Is it sorted?
	if(isSorted(Input,n)!=1){
		printf("Test Failed (List Unsorted)\n");
		printf("Input: ");
		printArray(Org,n);
		printf("Output: ");
		printArray(Input,n);
		//Delete the Arrays
		free(Input);
		free(Org);
		return 0;
	}
	//Are all the elements still here
	if(hasSameValues(Input, Org, n, n)!=1){
		printf("Test Failed (Missing Elements)\n");
		printf("Input: ");
		printArray(Org,n);
		printf("Output: ");
		printArray(Input,n);
		//Delete the Arrays
		free(Input);
		free(Org);
		return 0;
	}
	//Delete the Arrays
	free(Input);
	free(Org);
	return 1;
}

//Run k tests at one size
//Input: size to test (n) and number of tests (k)
//		function to test func
//Outputs: 0 if failed any test and 1 if passed all
//Side Effects: Prints What is being tests
int runMultTests(int n, int k, void (*func)(int* A, int size)){
	printf("Testing Arrays of Size: %d\n",n);
	int passed=0;
	for(int i=0; i < k; i++){
		passed+=runTest(n,func);
	}
	printf("Passed %d out of %d Tests.\n",passed,k);
	//return 1 if they all didn't pass
	if(passed!=k){
		return 0;
	}
	return 1;
}

//Run A Full Set of Tests
//Inputs: maxExp is the max power of 2 to test
//		func is the function to test
//Outputs: 1 if passed all and 0 otherwise
//Side Effects: Prints Details of tests
int fullTestBed(int maxExp, void (*func)(int* A, int size) ){
	int testsToRun = 10;
	for(int i=0; i < maxExp; i++){
		int myTestSize = fastPow(2,i);
		int results = runMultTests(myTestSize,testsToRun,func);
		if(results==0){
			return 0;
		}
	}
	return 1;
}

//Fast Power
//Input: base and exponent
//Output: base^exponent
//Side Effects: None
int fastPow(int base, int exp){
	if(exp==0){
		return 1;
	}
	if(exp%2==1){
		return base*fastPow(base,exp-1);
	}
	int temp = fastPow(base,exp/2);
	return temp*temp;
}
