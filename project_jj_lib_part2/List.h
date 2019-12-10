//
// Created by wmsksf on 02/12/19.
//

#ifndef PROJECT_JJ_LIST_H
#define PROJECT_JJ_LIST_H

#include "Vector.h"

struct Node
{
    Vector data;
    struct Node *next, *prev;
};

class List
{
public:
    struct Node *head, *tail;
    List();
    ~List();

    struct Node* insert_node();
    void insert(struct Node* nd, uint64_t rowId);

    void remove_node(struct Node *nd);

    void print();
};

#endif //PROJECT_JJ_LIST_H
