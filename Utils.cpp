//
// Created by wmsksf on 16/10/19.
//

#include <cstdint>
//#include <iostream>

#include "Utils.h"

static void swap(uint64_t* a, uint64_t* b)
{
    uint64_t tmp = *a;
    *a = *b;
    *b = tmp;
}

static uint64_t partition(uint64_t* A, uint64_t p, uint64_t r)
{
    uint64_t pivot = A[r];
    uint64_t i = p - 1;

    for (uint64_t j = p; j < r; j++)
        if (A[j] <= pivot)
        {
            i++;
            swap(&A[i], &A[j]);
        }

    swap(&A[i+1], &A[r]);

    return i + 1;
}

void Quicksort(uint64_t* A, uint64_t lo, uint64_t hi)
{
    if (lo < hi)
    {
        uint64_t q = partition(A, lo, hi);

        Quicksort(A, lo, q - 1);
        Quicksort(A, q + 1, hi);
    }
}


//void printA(uint64_t* A, uint64_t size)
//{
//    for(uint64_t i = 0; i < size; i++) { std::cout << A[i]; std::cout << " "; }
//    std::cout << std::endl;
//}
//
//int main()
//{
//    uint64_t A[] = {10, 8 ,4 ,112, 45, 22, 1, 8, 3};
//    uint64_t n = sizeof(A)/ sizeof(uint64_t);
//
//    Quicksort(A, 0, n - 1);
//    std::cout << "Sorted array:" << std::endl;
//    printA(A, n);
//
//    return 0;
//}
