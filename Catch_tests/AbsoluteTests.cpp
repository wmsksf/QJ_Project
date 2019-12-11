//
// Created by wmsksf on 04/11/19.
//

#define CATCH_CONFIG_MAIN

#define SIZE 10000

#include "catch.hpp"
#include "../project_jj_lib_part2/Vector.h"
#include "../project_jj_lib_part2/List.h"
#include "../project_jj_lib_part2/Datatypes_part2.h"
#include "../project_jj_lib_part2/MACROS.h"

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

TEST_CASE("Query object", "[Q_OBJ]")
{
    Query q;
    char *input;
    input = new char[40];
    memcpy(input, "0 1 2|0.1=1.0&1.2=2.1&1.2=0.0|1.2 2.2", 40);

    q.parse(input);
    REQUIRE(q.NumOfMatrices == 3);
    REQUIRE(q.NumOfPredicates == 3);
    REQUIRE(q.NumOfResults == 2);

    for (int i = 0; i < q.NumOfMatrices; i++)
        REQUIRE(q.Matrices[i] == i);
        for (int j = 0; j < q.NumOfPredicates; j++)
           switch(j)
            {
                case 0:
                    REQUIRE(q.Predicates[j].Matrices[0] == 0);
                    REQUIRE(q.Predicates[j].Matrices[1] == 1);
                    REQUIRE(q.Predicates[j].MatricesIndex[0] == 0);
                    REQUIRE(q.Predicates[j].MatricesIndex[1] == 1);
                    REQUIRE(q.Predicates[j].RowIds[0] == 1);
                    REQUIRE(q.Predicates[j].RowIds[1] == 0);
                    break;
                case 1:
                    REQUIRE(q.Predicates[j].Matrices[0] == 1);
                    REQUIRE(q.Predicates[j].Matrices[1] == 2);
                    REQUIRE(q.Predicates[j].MatricesIndex[0] == 1);
                    REQUIRE(q.Predicates[j].MatricesIndex[1] == 2);
                    REQUIRE(q.Predicates[j].RowIds[0] == 2);
                    REQUIRE(q.Predicates[j].RowIds[1] == 1);
                    break;
                case 2:
                    REQUIRE(q.Predicates[j].Matrices[0] == 1);
                    REQUIRE(q.Predicates[j].Matrices[1] == 0);
                    REQUIRE(q.Predicates[j].MatricesIndex[0] == 1);
                    REQUIRE(q.Predicates[j].MatricesIndex[1] == 0);
                    REQUIRE(q.Predicates[j].RowIds[0] == 2);
                    REQUIRE(q.Predicates[j].RowIds[1] == 0);
                break;
            }

    for (int j = 0; j < q.NumOfResults; j++)
    {
        double frack, intprt;
        int x, y;
        frack = modf(q.Results[j], &intprt);
        x = (int) intprt;
        y = fracto_int(frack, 1);

        switch (j)
        {
            case 0:
                REQUIRE(q.Matrices[x] == 1);
                REQUIRE(y == 2);
                break;
            case 1:
                REQUIRE(q.Matrices[x] == 2);
                REQUIRE(y == 2);
                break;
        }
    }
}

TEST_CASE("Query join() method", "[Q_JOIN]")
{
    SECTION("No join")
    {
        Relation *rel1, *rel2;

        rel1 = new Relation;
        rel1->numTuples = SIZE;
        rel1->initTuples();
        for (int i = 0; i < SIZE; i++)
        {
            rel1->setTupleVal(i, i+SIZE, i);
            REQUIRE(rel1->getTuples()[i].key == i+SIZE);
            REQUIRE(rel1->getTuples()[i].payloads[0] == i);
        }

        rel2 = new Relation;
        rel2->numTuples = SIZE;
        rel2->initTuples();
        for (int i = 0; i < SIZE; i++)
        {
            rel2->setTupleVal(i, i, i);
            REQUIRE(rel2->getTuples()[i].key == i);
            REQUIRE(rel2->getTuples()[i].payloads[0] == i);
        }

        Query q;
        List *l = nullptr;
        l = q.join(rel1, rel2);
        REQUIRE(q.rowsInResults == 0);
        REQUIRE(l == nullptr);

        delete l;
        delete rel1;
        delete rel2;
    }

    SECTION("Join of Relation objects of same values and size")
    {
        Relation *rel1, *rel2;

        rel1 = new Relation;
        rel1->numTuples = SIZE;
        rel1->initTuples();
        for (int i = 0; i < SIZE; i++)
        {
            rel1->setTupleVal(i, i, i);
            REQUIRE(rel1->getTuples()[i].key == i);
            REQUIRE(rel1->getTuples()[i].payloads[0] == i);
        }

        rel2 = new Relation;
        rel2->numTuples = SIZE;
        rel2->initTuples();
        rel2->copyTuplesVal(rel1, 0, SIZE-1);
        for (int i = 0; i < SIZE; i++)
        {
            REQUIRE(rel2->getTuples()[i].key == i);
            REQUIRE(rel2->getTuples()[i].payloads[0] == i);
        }

        Query q;
        List *l;
        l = q.join(rel1, rel2);
        REQUIRE(q.rowsInResults == SIZE);

        delete l;
        delete rel1;
        delete rel2;
    }

    SECTION("Join")
    {
        Relation *rel1, *rel2;

        rel1 = new Relation;
        rel1->numTuples = SIZE;
        rel1->initTuples();
        for (int i = 0; i < SIZE; i++)
        {
            rel1->setTupleVal(i, i, i);
            REQUIRE(rel1->getTuples()[i].key == i);
            REQUIRE(rel1->getTuples()[i].payloads[0] == i);
        }

        rel2 = new Relation;
        rel2->numTuples = 1;
        rel2->initTuples();
        rel2->setTupleVal(0,0,0);
        REQUIRE(rel2->getTuples()[0].key == 0);
        REQUIRE(rel2->getTuples()[0].payloads[0] == 0);

        Query q;
        List *l;
        l = q.join(rel1, rel2);
        REQUIRE(q.rowsInResults == 1);
        REQUIRE(l->head->data[0] == 0);

        delete l;
        delete rel1;
        delete rel2;
    }
}
