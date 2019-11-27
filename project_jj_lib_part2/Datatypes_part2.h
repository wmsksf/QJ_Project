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

    Predicate();
};

class Query{
public:
    int NumOfMatrices;
    int NumOfPredicates;
    int NumOfResults;
    int* Matrices;
    double* Results;
    Predicate* Predicates;

    Query();
    void parse(char *inq);
};
