//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_DATATYPES_H
#define PROJECT_JJ_DATATYPES_H

#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Utils.h"


class Tuple
{
    uint64_t key, payload;

public:
    Tuple(uint64_t key, uint64_t payload);

    uint64_t getKey() const;
    uint64_t getPayload() const;

};

class Relation
{
    Tuple *tuples;
    uint64_t num_tuples;

public:

};

class Matrix
{
    int numOfRows;
    int numOfColumns;
    uint64_t* data;

public:
    Matrix(long unsigned int, long unsigned int);
    ~Matrix();
    bool setMatrix(const char* fileName);       // Initializes the matrix data, given an input file
    void printMatrix();
};

#endif //PROJECT_JJ_DATATYPES_H
