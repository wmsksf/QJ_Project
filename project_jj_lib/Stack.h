//
// Created by wmsksf on 02/11/19.
//

#ifndef PROJECT_JJ_STACK_H
#define PROJECT_JJ_STACK_H

#include <cstdint>

// Use: iterative Quicksort
class Stack
{
    uint64_t *stack, capacity;
    int64_t top;

public:
    Stack(uint64_t size);
    ~Stack();

    void push(uint64_t x);
    uint64_t pop();
    uint64_t peek();

    uint64_t size();
    bool isEmpty();
    bool isFull();
};


#endif //PROJECT_JJ_STACK_H
