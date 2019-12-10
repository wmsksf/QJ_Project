//
// Created by wmsksf on 04/11/19.
//

#define CATCH_CONFIG_MAIN

#define SIZE 10000

#include "catch.hpp"
#include "../project_jj_lib_part2/Vector.h"
#include "../project_jj_lib_part2/List.h"

TEST_CASE("Vector object", "[VEC_OBJ]")
{
    Vector vec;
    REQUIRE(vec.size() == 0);
}

TEST_CASE("Vector push_back(), operator[] methods", "[VEC_PUSH_BACK]")
{
    Vector vec;

    for (int i = 0; i < SIZE; i++)
    {
        vec.push_back(i);
        REQUIRE(vec[i] == i);
    }
    REQUIRE(vec.size() == SIZE);
}

TEST_CASE("Vector search(), getIndex() methods", "[VEC_SEARCH]")
{
    Vector vec;
    vec.push_back(1);

    REQUIRE(vec.search(1) == true);
    REQUIRE(vec.getIndex(1) == 0);

    REQUIRE(vec.search(12345) == false);
    REQUIRE(vec.getIndex(12345) == -1);

    Vector v;
    for (int i = 0; i < SIZE; i++)
    {
        v.push_back(i);
        REQUIRE(v[i] == i);
        REQUIRE(v.search(i) == true);
        REQUIRE(v.getIndex(i) == i);
    }
}

TEST_CASE("List object", "[LIST_OBJ]")
{
    List list;
    REQUIRE(list.head == nullptr);

    struct Node *l = list.insert_node();

    if (list.head == nullptr)
        REQUIRE(l == list.head);
    else
        REQUIRE(l == list.tail);
}

TEST_CASE("List insert() method", "[LIST_INSRT]")
{
    List list;
    struct Node *n;
    for (int i = 0; i < SIZE; i++)
    {
        n = list.insert_node();
        REQUIRE(list.tail == n);

        list.insert(n, i);
        REQUIRE(n->data[0] == i);
    }
}

