//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_UTILS_H
#define PROJECT_JJ_UTILS_H

#include <cstdint>
#include "DataTypes.h"
#include "../project_jj_lib_part3/Barrier.h"

#define SIZE 1024

extern bool called_threads;

class Tuple;

void OptQuicksort(Tuple* A, uint64_t lo, uint64_t hi);
void Radixsort(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte = 56, Relation* RR = nullptr);
void parallerRadixsort(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte = 56, Relation* RR = nullptr);

Tuple getMatrixSize(char* fileName);

inline uint64_t mcg64()
{
    static uint64_t i = 1;
    return (i = (164603309694725029ull * i) % 14738995463583502973ull);
}

#endif //PROJECT_JJ_UTILS_H
