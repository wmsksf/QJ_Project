//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_UTILS_H
#define PROJECT_JJ_UTILS_H

#include <cstdint>
#include "DataTypes.h"
#include "LinkedList.h"

#define SIZE 1024
#define CUTOFF 1000

class Tuple;

//void Quicksort(Tuple* A, uint64_t lo, uint64_t hi);
void OptQuicksort(Tuple* A, uint64_t lo, uint64_t hi);

void Radixsort(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte = 56, Relation* RR = nullptr);

Tuple getMatrixSize(const char* fileName);

LinkedList* SortMergeJoin(Relation*, Relation*);

void clean_up(Matrix **matrix1, Matrix **matrix2,
              Relation **R1, Relation **R2, LinkedList **ResultsList);

uint64_t mcg64(void);

inline void assertions_count(uint64_t x) { std::cout << "count " << x << std::endl;}

#endif //PROJECT_JJ_UTILS_H