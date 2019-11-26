//
// Created by karolos on 26/11/19.
//

#include <stdint-gcc.h>

#ifndef PROJECT_JJ_DATATYPES_PART2_H
#define PROJECT_JJ_DATATYPES_PART2_H

#endif //PROJECT_JJ_DATATYPES_PART2_H

class Predicate{
public:
    char operation;     //operation can be 'j', '>', '<', '=', ... 'j': join
    int Matrices[2];
    int RowIds[2];
    uint64_t filter;
};

class Query{
public:
    int numOfMatrices;
    int numOfPredicates;
    int numOfResults;
    int* matrices;
    double* results;
    Predicate* predicates;

    Query(int numOfMatrices, int numOfPredices, int numOfResults, int* matrices, double* results, Predicate* predicates);
};
