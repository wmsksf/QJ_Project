//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_DATATYPES_H
#define PROJECT_JJ_DATATYPES_H

#include <cstdint>

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
    void print();
};

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

    uint64_t getNumTuples() const;
    void setNumTuples(uint64_t numTuples);
    void print();
    bool isSorted();
};

class Matrix
{
    long unsigned int numOfRows;
    long unsigned int numOfColumns;
    uint64_t* data;

public:
    Matrix(long unsigned int, long unsigned int);
    ~Matrix();
    bool setMatrix(const char* fileName);       // Initializes the matrix data, given an input file
    void printMatrix();
    Relation* getRelation(long unsigned int);
};

class Results
{
    Tuple Buffer[BUFFERSIZE];
    uint64_t index = 0;

    public:
    Results();

    void add(uint64_t x, uint64_t y);
    bool isFull();
    bool isEmpty();
    void print();
};

#endif //PROJECT_JJ_DATATYPES_H

