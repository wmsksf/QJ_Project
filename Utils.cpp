//
// Created by wmsksf on 16/10/19.
//

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

    return Tuple(columns, lines);
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
