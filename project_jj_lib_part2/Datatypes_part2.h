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
class nTuple
{
    uint64_t key;
    Vector payloads;

    nTuple();

    void setKey(uint64_t key_);
    uint64_t getKey();

    void setPayload(uint64_t payload_);
    Vector getPayloads();

    void swap(nTuple *tpl);
    bool equal(nTuple x);
};


nTuple::nTuple()
{
    key = 0;
    payloads.push_back(0);
}

void nTuple::setKey(uint64_t key_) { key = key_; }
uint64_t nTuple::getKey() { return key; }

void nTuple::setPayload(uint64_t payload_) { payloads.push_back(payload_); }


void nTuple::swap(nTuple *tpl)
{
    uint64_t tmp = key;
    setKey(tpl->getKey());
    tpl->setKey(tmp);

    Vector tmpayl = payloads;
    payloads = tpl->payloads;
    tpl->payloads = tmpayl;
}
