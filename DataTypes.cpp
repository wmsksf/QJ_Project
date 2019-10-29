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
