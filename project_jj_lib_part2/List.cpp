//
// Created by wmsksf on 02/12/19.
//

#include <iostream>
#include "List.h"

List::List() { head = tail = nullptr; }

List::~List()
{
    struct Node* tmp = head;
    struct Node* next;
    while (tmp != nullptr)
    {
        next = tmp->next;
        delete tmp;
        tmp = next;
    }
}

struct Node* List::insert_node()
{
    if (head == nullptr)
    {
        head = new struct Node;
        head->next = head->prev = nullptr;
        tail = head;

        return head;
    }

    tail->next = new struct Node;
    tail->next->next = nullptr;
    tail->next->prev = tail;
    tail = tail->next;

    return tail;
}

//first insertion -> expl:
//    List l;
//    for (uint64_t i = 0; i < 10; i++) l.insert(l.insert_node(), i);
////      and rest ->
//    uint64_t j = 0;
//    for (struct Node *h = l.getHead(); h != nullptr; h = h->next, j++)
//    h->data.push_back(j);
void List::insert(struct Node *nd, uint64_t rowId)
{
    if (nd == nullptr)
    {
        std::cerr << "Null ptr of type struct Node!" << std::endl;
        exit(EXIT_FAILURE);
    }
    nd->data.push_back(rowId);
}

void List::remove_node(struct Node *nd)
{
    if (head == nullptr)
    {
        std::cout << "Empty List object!" << std::endl;
        return;
    }

    if (nd == head)
    {
        if (head->next == nullptr)
        {
            delete head;
            head = tail = nullptr;
        }
        else
        {
            head->next->prev = nullptr;
            head = head->next;
            delete nd;
        }
    }
    else if (nd == tail)
    {
        tail->prev->next = nullptr;
        tail = tail->prev;
        delete nd;
    }
    else
    {
        struct Node *prev = head;
        while (prev->next != nullptr && prev->next != nd)
            prev = prev->next;

        if (prev == nullptr)
        {
            std::cout << "Given Node not in List object!" << std::endl;
            return;
        }

        prev->next = nd->next;
        nd->next->prev = nd->prev;
        delete nd;
    }
}

struct Node* List::getHead() { return head; }

void List::print()
{
    uint64_t n = 0;
    for (struct Node *h = head; h != nullptr; h = h->next, n++)
    {
        std::cout << n << " Node" << std::endl;
        for (uint64_t j = 0; j < h->data.size(); j++)
            std::cout << h->data[j] << " ";
        std::cout << std::endl;
    }
}