//
// Created by wmsksf on 28/11/19.
//

#include "Vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "MACROS.h"

Vector::Vector()
{
    vec = new uint64_t[1];
    ALLOC_CHECK(vec);

    capacity = 1;
    index = 0;
}

Vector::~Vector() {
    delete[] vec; }

uint32_t Vector::size()
{
    return index;
}

void Vector::push_back(uint64_t x)
{
    if (index == capacity)
    {
        capacity *= 2;
        uint64_t *tmp = new uint64_t[capacity];

        for (uint32_t i = 0; i < index; i++) tmp[i] = vec[i];
        delete[] vec;
        vec = tmp;
    }

    vec[index] = x;
    index++;
}

uint64_t Vector::operator[](uint64_t indx)
{
    if (indx > capacity)
    {
        std::cerr << "Index out of boundaries." << std::endl;
        exit(EXIT_FAILURE);
    }

    return vec[indx];
}

bool Vector::search(uint64_t x) {

    for(uint32_t i =0; i<index; i++)
        if(vec[i] == x) return true;

    return false;
}

uint32_t Vector::getIndex(uint64_t x)
{
    for (uint64_t i = 0; i < index; i++)
        if (vec[i] == x) return i;

    return -1;
}

void Vector::print()
{
    for (uint64_t i = 0; i < size(); i++)
        std::cout << vec[i] << " ";
}

void Vector::clear() {
    capacity = 1;
    index = 0;
    delete[] vec;

    vec = new uint64_t[1];
}
