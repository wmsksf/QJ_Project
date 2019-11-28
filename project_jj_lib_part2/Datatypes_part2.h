//
// Created by karolos on 26/11/19.
//

#include <stdint-gcc.h>
#include "Vector.h"

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
    int exec();
};

//---------------------> after setting follows update of class Tuple
//class Tuple
//{
//    uint64_t key;
//    Vector payloads;
//
//    Tuple();
//
//    void setKey(uint64_t key_);
//    uint64_t getKey();
//
//    void setPayload(uint64_t payload_);
//    Vector& getPayloads();
//
//    void swap(Tuple *tpl);
//};
//
//
//Tuple::Tuple() { key = 0; }
//
//void Tuple::setKey(uint64_t key_) { key = key_; }
//uint64_t Tuple::getKey() { return key; }
//
//void Tuple::setPayload(uint64_t payload_) { payloads.push_back(payload_); }
//Vector& Tuple::getPayloads() { return payloads; }
//
//void Tuple::swap(Tuple *tpl)
//{
//    uint64_t tmp = key;
//    setKey(tpl->getKey());
//    tpl->setKey(tmp);
//
//    Vector tmpayl;
//    tmpayl = payloads;
//    payloads = tpl->getPayloads();
//    tpl->getPayloads() = tmpayl;
//}
