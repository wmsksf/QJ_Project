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

Vector::Vector(Vector &vec)
{
    this->index = vec.index;
    this->capacity = vec.capacity;

    delete this->vec;
    this->vec = new uint64_t[this->capacity];
    for (uint64_t i = 0; i < this->index; i++) this->vec[i] = vec[i];
}

Vector::~Vector() { delete[] vec; }

uint64_t Vector::size()
{
    return index;
}

void Vector::push_back(uint64_t x)
{
    if (index == capacity)
    {
        capacity *= 2;
        uint64_t *tmp = new uint64_t[capacity];
        ALLOC_CHECK(tmp);

        for (uint64_t i = 0; i < index; i++) tmp[i] = vec[i];
        delete[] vec;
        vec = tmp;
    }

    vec[index] = x;
    index++;
}

uint64_t& Vector::operator[](uint64_t indx)
{
    if (indx > capacity)
    {
        std::cerr << "Index out of boundaries." << std::endl;
        exit(EXIT_FAILURE);
    }

    return vec[indx];
}

bool Vector::search(uint64_t x) {

    for(uint64_t i =0; i<index; i++)
        if(vec[i] == x) return true;

    return false;
}

int Vector::getIndex(uint64_t x)
{
    for (uint64_t i = 0; i < index; i++)
        if (vec[i] == x) return index;

    return -1;
}

void Vector::print()
{
    std::cout << "Vector" << std::endl;
    for (uint64_t i = 0; i < size(); i++)
        std::cout << vec[i] << " ";

    std::cout << std::endl;
}

void Vector::clear() {
    capacity = 1;
    index = 0;
    delete[] vec;
    vec = new uint64_t[1];
}
