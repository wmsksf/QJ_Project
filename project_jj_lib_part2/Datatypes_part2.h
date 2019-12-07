//
// Created by karolos on 26/11/19.
//

#include <stdint-gcc.h>
#include "Vector.h"
#include "../project_jj_lib/DataTypes.h"
#include "../project_jj_lib/LinkedList.h"
#include "List.h"


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
    long int rowsInResults;
    Predicate* Predicates;
    Vector* FilteredMatrices[4];  //max 4 matrices per query
    List* ListOfResults;
    Vector* MatricesJoined;

    Query();
    ~Query();
    void parse(char *inq);

    List* join(Relation*, Relation*);
    List* EQjoin(Relation *relA, Relation *relB);
    //void expandResultsList(LinkedList* latestJoin, uint64_t matrixA, uint64_t matrixB);
    bool filtering(uint64_t &filters);
    void exec();

    void empty_sum();
    void calc_sum();

    Relation* FltrRel(uint64_t mat, uint64_t rel);
};

#endif //PROJECT_JJ_DATATYPES_PART2_H
