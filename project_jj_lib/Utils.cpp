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


typedef struct {
    int n;
    int count;
    sem_t mutex;
    sem_t turnstile;
    sem_t turnstile2;
} barrier_t;

void init_barrier(barrier_t *barrier, int n)
{
    barrier->n = n;
    barrier->count = 0;
    sem_init(&barrier->mutex, 0, 1);
    sem_init(&barrier->turnstile, 0, 0);
    sem_init(&barrier->turnstile2, 0, 0);
}

void phase1_barrier(barrier_t *barrier)
{
    sem_wait(&barrier->mutex);
    if (++barrier->count == barrier->n) {
        int i;
        for (i = 0; i < barrier->n; i++) {
            sem_post(&barrier->turnstile);
        }
    }
    sem_post(&barrier->mutex);
    sem_wait(&barrier->turnstile);
}

void phase2_barrier(barrier_t *barrier)
{
    sem_wait(&barrier->mutex);
    if (--barrier->count == 0) {
        int i;
        for (i = 0; i < barrier->n; i++) {
            sem_post(&barrier->turnstile2);
        }
    }
    sem_post(&barrier->mutex);
    sem_wait(&barrier->turnstile2);
}

void wait_barrier(barrier_t *barrier)
{
    phase1_barrier(barrier);
    phase2_barrier(barrier);
}
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

//    dummy
int thread_check(int first, int last, int threads) {
    int thrds = 0;
    for (int i = first; i < last; i += ceil((float)(last-first)/(float)threads), thrds++) {}
    return thrds;
}

bool called_threads;
uint64_t byte, next_byte;
barrier_t barrier;
void check(const uint64_t *hist, Relation *R, uint64_t start, uint64_t end, uint64_t current_byte, Relation *RR)
{
    int threads = 4;

    int count = 0, first = -1, last = 0;
    for(int i = 0; i < 256; i++) {
        if (!hist[i]) continue;
        if (first == -1) first = i;
        last = i;
        count++;
    }
    last++;
    std::cout << first << " " << last << std::endl;

    if (count == 1) {
        Radixsort(R, start, end, current_byte-8, RR);
    }
    else {
        called_threads = true;

        uint64_t Psum[256] = {start};
        for (int i = 1; i < 256; i++) {
            Psum[i] = Psum[i - 1] + hist[i - 1];
        }

        byte = current_byte;
        next_byte = current_byte - 8;
        std::cout << "size of rel: " << R->numTuples << std::endl;

        int thrds = thread_check(first, last, threads);
        if (thrds < threads) {
            init_barrier(&barrier, thrds+1);
        } else {
            init_barrier(&barrier, threads+1);
        }

        sortJob *sort;
        std::vector<class sortJob*> jobs;
        int step = (int) ceil((float)(last-first)/(float)threads);
        for (int i = first; i < last; i+=step) {
            if (i+step > last) {
                sort = new sortJob(R, Psum[i], Psum[last]-1, current_byte - 8, RR);
                jobs.push_back(sort);
                std::cout << i << " ++ " << last << " " << Psum[i] << " " << Psum[last]-1<< std::endl;

            } else {
                sort = new sortJob(R, Psum[i], Psum[i+step]-1, current_byte - 8, RR);
                jobs.push_back(sort);
                std::cout << i << " ++ " << i+step << " " << Psum[i] << " " << Psum[i+step]-1<< std::endl;

            }
            job_scheduler.schedule(*sort);
        }
//        std::cout << "vector size: " << jobs.size() << std::endl;
        std::cout << "main thread wait on barrier\n";
        wait_barrier(&barrier);
        std::cout << "main thread waiting on barrier done\n";
        jobs.clear();
    }
}

void Radixsort(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte, Relation* RR)
{
    if (current_byte == 56 ) called_threads = false;
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

    std::cout << "current_byte " << current_byte  << " with flag " << called_threads << " next byte " << next_byte << std::endl;
    if (called_threads && current_byte > next_byte) return;
    if (!called_threads) {
        std::cout << "check yet to be called\n";
        check(Hist, R, start, end, current_byte, RR);
    }
    if (current_byte != next_byte+8 && current_byte > next_byte) {
        return;
    }

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
            else {
                Radixsort(RR, Psum[i - 1], Psum[i]-1, current_byte - 8, R);
            }
        }
        else{
            if(Psum[i] > Psum[i-1]) {
                OptQuicksort(RR->getTuples(), Psum[i-1], Psum[i]-1);
            }
        }
    }

    if(end > Psum[255]) {
        if ((end - Psum[255]) * sizeof(Tuple) > L1_CACHESIZE) {
            if (!current_byte) {
                OptQuicksort(RR->getTuples(), Psum[255], end);
            }
            else {
                Radixsort(RR, Psum[255], end, current_byte - 8, R);
            }
        } else {
            OptQuicksort(RR->getTuples(), Psum[255], end);
        }
    }

    R->clean(start,end);
    R->copyTuplesVal(RR, start, end);
    if(nth_byte==7) {
        delete RR;
    }

    if (current_byte == next_byte) {
        std::cout << "thread wait...\n";
        wait_barrier(&barrier);
        std::cout << "thread waiting done\n";
    }
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
