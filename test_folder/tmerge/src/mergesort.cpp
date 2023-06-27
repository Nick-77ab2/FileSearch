#include "mergesort.h"

void mergesort(int *A, int size)
{
    msortSec(A, 0, size - 1);
}

void msortSec(int *A, int start, int stop)
{
    int middle;

    if (start >= stop)
        return;
    
    middle = start + ((stop - start) / 2);
    msortSec(A, start, middle);
    msortSec(A, middle + 1, stop);
    merge(A, start, middle, stop);
}

void merge(int *A, int start, int middle, int stop)
{
    int *aux, aux_size, aux_middle, aux_stop, k, i, j;

    // copy from A to aux
    aux_size = stop - start + 1;
    aux = new int[aux_size];
    for (i = 0; i < aux_size; i++)
        aux[i] = A[i + start];
    
    // init aux_middle
    aux_middle = middle - start;
    aux_stop = stop - start;

    // i = 0 and j = aux_middle + 1
    i = 0;
    j = aux_middle + 1;

    for (k = start; k <= stop; k++) {
        if (i > aux_middle) {
            A[k] = aux[j];
            j++;
        }
        else if (j > aux_stop) {
            A[k] = aux[i];
            i++;
        }
        else if (aux[j] > aux[i]) {
            A[k] = aux[i];
            i++;
        }
        else {
            A[k] = aux[j];
            j++;
        }
    }

    delete[] aux;
}