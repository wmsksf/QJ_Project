//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_UTILS_H
#define PROJECT_JJ_UTILS_H

#include <cstdint>
#include "DataTypes.h"

#define SIZE 1024
class Tuple;



void Quicksort(uint64_t* A, uint64_t lo, uint64_t hi);

Tuple getMatrixSize(const char* fileName);

#endif //PROJECT_JJ_UTILS_H
