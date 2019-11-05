//
// Created by wmsksf on 02/11/19.
//

#include "Stack.h"

#include <iostream>
#include <cstdlib>

Stack::Stack(uint64_t size)
{
    stack = new uint64_t[size];
    capacity = size;
    top = 0;
}

Stack::~Stack() { delete[] stack; }

void Stack::push(uint64_t x)
{
    if (isFull())
    {
        std::cout << "Full stack object." << std::endl;
        exit(EXIT_FAILURE);
    }

    stack[top++] = x;
}

uint64_t Stack::pop()
{
    if (isEmpty())
    {
        std::cout << "Empty stack object." << std::endl;
        exit(EXIT_FAILURE);
    }

    return stack[--top];
}

uint64_t Stack::peek()
{
    if (!isEmpty())
        return stack[top-1];
    else
    {
        std::cout << "Empty stack object. Nothing to peek!" << std::endl;
        exit(EXIT_FAILURE);

    }
}

uint64_t Stack::size() { return top ; }
bool Stack::isEmpty() { return size() == 0; }
bool Stack::isFull() {  return size() == capacity; }
