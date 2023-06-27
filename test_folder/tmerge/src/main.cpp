/**
 @mainpage CS361 - Merge Sort Homework
 @section Description
 
 This program runs a series of tests on a classic mergesort and a threaded mergesort. Once the tests are completed, it builds a table of timings comparing the classic and threaded merge sort.
 
 Make Commands:
 
     make
 
 will build the executable.
 
     make run
 
 will run the experiments.
 
     make clean

will clear out the compiled code.
 
    make doc
 
will build the doxygen files.
 
*/

/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date 2021-2022
 @section Description
 
 The main program just runs all the tests defined elsewhere and prints out output. Then in collects the timings and also prints out output. It is primarily concerned with I/O design.
 */

//--ONLY MAKE CHANGES WHERE NOTED--

#include <iostream>
#include <chrono>
#include <string>
//Custom Files
#include "mergesort.h"
#include "threadmerge.h"
//Libraries to test the sorts
#include "testLib.h"

/**
 Test Threaded Merge Sort with 2 Threads
 @param A is a pointer to the array to sort
 @param size is the size of the array
 */
void threadMerge2(int* A, int size){
	tmergesort(A,size,2);
}
/**
 Test Threaded Merge Sort with 4 Threads
 @param A is a pointer to the array to sort
 @param size is the size of the array
 */
void threadMerge4(int* A, int size){
	tmergesort(A,size,4);
}

void threadMerge8(int* A, int size)
{
	tmergesort(A, size, 8);
}
/**
 Test Threaded Merge Sort with 16 Threads
 @param A is a pointer to the array to sort
 @param size is the size of the array
 */
void threadMerge16(int* A, int size){
	tmergesort(A,size,16);
}
/**
 Test Threaded Merge Sort with 32 Threads
 @param A is a pointer to the array to sort
 @param size is the size of the array
 */
void threadMerge32(int* A, int size){
	tmergesort(A,size,32);
}

/**
 Run all timings and print a table. Tests sizes 0 to 2^b.
 @param b is the max power of two to use in the table
*/
void runTimings(int b);

/**
 Time execution of a function on an array.
 @param size is the number of elements in the array
 @param func is the function to time
 @return The time taken in microseconds
 */
double timeFunc(int size, void (*func)(int* A, int n));

/**
 Run all tests then run timings.
 @param argc is not used
 @param argv is not used
 @return is always 0
 */
int main(int argc, char** argv)
{
	//You may change these two variables
	int maxTextSize=13;
	int maxTableSize=24;
	
	//Run Tests
	std::cout << "Test Classic Merge Sort" << std::endl;
	fullTestBed(maxTextSize,mergesort);
	
	std::cout << "Test Threaded Merge Sort with 2 Threads" << std::endl;
	fullTestBed(maxTextSize,threadMerge2);
	
	std::cout << "Test Threaded Merge Sort with 4 Threads" << std::endl;
	fullTestBed(maxTextSize,threadMerge4);

	std::cout << "Test Threaded Merge Sort with 8 Threads" << std::endl;
	fullTestBed(maxTextSize,threadMerge8);

	std::cout << "Test Threaded Merge Sort with 16 Threads" << std::endl;
	fullTestBed(maxTextSize,threadMerge16);
	
	std::cout << "Test Threaded Merge Sort with 32 Threads" << std::endl;
	fullTestBed(maxTextSize,threadMerge32);
	
	//Print a Table of Timings
	runTimings(maxTableSize);
	
	return 0;
}



//Runs all timings and makes a table
//Inputs: Max Size by exponent of 2
//	for int b times size 1 to 2^b
//Outputs: Done
//Side Effects: Prints Table to Screen
void runTimings(int b){
	std::string t1="Array Size";
	std::string t2="Merge";
	std::string t3="TMerge 2";
	std::string t4="TMerge 4";
	std::string t8="TMerge 8";
	std::string t5="TMerge 16";
	std::string t6="TMerge 32";
	printf("Time in Microseconds shown.\n");
	printf("Time to create arrays is not counted. Only sorting.\n");
	printf("| %10s | %9s | %9s | %9s | %9s | %9s | %9s |\n",
		t1.c_str(),
		t2.c_str(),
		t3.c_str(),
		t4.c_str(),
		t8.c_str(),
		t5.c_str(),
		t6.c_str());
	for(int p=0; p <= b; p++){
		int size = fastPow(2,p);
		double r1 = timeFunc(size,mergesort);
		double r2 = timeFunc(size,threadMerge2);
		double r3 = timeFunc(size,threadMerge4);
		double r4 = timeFunc(size, threadMerge8);
		double r5 = timeFunc(size,threadMerge16);
		double r6 = timeFunc(size,threadMerge32);
		printf("| %10d | %9.0f | %9.0f | %9.0f | %9.0f | %9.0f | %9.0f |\n"
			   ,size,r1,r2,r3,r4,r5, r6);
	}
}

//Time a single sort
//for a single size
//Inputs: Size and Function to time
//Outputs: Time taken
//Side Effects: None
double timeFunc(int size, void (*func)(int* A, int n)){
	//Make Array
	int* A = randomArray(size);
	//Record Start Time
	auto start = std::chrono::high_resolution_clock::now();
	func(A,size);
	//Record Stop Time
	auto end = std::chrono::high_resolution_clock::now();
	//Compute Time Taken
	double time_taken =
      std::chrono::duration_cast<std::chrono::microseconds>(
		end - start).count();
	//Free My Array
	free(A);
	return time_taken;
}
