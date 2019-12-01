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

        uint64_t operator[] (uint64_t x);
        Vector& operator= (Vector &vec);
};

#endif //PROJECT_JJ_VECTOR_H
