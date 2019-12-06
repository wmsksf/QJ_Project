//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_DATATYPES_H
#define PROJECT_JJ_DATATYPES_H

#include <cstdint>
#include "../project_jj_lib_part2/Vector.h"
#include "../project_jj_lib_part2/List.h"

#define L1_CACHESIZE 64*1024
#define BUFFERSIZE (1024*1024)/sizeof(Tuple)

class Tuple
{
public:
    uint64_t key;
    Vector payloads;

    Tuple();

    void setPayload(uint64_t payload_);
    Vector& getPayloads();

    void swap(Tuple *tpl);

    void print();
};
//    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Relation
{
    Tuple* tuples;

public:
    uint64_t numTuples;

    Relation();
    ~Relation();

    Tuple *getTuples() const;
    void initTuples();

    void setTupleVal(long unsigned int, uint64_t, Vector&);
//    use in method Matrix::getRelation
    void setTupleVal(long unsigned int, uint64_t, uint64_t);
    void copyTuplesVal(Relation* R,uint64_t, uint64_t);

    void print();
    void clean();
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
//    Initializes the matrix data, given an input file
    bool setMatrix(char* fileName);

    Relation* getRelation(long unsigned int);
    Relation* getRelation(List* list,int index, long int numOfRows, int columnNumber);

    void printMatrix();
    uint64_t * getData();
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
    Tuple* getBuffer();
    uint64_t getIndex();

    void add(uint64_t x, uint64_t y);
    bool isFull();
    bool isEmpty();
    void print();
};

#endif //PROJECT_JJ_DATATYPES_H
