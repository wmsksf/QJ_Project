//
// Created by wmsksf on 16/10/19.
//

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
//#include <iostream>

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


//IN PROCESS...
void Radixsort(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte = 0, Relation* RR = nullptr)
{
    if ((end - start) * sizeof(Tuple) < L1_CACHESIZE)
    {
        Quicksort(R->getTuples(), start, end-1);
        return;
    }

    if (RR == nullptr)
    {
        RR = new Relation;
        RR->setNumTuples(R->getNumTuples());
        RR->initTuplesVal(R);
    }

    uint64_t Hist[256] = {0};
    if(!current_byte)
        for (uint64_t i = 0; i < 256; i++)
            Hist[R->getTuples()[i].getPayload() & 0xff]++;
    else
        for (uint64_t i = 0; i < 256; i++)
            Hist[(R->getTuples()[i].getPayload() >> current_byte) & 0xff]++;

    uint64_t Psum[256] = {0};
    for (uint64_t i = 1; i < 256; i++)
    {
        Psum[i] = Psum[i-1] + Hist[i-1];
        if ((Psum[i] - Psum[i-1]) * sizeof(Tuple) > L1_CACHESIZE)
            if (current_byte % 2)
                Radixsort(RR, Psum[i-1], Psum[i], current_byte+8, R);
            else
                Radixsort(R, Psum[i-1], Psum[i], current_byte+8, RR);
    }
}


Tuple getMatrixSize(const char *fileName) {

    //Opening the input file
    FILE* fp;
    char buffer[SIZE + 1], lastchar = '\n';
    size_t bytes;
    int lines = 0;
    int columns = 0;
    fp = fopen(fileName,"r");     //Opening the file

    if (fp == NULL) {   //checking for error with fopen
        printf("here..\n");
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char c = getc(fp);
    while(c != '\n'){
        if(c == '|')
            columns++;
        c = getc(fp);
    }
    lines++;

    //A look to count the lines in our input file, which will also be the number of rows.
    while ((bytes = fread(buffer, 1, sizeof(buffer) - 1, fp))) {
        lastchar = buffer[bytes - 1];
        for (char *c = buffer; (c = (char*)(memchr(c, '\n', bytes - (c - buffer)))); c++) {
            lines++;
        }
    }
    if (lastchar != '\n')
        lines++;  /* Count the last line even if it lacks a newline */

    fclose(fp);

    Tuple tmp;
    tmp.setKey(columns);
    tmp.setPayload(lines);

    return tmp;
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
