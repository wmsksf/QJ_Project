//
// Created by wmsksf on 28/11/19.
//

#ifndef PROJECT_JJ_VECTOR_H
#define PROJECT_JJ_VECTOR_H

#include <stdint-gcc.h>

class Vector
{
    uint64_t *vec;
    uint64_t capacity, index;

public:
    Vector();
    ~Vector();

    uint64_t size();
    void push_back(uint64_t x);
    bool search(uint64_t x);
    int getIndex(uint64_t x);
    void clear();

    uint64_t operator[] (uint64_t x);

    void print();
};

#endif //PROJECT_JJ_VECTOR_H
