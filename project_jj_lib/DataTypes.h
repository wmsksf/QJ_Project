//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_DATATYPES_H
#define PROJECT_JJ_DATATYPES_H

#include <cstdint>
#include "../project_jj_lib_part2/Vector.h"

#define L1_CACHESIZE 64*1024
#define BUFFERSIZE (1024*1024)/sizeof(Tuple)

class Tuple
{
    uint64_t key, payload;

public:
    Tuple();

    uint64_t getKey() const;
    uint64_t getPayload() const;

    void setKey(uint64_t key);
    void setPayload(uint64_t payload);

    void swap(Tuple* tuple);
    bool equal(Tuple x);
    void print();
};
//    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Relation
{
    Tuple* tuples;
    uint64_t numTuples;

public:
    Relation();
    ~Relation();

    Tuple *getTuples() const;
    void initTuples();
    void initTuplesVal(Relation* R);
    void setTupleVal(long unsigned int, uint64_t, uint64_t);
    void copyTuplesVal(Relation* R,uint64_t, uint64_t);

    uint64_t getNumTuples() const;
    void setNumTuples(uint64_t numTuples);
    void print();
    bool isSorted();
    void filter(Vector*);
};
//    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Matrix
{
    long unsigned int numOfRows = 0;
    long unsigned int numOfColumns = 0;
    uint64_t* data = nullptr;

public:
//    Matrix(long unsigned int, long unsigned int);
    Matrix();
    ~Matrix();
    bool setMatrix(char* fileName);       // Initializes the matrix data, given an input file
    void printMatrix();
    Relation* getRelation(long unsigned int);
    uint64_t * getData();
    long unsigned int getNumOfRows();
    long unsigned int getNumOfColumns();
};
//    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Results
{
    Tuple *Buffer;
    uint64_t index = 0, Buffersize;

    public:
    Results();
    ~Results();

    void setBuffersize(uint64_t buffersize);
    void initBuffer();

    void add(uint64_t x, uint64_t y);
    bool isFull();
    bool isEmpty();
    void print();
};

#endif //PROJECT_JJ_DATATYPES_H
