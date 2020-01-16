//
// Created by karolos on 5/1/20.
//

#ifndef PROJECT_JJ_DATATYPES_PART3_H
#define PROJECT_JJ_DATATYPES_PART3_H

#include <pthread.h>
#include <cstdint>
//#include "../project_jj_lib_part2/Datatypes_part2.h"

#define MAX_DISTINCT_VALUES 49979693

class Stats{
public:
    uint64_t I, U, f, d; // min, max, # of rows, # of unique values

    void copy(Stats*);
};

class MatrixPrediction{
public:
    uint64_t matrixIndex;
    uint64_t numOfColumns;
    Stats* stats = nullptr;

    void copy(MatrixPrediction* p2);
    ~MatrixPrediction();

};

class Prediction{
public:
    uint64_t numOfMatrices;
    MatrixPrediction* matrices = nullptr;
    uint64_t numOfPredicates = 0;
    int* predicateOrder = nullptr;
    int getCost();

    bool predicateInPrediction(int predicate);
    Prediction* JoinPrediction(Prediction* predB,uint64_t indexA,uint64_t colA,uint64_t indexB,uint64_t colB, int predicateIndex);
    Prediction* EqualityFilterPrediction(uint64_t indexA,uint64_t colA,uint64_t indexB,uint64_t colB, int predicateIndex);
    ~Prediction();
    bool matrixInPrediction(int index);
    void print();
};

#endif //PROJECT_JJ_DATATYPES_PART3_H
