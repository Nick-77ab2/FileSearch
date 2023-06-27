#include "threadmerge.h"

// divide the array into sections of t_size = size // threadCount
// Main thread will sort the last part
// Then merge all the parts together (runtime = O(n))
void tmergesort(int* A, int size, int threadCount)
{
    int t_size, mod, i, middle, stop;
    std::thread thread_pool[threadCount];

    // No need to run threaded mergesort in these cases
    if (threadCount == 1 || size < threadCount) {
        mergesort(A, size);
        return;
    }

    // Calculate job size for each thread
    t_size = size / threadCount;
    mod = size % threadCount;
    
    // start threads
    for (i = 0; i < threadCount; i++)
        thread_pool[i] = std::thread(msortSec, A + t_size*i, 0, t_size - 1);

    // Sort the last part using main thread
    if (mod > 0)                                    // Why leave the main thread chilling
        mergesort(A + t_size*threadCount, mod);     // when it should be working?

    // Join child threads
    for (i = 0; i < threadCount; i++)
        thread_pool[i].join();
    
    // merge all sections
    for (i = 0; i < threadCount - 1; i++) {
        middle = t_size*(i + 1) - 1;
        stop = t_size*(i + 2) - 1;
        merge(A, 0, middle, stop);
    }

    // Merge the last section (if exists)
    if (mod > 0) {
        middle = t_size*threadCount - 1;
        stop = size - 1;
        merge(A, 0, middle, stop);
    }

    return;
}