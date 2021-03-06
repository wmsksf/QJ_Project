//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_DATATYPES_H
#define PROJECT_JJ_DATATYPES_H

#include <cstdint>
#include "../project_jj_lib_part2/Vector.h"
#include "../project_jj_lib_part2/List.h"
#include "../project_jj_lib_part3/Datatypes_part3.h"

#define L1_CACHESIZE 32*1024


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
    void clean(uint64_t start, uint64_t end);
    bool isSorted();

    void filter(Vector*);
};
//    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
class Matrix
{
public:
    long unsigned int numOfRows = 0;
    long unsigned int numOfColumns = 0;
    uint64_t* data = nullptr;
    Stats* stats;

//    Matrix(long unsigned int, long unsigned int);
    Matrix();
    ~Matrix();
//    Initializes the matrix data, given an input file
    bool setMatrix(char* fileName);
    void calcStats();

    Relation* getRelation(long unsigned int);
    Relation* getRelation(List* list,int index, long int numOfRows, int columnNumber);
    Relation* getRelationKeys(List* list,int index, long int numOfRows_, int columnNumber);

    void printMatrix();
    uint64_t * getData();
};
//    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#endif //PROJECT_JJ_DATATYPES_H
