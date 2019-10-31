//
// Created by wmsksf on 16/10/19.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Utils.h"
#include "LinkedList.h"
//
//static uint64_t partition(Tuple* A, uint64_t p, uint64_t r)
//{
//    uint64_t pivot = A[r].getKey();
//    uint64_t i = p - 1;
//
//    for (uint64_t j = p; j < r; j++)
//        if (A[j].getKey() <= pivot)
//        {
//            i++;
//            A[i].swap(&A[j]);
//        }
//
//    A[i + 1].swap(&A[r]);
//
//    return i + 1;
//}
//
//void Quicksort(Tuple* A, uint64_t lo, uint64_t hi)
//{
//    if (lo < hi)
//    {
//        uint64_t q = partition(A, lo, hi);
//
//        if(q)
//            Quicksort(A, lo, q - 1);
//        else
//            Quicksort(A, lo, q);
//
//        Quicksort(A, q + 1, hi);
//    }
//}

// OPT: USING MEDIANOFTHREE AND HOARE PARTITION
static uint64_t partition(Tuple* A, uint64_t p, uint64_t r)
{
    uint64_t x = A[p].getKey(), y = A[(r - p)/2 + p].getKey(), z = A[r - 1].getKey();
    uint64_t i = p , j = r - 1;

    // middle
    if ((y > x && y < z) || (y > z && y < x) ) x = y;
    else if ((z > x && z < y) || (z > y && z < x) ) x = z;

    for(;;)
    {
        do {j--;} while (A[j].getKey() < x);
        do {i++;} while (A[i].getKey() > x);

        if  (i < j) A[i].swap(&A[j]);
        else return j+1;
    }
}

void Quicksort(Tuple* A, uint64_t lo, uint64_t hi)
{
    if (hi - lo < 2) return;

    uint64_t q = partition(A, lo, hi);
    Quicksort(A, lo, q - 1);
    Quicksort(A, q + 1 , hi);
}

//IN PROCESS...
void Radixsort(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte, Relation* RR)
{
    if (current_byte == 56 && (end+1 - start) * sizeof(Tuple) < L1_CACHESIZE)
    {
        Quicksort(R->getTuples(), start, end);
        return;
    }

    if (RR == nullptr)
    {
        RR = new Relation;
        RR->setNumTuples(R->getNumTuples());
        RR->initTuples();
    }

    uint64_t Hist[256] = {0};
    for (uint64_t i = start; i <= end; i++)
        Hist[(R->getTuples()[i].getKey() >> current_byte) & 0xff]++; // for byte 0 same as A[i] & 0xff

    uint64_t Psum[256] = {0};
    for (int i = 1; i < 256; i++)
        Psum[i] = Psum[i-1] + Hist[i-1];

    // utility in copying R to RR
    uint64_t tmp[256] = {0};
    for (uint64_t i = 0; i < 256; i++) tmp[i] = Psum[i];

    Tuple tuple;
    for (uint64_t i = start; i <= end; i++)
    {
        tuple = R->getTuples()[i];
        uint64_t byte = (tuple.getKey() >> current_byte) & 0xff;

        RR->setTupleVal(tmp[byte]++, tuple.getKey(), tuple.getPayload());
    }


    uint64_t nth_byte = current_byte/8; // switch R, RR after byte checked
    for (uint64_t i = 1; i < 256; i++)
    {
        if ((Psum[i] - Psum[i-1]) * sizeof(Tuple) > L1_CACHESIZE)
        {
            if (!current_byte)
                Quicksort(RR->getTuples(), Psum[i-1], Psum[i]-1);
            else
                Radixsort(RR, Psum[i - 1], Psum[i]-1, current_byte - 8, R);
        }
        else{
            Quicksort(RR->getTuples(), Psum[i-1], Psum[i]-1);
        }
    }

    if ((end - Psum[255]) * sizeof(Tuple) > L1_CACHESIZE)
    {
        if (!current_byte)
            Quicksort(RR->getTuples(), Psum[255], end);
        else
            Radixsort(RR, Psum[255], end, current_byte - 8, R);
    }
    else{
        Quicksort(RR->getTuples(), Psum[255], end);
    }

    if(nth_byte%2){
        R->initTuplesVal(RR);
        delete RR;
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

LinkedList* SortMergeJoin(Relation* relA, Relation* relB) {

    if(relA == nullptr or relB == nullptr){
        std::cout << "Relations can't be NULL!" << std::endl;
        return nullptr;
    }
    Tuple* tupA = relA->getTuples();
    Tuple* tupB = relB->getTuples();
    uint64_t sizeA = relA->getNumTuples();
    uint64_t sizeB = relB->getNumTuples();
    if(tupA == nullptr or tupB == nullptr)
        return nullptr;

    int j=0;
    int jj=0;
    int flag = false;
    LinkedList *Results = new LinkedList;
    for(uint64_t i = 0; i<sizeA; i++){

        if(tupA[i].getKey() == tupB[j].getKey()){
            Results->insert(tupA[i].getPayload(), tupB[j].getPayload());

            while(tupA[i].getKey() == tupB[++j].getKey()){
                Results->insert(tupA[i].getPayload(), tupB[j].getPayload());
            }
            j = jj;
        }
        else if(tupA[i].getKey() > tupB[j].getKey()){

            while(tupA[i].getKey() > tupB[++j].getKey()){
                if (j == sizeB-1) {
                    flag = true;
                    break;
                }
            }

            if(flag) break;
            jj = j--;
            while(tupA[i].getKey() == tupB[++j].getKey()){
                Results->insert(tupA[i].getPayload(), tupB[j].getPayload());
            }
            j = jj;
        }
    }

    return Results;
}