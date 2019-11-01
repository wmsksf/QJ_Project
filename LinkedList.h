//
// Created by wmsksf on 31/10/19.
//

#ifndef PROJECT_JJ_LINKEDLIST_H
#define PROJECT_JJ_LINKEDLIST_H

#include "DataTypes.h"

struct node
{
    Results Data;
    struct node* next;
};

class LinkedList
{
    struct node* head;

    public:
    LinkedList();
    ~LinkedList();

    void insert(uint64_t x, uint64_t y);
    bool empty();
    void print();

    protected:
    struct node* overflow_node(uint64_t x, uint64_t y);
};

#endif //PROJECT_JJ_LINKEDLIST_H
