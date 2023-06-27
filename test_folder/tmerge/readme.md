# CS 361 - Homework 04
**Author**: Mark Boady
**Term**: Fall 2022-2023

## Overview


Merge Sort is an algorithm that very naturally fits into a parallel computation model. It is by nature a divide and conquer algorithm. Merge Sort sorts and array by cutting it in half, sorting both sides, and merging to back together. The tradition Merge Sort Algorithm has a runtime of $O(n \log n)$.

We *should* be able to do better with threads. Each component of the array can be sorted completely independent of the rest. We just need all the values for the final merge. If we have $t$ threads, we should be able cut the time down to $O(\frac{n}{t} \log \frac{n}{t})+O(n)$. We cut the array in $t$ subsections, each with $\frac{n}{t}$ elements. Then we sort in all those threads at the same time. Once we are done with our threads, we have an array with $t$ sorted sections. We need all the final data once at the end, but we can get very close to sorted without having all the data in any one thread.

The goal of this assignment is to see if you can improve the speed of Merge Sort using threads.

**IMPORTANT**: It is entirely possible your threaded sort will not be faster. Just because it works in theory, doesn't mean your implementation will be faster. If this happens, you will not lose points, but you should think about why in the report questions. It might help to put time different sections of the sort and see what it taking the longest time.

## Merge Sort Algorithm

The Merge Sort Algorithm is normally implemented using three functions. 

The first function is an interface for users. It takes the array and the size of the array. The recursive implementation needs to know the start and end point. Most programmers tend to think about sorting an entire array of a certain size.

```text
Function mergesort(array A, int size)
	msortSec(A,0, size - 1)
EndFunction
```

We generally then make a function to do the actual recursive Merge Sort sections.

```text
Function msortSec(A,start,stop)
	If(start < stop)
		Return
	EndIf
	Set middle = start + floor( (stop-start)/2)
	msortSec(A,start,middle)
	msortSec(A,middle+1,stop)
	merge(A,start,middle,stop)
EndFunction
```

When we call the `merge` algorithm, we have two sorted subsections of the Array. We know that the array is sorted from `start` to `middle`. We also know that it is sorted from `middle+1` to `stop`. Knowing this, we can sort the values in only $O(n)$ time. 

```text
Function merge(A,start,middle,stop)
	Create a new Array Aux with stop-start+1 spaces
	Copy elements from A to Aux 
	(Aux[0]=A[start], Aux[1]=A[start+1], etc)
	Let auxMiddle=(middle-start) and auxStop=(stop-start)
	Let i=0 and j=auxMiddle+1
	For(k=start, k < stop+1, k++)
		If( i > auxMiddle)
			A[k] = Aux[j]; j++
		ElseIf(j > auxStop)
			A[k]=Aux[i]; i++
		ElseIf(Aux[j] > Aux[i])
			A[k] = Aux[i]; i++
		Else
			A[k] = Aux[j]; j++
		EndIf
	EndFor
	Delete Array Aux
EndFunction
```

The `merge` function has a runtime of $O(n)$. The entire sort has a runtime of $O(n \log n)$.

##Threaded Merge

You will implement a function `tmergesort` that takes one additional input, the number of threads. This is the front-end for your sort. You will need to make additional helper functions. This main interface function may not be changed because it will be used by the provided code template.

```text
Function tmergesort(Array A, int size, int numThreads)
	If(numThreads == 1 or size < numThreads)
		mergesort(A,size) #No Gains from Threading
		Return
	EndIf
	Apply your thread approach here.
EndFunction
```

You may implement this function however you like. It does not need to be recursive. It just needs to use the basic ideas from Merge Sort (divide into section, sort, then merge).


## Programming Assignment

Develop a command line program in C++. You will be provided with a template zip file that will test and time the functions you are implementing. **Do Not** change the tests. Only the Merge Sort component.

### Program Specs

The final compiled program will be named `mergesort`. It takes no command line arguments.

### Implementation

You are expected to write professional code. Use good variable and function names. Provide comments using doxygen explaining how the code works. Document each function in the header file with comments in doxygen style. You will be graded on style as well as functionality.

## Citations

If you use any outside resources, talk about algorithm design with other students, or get help on assignments you **must** cite your resources in the comments. Uncited sources are an Academic Honesty Violation. Cited sources may lead to a deduction depending on the amount of code used, but will not violate Academic Honesty Policies. 

You are expect to write the majority of the code yourself and use resources for things like looking up commands. For example, if you forgot how to test if a file can be opened for reading you could look it up and cite a source. If you copy a critical algorithm and cite the code, you may still get a deduction for not developing the code yourself.

## Makefile

You will be provided with a makefile for this assignment. You may not change the provided file.

You are provided three commands.

- `make` - Builds the Program
- `make run` - Runs the Program
- `make clean` - Cleans out all compiled files and documentation
- `make doc` - Generates Doxygen Documentation files

## Other Requirements

If you submission does not meet the following guidelines we will not be able to grade it.

- You **must** use the C++ 17 Standard threads. No other thread libraries (pthreads, boost, etc) may be used. [https://en.cppreference.com/w/cpp/header/thread](https://en.cppreference.com/w/cpp/header/thread)
- Code **must** run on tux and be compiled with g++.
- All code **must** compile using the C++ 17 or above standard. `--std=c++17`
- All code **must** be submitted in a single git.
- Report described below **must** be included git.
- A working makefile **must** be provided.
- You may use libraries in the C++ 17 standard unless noted elsewhere in the instructions. [https://en.cppreference.com/w/cpp/header](https://en.cppreference.com/w/cpp/header)
- Your code **must** compile. You should always submit stable code, we will not debug code that does not compile.


## Report

The provided template code will run timings on your threaded sort and classic sort. Use these timings to answer the following questions. You may want to test on different systems with different numbers of threads to see what happens. (For example, TUX and your local computer.) It may help to provide plots of the times to make it more clear what the results are. (Plots are not required, display the data in the way that you feel is most convincing to show what happened.)

Submit a PDF with the data you collected and answers to the following questions. 


1. Provide the timings you collected in tables/plots/etc. 
2. Did threading improve the speed of sorting? If yes, where do you think the advantage came from? If no, what do you think could have caused this?
3. What was the most difficult part of coding this assignment?
4. What was the easiest part of coding this assignment?

Put all tables, plots, and answers into a single pdf `abs123\_report.pdf` and submit it with your code.


## Grading

This homework is worth 100 points.

| Points | Section |
| ------ | ------- |
| 15 | Classic Merge: Implementation, Style, and Design |
| 50 | Threaded Merge Sort: Using Threads to sort individual sections of the array |
| 10 | Threaded Merge Sort: Combing Results from individual threads into final sorted array |
| 2 |Overall Style of code and variable names |
| 1 | File Names are correct |
| 2 | All required files submitted |
| 10 | Report: Data Collection and Presentation (Tables, Plots, etc) |
| 4 | Report: Speed Improvement Short Answer Question |
| 3 | Report: Difficult Short Answer |
| 3 | Report: Easiest Short Answer |
