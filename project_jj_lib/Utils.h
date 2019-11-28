//
// Created by wmsksf on 16/10/19.
//

#ifndef PROJECT_JJ_UTILS_H
#define PROJECT_JJ_UTILS_H

#include <cstdint>
#include "DataTypes.h"

#define SIZE 1024

class Tuple;

void OptQuicksort(Tuple* A, uint64_t lo, uint64_t hi);
void Radixsort(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte = 56, Relation* RR = nullptr);

Tuple getMatrixSize(char* fileName);

static uint64_t temp = 0;       //Used as default parameter given by reference

/* Just for testing purposes. It doesn't return a LinkedList* with the results
 *  from the joined tables. It is only used to get the total row count from the
 *  two joined tables. */
void JoinSortedRelations(Relation *, Relation *, uint64_t& );

void SortMergeJoin(Relation*, Relation*,uint64_t& = temp);

inline uint64_t mcg64()
{
    static uint64_t i = 1;
    return (i = (164603309694725029ull * i) % 14738995463583502973ull);
}

void GetfromCmd(int argc, char **argv, char **file1, char **file2, long long unsigned int *rel1, long long unsigned int *rel2);

#endif //PROJECT_JJ_UTILS_H
