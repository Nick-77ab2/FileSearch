# CS 361 - File Search
Drexel University
Professor Mark Boady


## Overview


Searching files for  specific string of text is a common problem. Each file can be easily searching in its own thread. Spanning hundreds of threads to search hundreds of files would not be a good plan. This is a problem where a thread pool can come in handy. We can search some files concurrently, but we don't want to search all files concurrently.

## Concurrent File Search

Your program will takes two command line inputs.

- `target` - a string of text to search for
- `dir` - A directory to search


The program will find every line of every file in the directory (and all subdirectories) that include the target line. You must search all subdirectories, not just the primary directory. You may assume no one will create a sym-link cycle in the directory.

**Hint**: The `std::filesystem` namespace will be helpful. 

The program cannot run if a target is not given. If a directory is not given, use the current directory as the starting point.

Your file will be called `bin/search`. It will be called with the command line arguments

 ```text
 bin/search [target string] [directory]
```
 
 The second command line argument is optional but the first is required.

Break up your task into two components:

- Producer Thread
- Thread Pool

The **Producer Thread** should search for all files worth opening. There is no point in trying to open and read a file that will not contain the value. Your search will only look in files that contain the following extensions: cc, c, cpp, h, hpp, pl, sh, py, txt. Ignore any files with other extension you find in the directory. The **Producer Thread**'s job is to find all files with these extensions and pass them to the thread pool.

The **Thread Pool** will open up the files it is provided and search them for the target text. These threads should all be looking at files at the same time. You **must** have a minimum of 2 threads running. Attempt to use the number of threads given by `hardware_concurrency` but default to 2 if the number is less than 2.


You **must** make sure that your I/O and any shared data structures are thread safe. You should use mutex locks and conditional variables. You may implement them in Data Structures, but **you** have to make those data structures.

You **must** ensure the following:

- Files are search concurrently
- All files containing the results are found
- All threads exit gracefully once they run out of files to search.

Once you have completed your code, you will run a few tests and write your thoughts on how your code works. You will write your thoughts in a readme file.

## Output Style

This section describes the basic output style. An example execution is provided at the end of the instructions.

Start out my printing the target, directory, and number of threads being used.

```text
!---- Search Started ----!
Target Folder: /gcc
Target Text: <thread>
Using a Pool of 4 threads to search.
```

When a result is found, print out the thread that found it, file name, and line containing the target text.

```
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/gcc/testsuite/g++.dg/modules/xtreme-header-2.h"
Line 12: #include <thread>
----------
```

Print a completion message at the end. Remember you may find 0 returns, so the user still has to know their search ran.

```text
!---- Search Complete ----!
```

## Implementation

You are expected to write professional code. Use good variable and function names. Provide comments (in doxygen) explaining how the code works. Documents each function in the header file with comments. You will be graded on style as well as functionality.

## Citations

If you use any outside resources, talk about algorithm design with other students, or get help on assignments you **must** cite your resources in the comments. Uncited sources are an Academic Honesty Violation. Cited sources may lead to a deduction depending on the amount of code used, but will not violate Academic Honesty Policies. 

You are expect to write the majority of the code yourself and use resources for things like looking up commands. For example, if you forgot how to test if a file can be opened for reading you could look it up and cite a source. If you copy a critical algorithm and cite the code, you may still get a deduction for not developing the code yourself.

## Readme

Your readme will include both instructions and reflections on your code. It must be stored on the root of your folder structure. It must include the following. You must use markdown and call it readme.

There is no minimum or maximum length for the short essay questions, you are graded entirely on content. A short but comprehensive answer is better than a long confusing answer.

1) Your name and drexel ID (abc123@drexel.edu)

       Nicholas Pelletier, nwp28 

2) Instructions to run you code.

    regular instructions, make, make run, make clean etc. ./bin/search ["searchword"] [searchdirectory]

3) Short Essay Question 1: What data structures did you use/build? Why?

    I used a thread-safe queue (the example in the book). It's easiest to run a queue with this type of data. Given that we're giving a threadpool an amount of data and we have one task of "searching a file for a target string" we can just create a queue that gives the task of searching for that target thread in a given file to a thread in the pool. We don't need to look at that file again or run that exact task on that exact file again so it's fine to pop it off of the queue.

4) Short Essay Question 2: How does the thread pool get it's tasks?

    I partially explained this in the question above, but the threadpool gets its tasks through it's own submit function. This function, in short, adds a task of function type to the thread-safe queue and the threadpool pulls from the threadsafe-queue soon after if the thread taking the task is able to. And of course if there was a task to pull (otherwise it yields).

5) Short Essay Question 3: How does the thread pool know that it is finished all tasks?

    There's a joiner function. they're all joined. What occurs though is that the main producer thread is constantly giving the pool new tasks to pull from. The threadpool is creating workerthreads and these workerthread are looking for a done flag (atomic bool) to be set (each), if it's not set then the workerthreads continue to grab tasks off of the queue and continue.

6) Short Essay Question 4: What locks did you use? OR Why didn't use use any locks?

    Within the thread-safe queue there's locks and to be specific mutex locks. they were used to make the queue thread-safe. It was optimized in order to make each operation thread-safe instead of encapsulating the whole queue and making things quite inneficient. This efficient thread-safe queue was integral as a central system. Otherwise I just used a simple lock/unlock mutex for I/O operations and other small tasks.

7) Short Essay Question 5: What conditional variables did you use? OR Why didn't use use any conditional variables?

    there was a conditional variable used within the thread-safe queue in order to make the addition of and waiting on of data thread-safe. (data_cond).


## Makefile

You **must** provide a makefile to compile your code. We will type `make` and it **must** build your program. If there are any compile errors or a makefile is not provided we cannot test your code. We will test your code on custom folder. No `make run` is needed for this assignment.

You must have the following make targets:

1) `make`  - Builds the Program
2) `make clean` - Remove compiled code


## Other Requirements

If you submission does not meet the following guidelines we will not be able to grade it.

- You **must** use the C++ 17 Standard threads. No other thread libraries (pthreads, boost, etc) may be used. [https://en.cppreference.com/w/cpp/header/thread](https://en.cppreference.com/w/cpp/header/thread)
- Code **must** run on tux and be compiled with g++.
- All code **must** compile using the C++ 17 or above standard. (`--std=c++17`)
- All code **must** be submitted to the course github classroom.
- A working makefile **must** be provided.
- Must provide a readme file
- You may use libraries in the C++ 17 standard unless noted elsewhere in the instructions. [https://en.cppreference.com/w/cpp/header](https://en.cppreference.com/w/cpp/header)
- Your code **must** compile. You should always submit stable code, we will not debug code that does not compile.

## Grading

This homework is worth 100 points.

| Task | Points |
| ---- | ------ |
| Main: Handles Command Line Arguments | 2 |
| Main: Stars all threads | 2 |
| Main: Joins all threads before exiting | 2 |
| Main: Overall Style | 3 |
| Producer Threat | 25 |
| Thread Pool | 40 |
| Readme.md: Name and Email | 1 |
| Readme.md: Instructions | 1 |
| Readme.md: Short Essay 1 | 4 |
| Readme.md: Short Essay 2 | 4 |
| Readme.md: Short Essay 3 | 4 |
| Readme.md: Short Essay 4 | 4 |
| Readme.md: Short Essay 5 | 4 |
| Makefile is correct | 2 |
| Required File Structure in GitHub | 2 |

## Essay Answers

You may put your answers to the essay questions here. 

## Extended Example

To get a large folder structure to test on I used the GCC source code.

(https://gcc.gnu.org/git.html)[https://gcc.gnu.org/git.html]

OR

```text
git clone git://gcc.gnu.org/git/gcc.git SomeLocalDir
```

Once downloading it, I ran a search for the thread header.

```text
bin/search "<thread>" /gcc 
```

This is the full output of that execution.

```text
bin/search "<thread>" /gcc 
!---- Search Started ----!
Target Folder: /gcc
Target Text: <thread>
Using a Pool of 4 threads to search.
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libsanitizer/tsan/tsan_suppressions.cpp"
Line 30: // False positive when using std <thread>.
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/gcc/testsuite/g++.dg/modules/xtreme-header-2.h"
Line 12: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/gcc/testsuite/g++.dg/modules/xtreme-header.h"
Line 82: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/gcc/cp/name-lookup.c"
Line 6877:     /* <thread>.  */
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/gcc/cp/name-lookup.c"
Line 6878:     {"thread", "<thread>", cxx11},
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/gcc/cp/name-lookup.c"
Line 6879:     {"this_thread", "<thread>", cxx11},
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/include/bits/std_thread.h"
Line 289:   // N.B. other comparison operators are defined in <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/include/precompiled/stdc++.h"
Line 113: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/util/atomic/wait_notify_util.h"
Line 23: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/util/thread/all.h"
Line 28: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/27_io/basic_syncbuf/sync_ops/1.cc"
Line 32: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/timed_mutex/try_lock_until/4.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/timed_mutex/try_lock_until/3.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/timed_mutex/try_lock_until/57641.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/timed_mutex/unlock/2.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/shared_future/members/wait.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/packaged_task/members/invoke5.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/packaged_task/cons/3.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/latch/3.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/async/async.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/async/54297.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/lock/2.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/condition_variable_any/53830.cc"
Line 28: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/condition_variable_any/stop_token/wait_on.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/condition_variable_any/50862.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/barrier/arrive_and_drop.cc"
Line 34: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/barrier/completion.cc"
Line 34: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/barrier/arrive.cc"
Line 34: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/barrier/arrive_and_wait.cc"
Line 34: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/shared_mutex/try_lock/2.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/shared_mutex/unlock/1.cc"
Line 23: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/condition_variable/54185.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/condition_variable/members/3.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/recursive_mutex/unlock/2.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/recursive_timed_mutex/try_lock_until/3.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/recursive_timed_mutex/unlock/2.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/semaphore/try_acquire_for.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/semaphore/try_acquire_posix.cc"
Line 27: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/semaphore/try_acquire_until.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/semaphore/100806.cc"
Line 10: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/headers/thread/std_c++0x_neg.cc"
Line 22: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/headers/thread/types_std_c++20.cc"
Line 22: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/headers/thread/types_std_c++0x.cc"
Line 21: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/yield.cc"
Line 22: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/95989.cc"
Line 23: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/sleep_until-mt.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/1.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/60421.cc"
Line 27: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/sleep_for.cc"
Line 22: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/sleep_until.cc"
Line 22: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/sleep_for-mt.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/57060.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/this_thread/58038.cc"
Line 22: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/promise/60966.cc"
Line 28: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/mutex/try_lock/2.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/mutex/unlock/2.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/jthread/100612.cc"
Line 6: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/jthread/95989.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/jthread/1.cc"
Line 22: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/jthread/jthread.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/jthread/1.cc"
Line 25: # error "Feature-test macro for jthread missing in <thread>"
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/jthread/1.cc"
Line 27: # error "Feature-test macro for jthread has wrong value in <thread>"
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/jthread/3.cc"
Line 23: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/stop_token/stop_callback/destroy.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/call_once/39909.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/swap/1.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/84532.cc"
Line 21: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/adl.cc"
Line 21: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/native_handle/typesizes.cc"
Line 23: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/native_handle/cancel.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/members/4.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/members/1.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/members/5.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/members/2.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/members/3.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/members/hardware_concurrency.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/id/operators.cc"
Line 21: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/id/operators_c++20.cc"
Line 22: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/id/hash.cc"
Line 22: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/id/70294.cc"
Line 20: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/70503.cc"
Line 23: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/assign_neg.cc"
Line 21: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/4.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/1.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/terminate.cc"
Line 23: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/49668.cc"
Line 23: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/5.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/2.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/moveable.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/6.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/7.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/84535.cc"
Line 21: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/3.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/8.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/69724.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/lwg2097.cc"
Line 21: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/9.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/thread/cons/copy_neg.cc"
Line 21: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/future/members/wait.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/future/members/93456.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/shared_timed_mutex/try_lock_until/1.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/shared_timed_mutex/try_lock/2.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/shared_timed_mutex/try_lock/3.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/30_threads/shared_timed_mutex/unlock/1.cc"
Line 24: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/23_containers/vector/debug/multithreaded_swap.cc"
Line 30: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/17_intro/headers/c++1998/49745.cc"
Line 79: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/29_atomics/atomic_float/wait_notify.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/29_atomics/atomic_integral/wait_notify.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/29_atomics/atomic/wait_notify/generic.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/29_atomics/atomic/wait_notify/bool.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/29_atomics/atomic/wait_notify/pointers.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/29_atomics/atomic_flag/wait_notify/1.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/29_atomics/atomic_ref/wait_notify.cc"
Line 25: #include <thread>
----------
----------
Thread 0x700003b88000 found a match.
File: "/gcc/libstdc++-v3/testsuite/26_numerics/random/random_device/94087.cc"
Line 26: #include <thread>
----------
----------
Thread 0x700003c8e000 found a match.
File: "/gcc/libstdc++-v3/testsuite/performance/30_threads/future/polling.cc"
Line 20: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/performance/20_util/memory_resource/pools.cc"
Line 135: # include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/20_util/shared_ptr/atomic/3.cc"
Line 23: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/testsuite/20_util/shared_ptr/thread/default_weaktoshared.cc"
Line 32: #include <thread>
----------
----------
Thread 0x700003c0b000 found a match.
File: "/gcc/libstdc++-v3/testsuite/20_util/shared_ptr/thread/mutex_weaktoshared.cc"
Line 32: #include <thread>
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/src/c++11/thread.cc"
Line 27: #include <memory> // include this first so <thread> can use shared_ptr
----------
----------
Thread 0x700003d11000 found a match.
File: "/gcc/libstdc++-v3/src/c++11/thread.cc"
Line 28: #include <thread>
----------
!---- Search Complete ----!

```
