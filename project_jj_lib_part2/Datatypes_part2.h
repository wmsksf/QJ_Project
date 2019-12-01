//
// Created by karolos on 26/11/19.
//

#include <stdint-gcc.h>
#include "Vector.h"
#include "../project_jj_lib/DataTypes.h"


#ifndef PROJECT_JJ_DATATYPES_PART2_H
#define PROJECT_JJ_DATATYPES_PART2_H

class Predicate{
public:
    char operation;     //operation can be 'j', '>', '<', '=', ... 'j': join
    int Matrices[2];
    int RowIds[2];
    uint64_t filter;

    Predicate();
    char getOperation();
};

class Query{
public:
    int NumOfMatrices;
    int NumOfPredicates;
    int NumOfResults;
    int* Matrices;
    double* Results;
    Predicate* Predicates;
    uint64_t* MatricesData[4];  //max 4 matrices per query
    Vector* FilteredMatrices[4];

    Query();
    void parse(char *inq);
    int exec();
    static Vector* applyFilter(Relation*, char, uint64_t);

};

#endif //PROJECT_JJ_DATATYPES_PART2_H
