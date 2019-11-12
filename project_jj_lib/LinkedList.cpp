//
// Created by wmsksf on 31/10/19.
//

#include <iostream>
#include "LinkedList.h"

//list for this case at least
LinkedList::LinkedList(uint64_t Datasize)
{
    head = tail = nullptr;
    this->Datasize = Datasize;
}

LinkedList::~LinkedList()
{
    tail = nullptr;

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
//        init buffer
        head->Data.setBuffersize(Datasize);
        head->Data.initBuffer();

        head->Data.add(x,y);
        head->next = nullptr;
        tail = head;

        return;
    }

    struct node *last = tail;
    if (!last->Data.isFull())
    {
        last->next = overflow_node(x,y);
        tail = last->next;
    }
    else
        last->Data.add(x,y);
}

bool LinkedList::empty()
{
    if (head == nullptr || head->Data.isEmpty())
        return true;
}

struct node* LinkedList::overflow_node(uint64_t x, uint64_t y)
        {
            struct node* nd = new struct node;
//            init buffer
            nd->Data.setBuffersize(Datasize);
            nd->Data.initBuffer();

            nd->Data.add(x,y);
            nd->next = nullptr;

            return nd;
        }

        void LinkedList::print()
        {
           for (struct node* tmp = head; tmp != nullptr; tmp = tmp->next)
               tmp->Data.print();
        }
