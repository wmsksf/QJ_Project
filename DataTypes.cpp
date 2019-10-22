//
// Created by wmsksf on 16/10/19.
//

#include "DataTypes.h"

Tuple::Tuple(uint64_t key, uint64_t payload) : key(key), payload(payload) {}

uint64_t Tuple::getKey() const {
    return key;
}

uint64_t Tuple::getPayload() const {
    return payload;
}

void Tuple::setKey(uint64_t key_) {
    Tuple::key = key_;
}

void Tuple::setPayload(uint64_t payload_) {
    Tuple::payload = payload_;
}

void Tuple::swap(Tuple* tuple)
{
    uint64_t tmp = this->getKey();
    this->setKey(tuple->getKey());
    tuple->setKey(tmp);

    tmp = this->getPayload();
    this->setPayload(tuple->getPayload());
    tuple->setPayload(tmp);
}

