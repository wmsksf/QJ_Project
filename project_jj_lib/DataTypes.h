//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_DATATYPES_H
#define PROJECT_JJ_DATATYPES_H

#include <cstdint>
#include "../project_jj_lib_part2/Vector.h"

#define L1_CACHESIZE 64*1024

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
    void clean();
    bool isSorted();
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
};

#endif //PROJECT_JJ_DATATYPES_H
