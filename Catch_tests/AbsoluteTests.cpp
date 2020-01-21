//
// Created by wmsksf on 04/11/19.
//

#define CATCH_CONFIG_MAIN

#define SSIZE 10000

#include "catch.hpp"
#include "../project_jj_lib_part2/Vector.h"
#include "../project_jj_lib_part2/List.h"
#include "../project_jj_lib_part2/Datatypes_part2.h"
#include "../project_jj_lib_part2/MACROS.h"
#include "../project_jj_lib_part3/JobScheduler.h"
#include "../project_jj_lib/Utils.h"

TEST_CASE("Vector object", "[VEC_OBJ]")
{
    Vector vec;
    REQUIRE(vec.size() == 0);
}

TEST_CASE("Vector push_back(), operator[] methods", "[VEC_PUSH_BACK]")
{
    Vector vec;

    for (int i = 0; i < SSIZE; i++)
    {
        vec.push_back(i);
        REQUIRE(vec[i] == i);
    }
    REQUIRE(vec.size() == SSIZE);
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
    for (int i = 0; i < SSIZE; i++)
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
    for (int i = 0; i < SSIZE; i++)
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
        rel1->numTuples = SSIZE;
        rel1->initTuples();
        for (int i = 0; i < SSIZE; i++)
        {
            rel1->setTupleVal(i, i+SSIZE, i);
            REQUIRE(rel1->getTuples()[i].key == i+SSIZE);
            REQUIRE(rel1->getTuples()[i].payloads[0] == i);
        }

        rel2 = new Relation;
        rel2->numTuples = SSIZE;
        rel2->initTuples();
        for (int i = 0; i < SSIZE; i++)
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

    SECTION("Join of Relation objects of same values and SSIZE")
    {
        Relation *rel1;

        rel1 = new Relation;
        rel1->numTuples = SSIZE;
        rel1->initTuples();
        for (int i = 0; i < SSIZE; i++)
        {
            rel1->setTupleVal(i, i, i);
            REQUIRE(rel1->getTuples()[i].key == i);
            REQUIRE(rel1->getTuples()[i].payloads[0] == i);
        }

        Query q;
        List *l;
        l = q.join(rel1, rel1);
        REQUIRE(q.rowsInResults == SSIZE);

        delete l;
        delete rel1;
    }

    SECTION("Join with relation of single value")
    {
        Relation *rel1, *rel2;

        rel1 = new Relation;
        rel1->numTuples = SSIZE;
        rel1->initTuples();
        for (int i = 0; i < SSIZE; i++)
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

TEST_CASE("Query equality_filter() method", "[Q_EQ_FLTR]")
{
    SECTION("Equality filtering after join of same relations")
    {
        Relation *rel1;

        rel1 = new Relation;
        rel1->numTuples = SSIZE;
        rel1->initTuples();
        for (int i = 0; i < SSIZE; i++)
            rel1->setTupleVal(i, i, i);

        Query q;
        q.ListOfResults = q.join(rel1, rel1);
        q.equality_filter(rel1, rel1);
        REQUIRE(q.rowsInResults == SSIZE);

        delete rel1;
    }

    SECTION("Equality filtering after join with relation of single value")
    {
        Relation *rel1, *rel2;

        rel1 = new Relation;
        rel1->numTuples = SSIZE;
        rel1->initTuples();
        for (int i = 0; i < SSIZE; i++)
            rel1->setTupleVal(i, i, i);

        rel2 = new Relation;
        rel2->numTuples = 1;
        rel2->initTuples();
        rel2->setTupleVal(0, 0, 0);

        Query q;
        q.ListOfResults = q.join(rel1, rel2);
        q.equality_filter(rel1, rel2);
        REQUIRE(q.rowsInResults == 1);

    }
}


TEST_CASE("Query statistics", "[Q_STATS]") {

    SECTION("No need for re-arrangement"){

        //load matrices
        MATRICES = new Matrix[13];
        for (int i = 0; i < 13; i++) {
            char fileName[5];
            sprintf(fileName,"r%i",i);
            MATRICES[i].setMatrix(fileName);
        }

        Query q;
        char *input;
        input = new char[80];
        memcpy(input, "9 0 2|0.1=1.0&1.0=2.2&0.0>12472|1.0 0.3 0.4", 80);

        q.parse(input);
        REQUIRE(q.NumOfMatrices == 3);
        REQUIRE(q.NumOfPredicates == 3);
        REQUIRE(q.NumOfResults == 3);

        //get matrices stats
        for(int i = 0 ; i < q.NumOfMatrices; i++){
            q.stats[i] = new Stats[MATRICES[q.Matrices[i]].numOfColumns];
            for(int j=0; j<MATRICES[q.Matrices[i]].numOfColumns;j++){
                q.stats[i][j].copy(&MATRICES[q.Matrices[i]].stats[j]);
            }
        }

        uint64_t f = 0;
        bool filters = q.filtering(f);
        q.plan_predicates();
        q.rearrange_predicates();

        REQUIRE(q.Predicates[0].MatricesIndex[0] == 0);
        REQUIRE(q.Predicates[0].MatricesIndex[1] == 1);
        REQUIRE(q.Predicates[1].MatricesIndex[0] == 1);
        REQUIRE(q.Predicates[1].MatricesIndex[1] == 2);

        delete[] MATRICES;
        delete[] input;
    }

    SECTION("Re-arranged predicates"){

        //load matrices
        MATRICES = new Matrix[13];
        for (int i = 0; i < 13; i++) {
            char fileName[5];
            sprintf(fileName,"r%i",i);
            MATRICES[i].setMatrix(fileName);
        }

        Query q;
        char *input;
        input = new char[80];
        memcpy(input, "3 0 1|0.2=1.0&0.1=2.0&0.2>3499|1.2 0.1", 80);

        q.parse(input);
        REQUIRE(q.NumOfMatrices == 3);
        REQUIRE(q.NumOfPredicates == 3);
        REQUIRE(q.NumOfResults == 2);

        //get matrices stats
        for(int i = 0 ; i < q.NumOfMatrices; i++){
            q.stats[i] = new Stats[MATRICES[q.Matrices[i]].numOfColumns];
            for(int j=0; j<MATRICES[q.Matrices[i]].numOfColumns;j++){
                q.stats[i][j].copy(&MATRICES[q.Matrices[i]].stats[j]);
            }
        }

        uint64_t f = 0;
        bool filters = q.filtering(f);
        q.plan_predicates();
        q.rearrange_predicates();

        REQUIRE(q.Predicates[0].MatricesIndex[0] == 0);
        REQUIRE(q.Predicates[0].MatricesIndex[1] == 2);
        REQUIRE(q.Predicates[1].MatricesIndex[0] == 0);
        REQUIRE(q.Predicates[1].MatricesIndex[1] == 1);

        delete[] MATRICES;
        delete[] input;
    }
}



TEST_CASE("Parallel sort", "[Q_PARALLEL_SORT]") {

    job_scheduler.init(8);
    Relation *rel1, *rel2;

    rel1 = new Relation;
    rel1->numTuples = SSIZE;
    rel1->initTuples();
    for (int i = 0; i < SSIZE; i++)
    {
        rel1->setTupleVal(i, SSIZE-1, i);
    }

    prepareRadix(rel1);
    REQUIRE(rel1->isSorted());

    rel2 = new Relation;
    rel2->numTuples = SSIZE;
    rel2->initTuples();
    for (int i = 0; i < SSIZE; i++)
    {
        rel2->setTupleVal(i, i, i);
    }

    prepareRadix(rel2);

    REQUIRE(rel2->isSorted());

    delete rel1;
    delete rel2;
//        job_scheduler.stop();

}



TEST_CASE("Parallel thread join 1", "[Q_PARALLEL_JOIN1]") {

    SECTION("No join")
    {
        job_scheduler.init(8);
        Relation *rel1, *rel2;

        rel1 = new Relation;
        rel1->numTuples = SSIZE;
        rel1->initTuples();
        for (int i = 0; i < SSIZE; i++)
        {
            rel1->setTupleVal(i, i+SSIZE, i);
            REQUIRE(rel1->getTuples()[i].key == i+SSIZE);
            REQUIRE(rel1->getTuples()[i].payloads[0] == i);
        }

        rel2 = new Relation;
        rel2->numTuples = SSIZE;
        rel2->initTuples();
        for (int i = 0; i < SSIZE; i++)
        {
            rel2->setTupleVal(i, i, i);
            REQUIRE(rel2->getTuples()[i].key == i);
            REQUIRE(rel2->getTuples()[i].payloads[0] == i);
        }

        Query q;
        List *l = nullptr;
        l = q.prepareJoin(rel1, rel2);
        REQUIRE(q.rowsInResults == 0);
        REQUIRE(l == nullptr);

        delete l;
        delete rel1;
        delete rel2;
//        job_scheduler.stop();
    }

}

TEST_CASE("Parallel thread join 2", "[Q_PARALLEL_JOIN2]") {
    SECTION("Join of Relation objects of same values and SSIZE")
    {
        job_scheduler.init(8);
        Relation *rel1;

        rel1 = new Relation;
        rel1->numTuples = SSIZE;
        rel1->initTuples();
        for (int i = 0; i < SSIZE; i++)
        {
            rel1->setTupleVal(i, i, i);
            REQUIRE(rel1->getTuples()[i].key == i);
            REQUIRE(rel1->getTuples()[i].payloads[0] == i);
        }

        Query q;
        List *l;
        l = q.prepareJoin(rel1, rel1);
        REQUIRE(q.rowsInResults == SSIZE);

        delete l;
        delete rel1;
//        job_scheduler.stop();
    }

}

TEST_CASE("Parallel thread join 3", "[Q_PARALLEL_JOIN3]") {
    SECTION("Join with relation of single value") {
        Relation *rel1, *rel2;

        rel1 = new Relation;
        rel1->numTuples = SSIZE;
        rel1->initTuples();
        for (int i = 0; i < SSIZE; i++) {
            rel1->setTupleVal(i, i, i);
            REQUIRE(rel1->getTuples()[i].key == i);
            REQUIRE(rel1->getTuples()[i].payloads[0] == i);
        }

        rel2 = new Relation;
        rel2->numTuples = 1;
        rel2->initTuples();
        rel2->setTupleVal(0, 0, 0);
        REQUIRE(rel2->getTuples()[0].key == 0);
        REQUIRE(rel2->getTuples()[0].payloads[0] == 0);

        Query q;
        List *l;
        l = q.prepareJoin(rel1, rel2);
        REQUIRE(q.rowsInResults == 1);
        REQUIRE(l->head->data[0] == 0);

        delete l;
        delete rel1;
        delete rel2;
        job_scheduler.stop();
    }
}