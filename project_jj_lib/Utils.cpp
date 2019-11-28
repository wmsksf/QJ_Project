//
// Created by wmsksf on 16/10/19.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <getopt.h>
#include "Utils.h"
#include "Stack.h"
#include "../project_jj_lib_part2/MACROS.h"

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
        RR->setNumTuples(R->getNumTuples());
        RR->initTuples();
    }

    uint64_t Hist[256] = {0};
    for (uint64_t i = start; i <= end; i++)
        Hist[(R->getTuples()[i].getKey() >> current_byte) & 0xff]++; // for byte 0 same as A[i] & 0xff

    uint64_t Psum[256] = {start};
    for (int i = 1; i < 256; i++)
        Psum[i] = Psum[i-1] + Hist[i-1];

//    utility in copying R to RR
    uint64_t tmp[256] = {0};
    for (uint64_t i = 0; i < 256; i++) tmp[i] = Psum[i];

    Tuple tuple;
//    because of push_back of Vector object
    RR->clean();
    for (uint64_t i = start; i <= end; i++)
    {
        tuple = R->getTuples()[i];
        uint64_t byte = (tuple.getKey() >> current_byte) & 0xff;

        for (uint64_t j = 0; j < tuple.getPayloads().size(); j++)
            RR->setTupleVal(tmp[byte], tuple.getKey(), tuple.getPayloads()[j]);
        tmp[byte]++;
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
    R->copyTuplesVal(RR, start, end);
//    std::cout <<"END OF RADIX R\n"; R->print();

    if(nth_byte==7) {
        delete RR;
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
    tmp.setKey(columns);
    tmp.setPayload(lines);

    return tmp;
}

void SortMergeJoin(Relation* relA, Relation* relB, uint64_t& count) {

    if(relA == nullptr or relB == nullptr){
        std::cout << "Relations can't be NULL!" << std::endl;
        exit(EXIT_FAILURE);
    }

    uint64_t sizeA = relA->getNumTuples();
    uint64_t sizeB = relB->getNumTuples();

    Radixsort(relA,0,sizeA-1);
    Radixsort(relB,0,sizeB-1);

    relA->print(); std::cout << "\n\n\n"; relB->print();

    JoinSortedRelations(relA,relB,count);

    fprintf(stdout, "\n\nAfter radix sort or relations -> error with RowIds\n in file %s on line %d\n", __FILE__, __LINE__);
    std::cout << std::endl;

    std::cout << "Number of tuples after join: " << count << std::endl;
}

void JoinSortedRelations(Relation *relA, Relation *relB, uint64_t& count) {

    if (!relA->isSorted() || !relB->isSorted())
        return;

    Tuple* tupA = relA->getTuples();
    Tuple* tupB = relB->getTuples();

    if(tupA == nullptr or tupB == nullptr)
        return;

    uint64_t sizeA = relA->getNumTuples();
    uint64_t sizeB = relB->getNumTuples();
    uint64_t j=0;
    uint64_t jj=0;
    bool flag = false;
    uint64_t counter = 0;

    for(uint64_t i = 0; i<sizeA; i++){

        if(tupA[i].getKey() == tupB[j].getKey()){
            counter++;

            while(tupA[i].getKey() == tupB[++j].getKey()){
                counter++;
                if(j == sizeB-1) break;
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

            if (j == sizeB-1) {
                break;
            }
            if(flag) break;
            jj = j--;
            while(tupA[i].getKey() == tupB[++j].getKey()){
                counter++;
            }
            j = jj;
        }
    }
    count = counter;
}

static char* copy_chars(char *givenChars)
{
    char *chars = (char*) malloc((strlen(givenChars) + 1) * sizeof(char));
    if (chars == NULL)
    {
        std::cerr << "Memory allocation for char* failed." << std::endl;
        exit(EXIT_FAILURE);
    }

    memcpy(chars, givenChars, (strlen(givenChars) + 1) * sizeof(char));
    return chars;
}

void GetfromCmd(int argc, char **argv, char **file1, char **file2, long long unsigned int *rel1, long long unsigned  int *rel2)
{
    if (argc < 9) USAGE(argv[0]);

    int opt;
    struct option long_options[] =
            {
                    {"f1", required_argument, nullptr, 'f'},
                    {"rel1", required_argument, nullptr, 'g'},
                    {"f2", required_argument, nullptr, 'k'},
                    {"rel2", required_argument, nullptr, 'l'},
                    {nullptr, 0, nullptr, 0}
            };

    while ((opt = getopt_long_only(argc, argv, "", long_options, nullptr)) != -1)
    {
        switch (opt)
        {
            case 'f':
                *file1 = copy_chars(optarg);
                break;
            case 'g':
                *rel1 = atoll(optarg);
                break;
            case 'k':
                *file2 = copy_chars(optarg);
                break;
            case 'l':
                *rel2 = atoll(optarg);
                break;
            default:
                USAGE(argv[0]);
        }
    }
}
