//
// Created by wmsksf on 31/10/19.
//

#ifndef PROJECT_JJ_LINKEDLIST_H
#define PROJECT_JJ_LINKEDLIST_H

#include "DataTypes.h"

// USE: storing results, <rowIdR, rowIdS>, given class Results in SortMergeJoin()
struct node
{
    Results Data;
    struct node* next;
};

class LinkedList
{
    struct node* head;
    uint64_t Datasize = 0;

    public:
    LinkedList(uint64_t Datasize);
    ~LinkedList();

    void insert(uint64_t x, uint64_t y);
    bool empty();
    void print();

    protected:
    struct node* overflow_node(uint64_t x, uint64_t y);
};

#endif //PROJECT_JJ_LINKEDLIST_H
