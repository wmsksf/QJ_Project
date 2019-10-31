//
// Created by wmsksf on 31/10/19.
//

#include <iostream>
#include "LinkedList.h"

//list for this case at least
LinkedList::LinkedList()
{
    head = nullptr;
}

LinkedList::~LinkedList()
{
    struct node* tmp = head;
    struct node* next;
    while (tmp != nullptr)
    {
        next = tmp->next;
        delete tmp;
        tmp = next;
    }
}
void LinkedList::insert(uint64_t x, uint64_t y)
{
    if (head == nullptr)
    {
        head = new struct node;
        head->Data.add(x,y);
        head->next = nullptr;

        return;
    }

    struct node* current = head;
    struct node* prev = current;

    while(current != nullptr)
    {
        if (!current->Data.isFull())
        {
            current->Data.add(x,y);
            return;
        }

        current = current->next;
        if (current != nullptr)
            prev = current;
    }

    prev->next = overflow_node(x,y);
}

struct node* LinkedList::overflow_node(uint64_t x, uint64_t y)
        {
            struct node* nd = new struct node;
            nd->Data.add(x,y);
            nd->next = nullptr;

            return nd;
        }

        void LinkedList::print()
        {
           for (struct node* tmp = head; tmp != nullptr; tmp = tmp->next)
               tmp->Data.print();
        }
