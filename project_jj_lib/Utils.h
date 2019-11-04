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

static uint64_t temp = 0;       //Used as default parameter given by reference
LinkedList * JoinSortedRelations(Relation*, Relation*, uint64_t& = temp);

/* Just for testing purposes. It doesn't return a LinkedList* with the results
 *  from the joined tables. It is only used to get the total row count from the
 *  two joined tables. */
void JoinSortedRelationsTest(Relation *, Relation *, uint64_t& );

LinkedList* SortMergeJoin(Relation*, Relation*,uint64_t& = temp, bool = false);

void clean_up(Matrix **matrix1, Matrix **matrix2,
              Relation **R1, Relation **R2, LinkedList **ResultsList);

uint64_t mcg64(void);

inline void assertions_count(uint64_t x) { std::cout << "count " << x << std::endl;}

#endif //PROJECT_JJ_UTILS_H