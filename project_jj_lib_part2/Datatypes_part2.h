//
// Created by karolos on 26/11/19.
//


#ifndef PROJECT_JJ_DATATYPES_PART2_H
#define PROJECT_JJ_DATATYPES_PART2_H

#include <stdint-gcc.h>
#include "Vector.h"
#include "../project_jj_lib/DataTypes.h"
#include "../project_jj_lib_part3/Datatypes_part3.h"
#include "List.h"
# define NUMOFJOINTHREADS 4

class Predicate{
public:
    char operation;     //operation can be 'j', '>', '<', '=', ... 'j': join
    int Matrices[2];
    int RowIds[2];
    int MatricesIndex[2];
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
    long int rowsInResults;
    Predicate* Predicates;
    Vector* FilteredMatrices[4];  //max 4 matrices per query
    List* ListOfResults;
    Vector* MatricesJoined;
    Stats* stats[4] = {nullptr};

    Query();
    ~Query();
    void parse(char *inq);

    bool filtering(uint64_t &filters);
    bool prev_predicate(int cur1, int cur2, int i);
    void rearrange_predicates();
    void plan_predicates();

    Relation* FltrRel(uint64_t mat,uint64_t  index,uint64_t rel);
    void exec();
    void equality_filter(Relation *r1, Relation *r2);
    List* join(Relation*, Relation*);
    List* parallerJoin(Relation*,Relation*,int,int,int,int);
    static void parallerJoin(Relation*,Relation*,int,int,int,int,List**,long int*);
    List* prepareJoin(Relation* A, Relation* B);

    void empty_sum();
    void calc_sum();
};

int fracto_int(double number, int dec_num);

#endif //PROJECT_JJ_DATATYPES_PART2_H
