//
// Created by wmsksf on 16/10/19.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Utils.h"
#include "LinkedList.h"
#include "Stack.h"

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

//static uint64_t MedianofThree_partition(Tuple* A, uint64_t p, uint64_t r)
//{
//    uint64_t x = A[p].getKey(), y = A[(r - p)/2 + p].getKey(), z = A[r-1].getKey();
//    uint64_t i, j = r - 1;
//    if (!p) i = p;
//    else i = p - 1;
//
//    // middle
//    if ((y > x && y < z) || (y > z && y < x) ) x = y;
//    else if ((z > x && z < y) || (z > y && z < x) ) x = z;
//
////    hoare partition for less swaps
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
//void Insertionsort(Tuple* A, uint64_t lo, uint64_t hi)
//{
//    for (uint64_t i = lo + 1; i <= hi; i++)
//    {
//        Tuple t = A[i];
//        uint64_t j = i-1;
//
//        while (j >= lo && A[j].getKey() > t.getKey())
//        {
//            A[j+1].swap(&A[j]);
//            j--;
//        }
//
//        A[j+1].swap(&t);
//    }
//}
//
//void OptQuicksort(Tuple *A, uint64_t lo, uint64_t hi)
//{
//    while (lo < hi)
//    {
////      insertion sort invoked for small size of array
//        if (hi - lo < CUTOFF)
//        {
//            Insertionsort(A, lo, hi);
//            break;
//        }
//        else
//        {
////          median of three
//            uint64_t pivot = MedianofThree_partition(A, lo, hi);
////            uint64_t pivot = partition(A, lo, hi);
//
////           tail recursion at most O(logn) space to be used
//            if (pivot - lo < hi - pivot) // recurse into smaller half
//            {
//                if (pivot)
//                    OptQuicksort(A, lo, pivot - 1);
//                else
//                    OptQuicksort(A, lo, pivot);
//
//                lo = pivot + 1;
//            }
//            else
//            {
//                OptQuicksort(A, pivot+1, hi);
//                hi = pivot - 1;
//            }
//        }
//    }
//}


static uint64_t partition(Tuple* A, uint64_t p, uint64_t r)
{
    uint64_t pivot = A[r].getKey();
    int64_t i = p - 1;

    for (uint64_t j = p; j < r; j++)
        if (A[j].getKey() <= pivot)
        {
            i++;
            A[i].swap(&A[j]);
        }

    A[i + 1].swap(&A[r]);

    return i + 1;
}

void OptQuicksort(Tuple *A, uint64_t lo, uint64_t hi)
{

    if (hi == lo) return;
    Stack stack(hi-lo+1);


    stack.push(lo);
    stack.push(hi);

    while (!stack.isEmpty())
    {
        hi = stack.pop();
        lo = stack.pop();


        uint64_t pivot = partition(A, lo, hi);


//        >>>>>>>>>>>>>>>>>>>>>>>>
//        tail recursion to be added after testing of current code is successful
//        medianofthree might be added as well
//        >>>>>>>>>>>>>>>>>>>>>>>>


//        if elements on left side of pivot, push left side to stack
        if ( pivot > 0 and pivot - 1 > lo)
        {
            stack.push(lo);
            stack.push(pivot-1);
        }

//        if elements on right side of pivot, push right side to stack
        if ( pivot + 1 < hi)
        {
            stack.push(pivot+1);
            stack.push(hi);
        }
    }
}

//IN PROCESS...
void Radixsort(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte, Relation* RR)
{
    if (current_byte == 56 && (end+1 - start) * sizeof(Tuple) < L1_CACHESIZE)
    {
//        Quicksort(R->getTuples(), start, end);
        OptQuicksort(R->getTuples(), start, end);

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

    uint64_t Psum[256] = {start};
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
        if(Psum[i-1]-1 == end) break;
        if (!(Psum[i] - Psum[i-1])) continue;

        if ((Psum[i] - Psum[i-1]) * sizeof(Tuple) > L1_CACHESIZE)
        {
            if (!current_byte) {
//                Quicksort(RR->getTuples(), Psum[i - 1], Psum[i]-1);
                OptQuicksort(RR->getTuples(), Psum[i - 1], Psum[i]-1);

            }
            else
                Radixsort(RR, Psum[i - 1], Psum[i]-1, current_byte - 8, R);
        }
        else{
            if(Psum[i] > Psum[i-1])
//                Quicksort(RR->getTuples(), Psum[i-1], Psum[i]-1);
                OptQuicksort(RR->getTuples(), Psum[i-1], Psum[i]-1);

        }
    }

    if(end > Psum[255]) {
        if ((end - Psum[255]) * sizeof(Tuple) > L1_CACHESIZE) {
            if (!current_byte)
//            Quicksort(RR->getTuples(), Psum[255], end);
                OptQuicksort(RR->getTuples(), Psum[255], end);
            else
                Radixsort(RR, Psum[255], end, current_byte - 8, R);
        } else {
//            Quicksort(RR->getTuples(), Psum[255], end);
            OptQuicksort(RR->getTuples(), Psum[255], end);
        }
    }

//    if(start>55000)
//        for(int i =start; i <= start+20; i++)
//            std::cout << R->getTuples()[i].getKey() << std::endl;

    if(nth_byte==6 or nth_byte==4 or nth_byte==2 or nth_byte==0)
        R->copyTuplesVal(RR,start,end);


    if(nth_byte==7) {
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
    char previous;
    while(c != '\n'){
        if(c == '|' or c == ',')
            columns++;
        previous = c;
        c = getc(fp);
    }
    if(previous <= '9' and previous >= '0')
        columns++;
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

    uint64_t sizeA = relA->getNumTuples();
    uint64_t sizeB = relB->getNumTuples();

    Radixsort(relA,0,sizeA-1);
    Radixsort(relB,0,sizeB-1);

   //relB->print();

    if (!relA->isSorted() || !relB->isSorted())
        return nullptr;

    Tuple* tupA = relA->getTuples();
    Tuple* tupB = relB->getTuples();
    if(tupA == nullptr or tupB == nullptr)
        return nullptr;

    int j=0;
    int jj=0;
    int flag = false;
    uint64_t counter = 0;
    LinkedList *Results = new LinkedList;
    for(uint64_t i = 0; i<sizeA; i++){

        // FOR DEBUGGING PURPOSES
        std::cout << i << std::endl;
        //REMOVE BEFORE RELEASE

        if(tupA[i].getKey() == tupB[j].getKey()){
            Results->insert(tupA[i].getPayload(), tupB[j].getPayload());
            counter++;

            while(tupA[i].getKey() == tupB[++j].getKey()){
                Results->insert(tupA[i].getPayload(), tupB[j].getPayload());
                counter++;
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
                counter++;
            }
            j = jj;
        }
    }
    std::cout << "Number of tuples after join: " << counter;
    return Results;
}