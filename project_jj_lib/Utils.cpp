//
// Created by wmsksf on 16/10/19.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <getopt.h>
#include <semaphore.h>
#include <math.h>
#include "Utils.h"
#include "Stack.h"
#include "../project_jj_lib_part2/MACROS.h"
#include "../project_jj_lib_part3/JobScheduler.h"

//----------------------------------------------------------------------------------------------------------------------

static uint64_t partition(Tuple* A, uint64_t p, uint64_t r)
{
    uint64_t pivot = A[r].key;
    int64_t i = p - 1;

    for (uint64_t j = p; j < r; j++)
        if (A[j].key <= pivot)
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

void Radixsort(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte, Relation* RR)
{
    if (current_byte == 56 && (end+1 - start) * sizeof(Tuple) < L1_CACHESIZE)
    {
        OptQuicksort(R->getTuples(), start, end);
        return;
    }

    if (RR == nullptr)
    {
        RR = new Relation;
        RR->numTuples = R->numTuples;
        RR->initTuples();
    }

    uint64_t Hist[256] = {0};
    for (uint64_t i = start; i <= end; i++)
        Hist[(R->getTuples()[i].key >> current_byte) & 0xff]++; // for byte 0 same as A[i] & 0xff

    uint64_t Psum[256] = {start};
    for (int i = 1; i < 256; i++)
        Psum[i] = Psum[i-1] + Hist[i-1];

//    utility in copying R to RR
    uint64_t tmp[256] = {0};
    for (uint64_t i = 0; i < 256; i++) tmp[i] = Psum[i];

    RR->clean(start,end);
    for (uint64_t i = start; i <= end; i++)
    {
        uint64_t byte = (R->getTuples()[i].key >> current_byte) & 0xff;
        RR->setTupleVal(tmp[byte]++, R->getTuples()[i].key, R->getTuples()[i].payloads);
    }

//    switch R, RR after byte checked
    uint64_t nth_byte = current_byte/8;
    for (uint64_t i = 1; i < 256; i++)
    {
        if(Psum[i-1]-1 == end) break;
        if (!(Psum[i] - Psum[i-1])) continue;

        if ((Psum[i] - Psum[i-1]) * sizeof(Tuple) > L1_CACHESIZE)
        {
            if (!current_byte) {
                OptQuicksort(RR->getTuples(), Psum[i - 1], Psum[i]-1);
            }
            else
                Radixsort(RR, Psum[i - 1], Psum[i]-1, current_byte - 8, R);
        }
        else{
            if(Psum[i] > Psum[i-1])
                OptQuicksort(RR->getTuples(), Psum[i-1], Psum[i]-1);
        }
    }

    if(end > Psum[255]) {
        if ((end - Psum[255]) * sizeof(Tuple) > L1_CACHESIZE) {
            if (!current_byte)
                OptQuicksort(RR->getTuples(), Psum[255], end);
            else
                Radixsort(RR, Psum[255], end, current_byte - 8, R);
        } else {
            OptQuicksort(RR->getTuples(), Psum[255], end);
        }
    }
    R->clean(start,end);
    R->copyTuplesVal(RR, start, end);

    if(nth_byte==7) {
        delete RR;
    }
}

void sort(Relation *rel)
{
    bucks *b = checkBuckets(rel,0, rel->numTuples-1);
    if (b == nullptr) {
        Radixsort(rel, 0, rel->numTuples-1);
        return;
    }
    splitBuckets(rel,0,rel->numTuples-1, b);
//    rel->print();
}

bucks* checkBuckets(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte)
{
    while (current_byte != 0)
    {
        uint64_t Hist[256] = {0};
        int count = 0, first = -1, last = 0;

        for (uint64_t i = start; i <= end; i++) {
            Hist[(R->getTuples()[i].key >> current_byte) & 0xff]++;
        }
        for(int i = 0; i < 256; i++) {
            if (!Hist[i]) continue;
            if (first == -1) first = i;
            last = i;
            count++;
        }
        last++;

        if (count > 1) {
            bucks *buck = new bucks;
            buck->first = first;
            buck->last = last;
            buck->byte = current_byte;

            std::cout << buck->first << " - " << buck->last << " : " << buck->byte << std::endl;
            return buck;
        }
        current_byte -= 8;
    }
    return nullptr;
}

//    dummy
int thread_check(int first, int last, int threads) {
    int thrds = 0;
    for (int i = first; i < last; i += ceil((float)(last-first)/(float)threads), thrds++) {}
    return thrds;
}

barrier_t barrier;
int byte;
void splitBuckets(Relation *R, uint64_t start, uint64_t end, bucks *buck)
{
    int threads = 4;

    uint64_t Hist[256] = {0};
    for (uint64_t i = start; i <= end; i++) {
        Hist[(R->getTuples()[i].key >> buck->byte) & 0xff]++;
    }
    uint64_t Psum[256] = {start};
    for (int i = 1; i < 256; i++) {
        Psum[i] = Psum[i - 1] + Hist[i - 1];
    }

    byte = buck->byte;
    int thrds = thread_check(buck->first, buck->last, threads);
    if (thrds < threads) {
        init_barrier(&barrier, thrds+1);
    } else {
        init_barrier(&barrier, threads+1);
    }

    sortJob *sort;
    std::vector<class sortJob*> jobs;
    std::cout << R->numTuples << std::endl;
    int step = (int) ceil((float)(buck->last-buck->first)/(float)threads);
    for (int i = buck->first; i < buck->last; i+=step) {
        if (i+step > buck->last) {
            sort = new sortJob(R, Psum[i], Psum[buck->last]-1, buck->byte, nullptr);
            jobs.push_back(sort);
            std::cout << i << " ++ " << buck->last << " : " << Psum[i] << " " << Psum[buck->last]-1<< std::endl;

        } else {
            sort = new sortJob(R, Psum[i], Psum[i+step]-1, buck->byte, nullptr);
            jobs.push_back(sort);
            std::cout << i << " ++ " << i+step << " : " << Psum[i] << " " << Psum[i+step]-1<< std::endl;

        }
        job_scheduler.schedule(*sort);
    }

    std::cout << "main thread wait on barrier\n";
    wait_barrier(&barrier);
    std::cout << "main thread waiting on barrier done\n";
    jobs.clear();
}

void radix(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte, Relation* RR)
{

    s_cout{} << "current byte " << current_byte  << " and byte " << byte << std::endl;
    if ((end+1 - start) * sizeof(Tuple) < L1_CACHESIZE)
    {
        OptQuicksort(R->getTuples(), start, end);

        if (current_byte == byte) {
            std::cout << "thread wait...\n";
            wait_barrier(&barrier);
            std::cout << "thread waiting done\n";
        }
        return;
    }
    if (current_byte == byte) {
        radix(R, start, end, current_byte-8);

        s_cout{} << "thread wait...\n";
        wait_barrier(&barrier);
        s_cout{} << "thread waiting done\n";

        return;
    }

    if (RR == nullptr)
    {
        RR = new Relation;
        RR->numTuples = R->numTuples;
        RR->initTuples();
    }

    uint64_t Hist[256] = {0};
    for (uint64_t i = start; i <= end; i++)
        Hist[(R->getTuples()[i].key >> current_byte) & 0xff]++; // for byte 0 same as A[i] & 0xff

    uint64_t Psum[256] = {start};
    for (int i = 1; i < 256; i++)
        Psum[i] = Psum[i-1] + Hist[i-1];

//    utility in copying R to RR
    uint64_t tmp[256] = {0};
    for (uint64_t i = 0; i < 256; i++) tmp[i] = Psum[i];

    RR->clean(start,end);
    for (uint64_t i = start; i <= end; i++)
    {
        uint64_t byte = (R->getTuples()[i].key >> current_byte) & 0xff;
        RR->setTupleVal(tmp[byte]++, R->getTuples()[i].key, R->getTuples()[i].payloads);
    }

//    switch R, RR after byte checked
    uint64_t nth_byte = current_byte/8;

    for (uint64_t i = 1; i < 256; i++)
    {
        if(Psum[i-1]-1 == end) break;
        if (!(Psum[i] - Psum[i-1])) continue;

        if ((Psum[i] - Psum[i-1]) * sizeof(Tuple) > L1_CACHESIZE)
        {
            if (!current_byte) {
                OptQuicksort(RR->getTuples(), Psum[i - 1], Psum[i]-1);
            }
            else
                radix(RR, Psum[i - 1], Psum[i]-1, current_byte - 8, R);
        }
        else{
            if(Psum[i] > Psum[i-1])
                OptQuicksort(RR->getTuples(), Psum[i-1], Psum[i]-1);
        }
    }

    if(end > Psum[255]) {
        if ((end - Psum[255]) * sizeof(Tuple) > L1_CACHESIZE) {
            if (!current_byte)
                OptQuicksort(RR->getTuples(), Psum[255], end);
            else
                radix(RR, Psum[255], end, current_byte - 8, R);
        } else {
            OptQuicksort(RR->getTuples(), Psum[255], end);
        }
    }
    R->clean(start,end);
    R->copyTuplesVal(RR, start, end);

    if(nth_byte==7) {
        delete RR;
    }
    s_cout{} <<"done\n";
}

Tuple getMatrixSize(char *fileName) {

    //Opening the input file
    FILE* fp;
    char buffer[SIZE + 1], lastchar = '\n';
    size_t bytes;
    int lines = 0;
    int columns = 0;
    fp = fopen(fileName,"r");     //Opening the file

    if (fp == NULL) {   //checking for error with fopen
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
    tmp.key = columns;
    tmp.setPayload(lines);

    return tmp;
}
