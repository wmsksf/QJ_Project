//
// Created by wmsksf on 16/10/19.
//

#include <cstdint>
#include "Utils.h"

static uint64_t partition(Tuple* A, uint64_t p, uint64_t r)
{
    uint64_t pivot = A[r].getKey();
    uint64_t i = p - 1;

    for (uint64_t j = p; j < r; j++)
        if (A[j].getKey() <= pivot)
        {
            i++;
            A[i].swap(&A[j]);
        }

    A[i + 1].swap(&A[r]);

    return i + 1;
}

void Quicksort(Tuple* A, uint64_t lo, uint64_t hi)
{
    if (lo < hi)
    {
        uint64_t q = partition(A, lo, hi);

        Quicksort(A, lo, q - 1);
        Quicksort(A, q + 1, hi);
    }
}

// OPT _ add ref
//static uint64_t MedianofThree_partition(Tuple* A, uint64_t p, uint64_t r)
//{
//    uint64_t x = A[p].getKey(), y = A[(r - p)/2 + p].getKey(), z = A[r - 1].getKey();
//    uint64_t i = p , j = r - 1;
//
//    // middle
//    if ((y > x && y < z) || (y > z && y < x) ) x = y;
//    else if ((z > x && z < y) || (z > y && z < x) ) x = z;
//
//    for(;;)
//    {
//        do {j--;} while (A[j].getKey() > x);
//        do {i++;} while (A[i].getKey() < x);
//
//        if  (i < j) A[i].swap(&A[j]);
//        else return j+1;
//    }
//
//}
//
//void Median_Quicksort(Tuple* A, uint64_t lo, uint64_t hi)
//{
//    if (hi - lo < 2) return;
//
//    uint64_t q = MedianofThree_partition(A, lo, hi);
//    Median_Quicksort(A, lo, q - 1);
//    Median_Quicksort(A, q + 1 , hi);
//}


//TMP TEST
//// _test in  main.cpp
//
//#include <iostream>
//#include "DataTypes.h"
//#include "Utils.h"
//
//void printA(Tuple* A, uint64_t size)
//{
//    for(uint64_t i = 0; i < size; i++) std::cout << A[i].getKey() << " ";
//    std::cout << std::endl;
//}
//
//int main()
//{
//    uint64_t n = 8;
//
//    Tuple *A;
//    A = new Tuple[8]{{100,0}, {1,2}, {23,1}, {12, 3}, {1,4}, {167,5}, {98,6}, {200,7}};
//
//    Quicksort(A, 0, n - 1);
//    std::cout << "Sorted array:" << std::endl;
//    printA(A, n);
//
////    Median_Quicksort(A, 0, n - 1);
////    std::cout << "Sorted array with median three:" << std::endl;
////    printA(A, n);
//    return 0;
//}