//
// Created by wmsksf on 04/11/19.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../project_jj_lib/DataTypes.h"
#include "../project_jj_lib/Utils.h"

#define RELSIZE 100000
#define ONEVALUE 453467

// 4 assertions
//TEST_CASE("Set matrix object and get relation object from it", "[SETMATRIXANDGETRELATIONBJECTSTEST]")
//{
//    SECTION("Get size of matrix object")
//    {
//        const char* file = "relA";
//        Tuple a = getMatrixSize(file);
//        REQUIRE(a.getKey() != 0);
//        REQUIRE(a.getPayload() != 0);
//
//        SECTION("Set matrix object")
//        {
//            Matrix* matrix = new Matrix(a.getPayload(),a.getKey());
//            REQUIRE(matrix->setMatrix(file) != false);
//
//            SECTION("Get relation object from matrix object")
//            {
//            Relation *rel = matrix->getRelation(1);
//            REQUIRE(rel != nullptr);
//            }
//        }
//    }
//}

// BEHAVIOR REGARDING NATURE OF DATA IN ARRAY
//  CHANGE RELSIZE TO TEST SIZE OF ARRAY
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
TEST_CASE("Iterative quick sort with random data", "[QUICKRAND]")
{
    Relation *r = new Relation;
    r->setNumTuples(RELSIZE);
    REQUIRE(r->getNumTuples() == RELSIZE);

    r->initTuples();
    Tuple *rel = r->getTuples();
    for (uint64_t i = 0; i < RELSIZE; i++)
    {
        rel[i].setKey(mcg64());
        REQUIRE(rel[i].getKey() >= 0);
        REQUIRE(rel[i].getKey() <= 18446744073709551615);

        rel[i].setPayload(i);
        REQUIRE(rel[i].getPayload() == i);
    }

    OptQuicksort(rel, 0, RELSIZE-1);

    uint64_t a = rel[0].getKey();
    for (uint64_t i = 1; i < RELSIZE-1; i++)
    {
    uint64_t b = rel[i].getKey();
    REQUIRE(a <= b);
    a = b;

    }

    delete r;
}

TEST_CASE("Iterative quick sort with data in descending order", "[QUICKDESCND]")
{
    Relation *r = new Relation;
    r->setNumTuples(RELSIZE);
    REQUIRE(r->getNumTuples() == RELSIZE);

    r->initTuples();
    Tuple *rel = r->getTuples();

    for (uint64_t i = 0; i < RELSIZE; i++)
    {
        rel[i].setKey(mcg64());
        REQUIRE(rel[i].getKey() >= 0);
        REQUIRE(rel[i].getKey() <= 18446744073709551615);

        rel[i].setPayload(i);
        REQUIRE(rel[i].getPayload() == i);
    }

    Tuple j, k;
    for (uint64_t i = 0; i < RELSIZE/2; i++)
    {
        j = rel[i];
        k = rel[RELSIZE-i-1];

        rel[i].swap(&rel[RELSIZE-i-1]);

        REQUIRE(k.equal(rel[i]));
        REQUIRE(j.equal(rel[RELSIZE-i-1]));

    }

    OptQuicksort(rel, 0, RELSIZE-1);

    uint64_t a = rel[0].getKey();
    for (uint64_t i = 1; i < RELSIZE-1; i++)
    {
        uint64_t b = rel[i].getKey();
        REQUIRE(a <= b);
        a = b;
    }

    delete r;
}

TEST_CASE("Iterative quick sort with data of single value", "[QUICKONE]")
{
    Relation *r = new Relation;
    r->setNumTuples(RELSIZE);
    REQUIRE(r->getNumTuples() == RELSIZE);

    r->initTuples();
    Tuple *rel = r->getTuples();

    for (uint64_t i = 0; i < RELSIZE; i++)
    {
        rel[i].setKey(ONEVALUE);
        REQUIRE(rel[i].getKey() == ONEVALUE);

        rel[i].setPayload(i);
        REQUIRE(rel[i].getPayload() == i);
    }

    OptQuicksort(rel, 0, RELSIZE-1);

    uint64_t a = rel[0].getKey();
    for (uint64_t i = 1; i < RELSIZE-1; i++)
    {
        uint64_t b = rel[i].getKey();
        REQUIRE(a <= b);
        a = b;
    }

    delete r;
}
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//
//TEST_CASE("Radix sort", "[RADIXSORTTEST]")
//{
//
//    const char* file = "relA";
//    Tuple a = getMatrixSize(file);
//    Matrix* matrix = new Matrix(a.getPayload(),a.getKey());
//    Relation *rel = matrix->getRelation(1);
//
//    SECTION("Size of relation object")
//    {
//        uint64_t size = rel->getNumTuples();
//        REQUIRE(size != 0);
//
//        SECTION("Performance of radix sort")
//        {
//            Radixsort(rel, 0, size-1);
//            REQUIRE(rel->isSorted() != false);
//        }
//    }
//}


TEST_CASE("Radix sort test with tiny test files", "[RADIXTINY]")
{

    const char* file1 = "relA_tiny";
    Tuple t1 = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(t1.getPayload(),t1.getKey());
    REQUIRE(matrix1->setMatrix(file1));


    const char* file2 = "relB_tiny";
    Tuple t2 = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(t2.getPayload(),t2.getKey());
    REQUIRE(matrix2->setMatrix(file2));


    Relation *R1,*R2;
    R1 = matrix1->getRelation(0);
    R2 = matrix2->getRelation(0);

    uint64_t size1 = R1->getNumTuples();
    uint64_t size2 = R2->getNumTuples();

    REQUIRE(size1 == 1000);
    REQUIRE(size2 == 2000);

    Radixsort(R1,0,size1-1);

    uint64_t  a = R1->getTuples()[0].getKey();
    for (uint64_t i = 1; i <size1 ; i++)
    {
        uint64_t b = R1->getTuples()[i].getKey();
        REQUIRE(a <= b);
        a = b;

    }
    uint64_t count = 0;

    for (uint64_t i = 0; i <size1 ; i++)
    {
        count+= R1->getTuples()[i].getPayload();
    }
    REQUIRE( count == (size1*(size1-1)/2));


    Radixsort(R2,0,size2-1);

    a = R2->getTuples()[0].getKey();
    for (uint64_t i = 1; i < size2; i++)
    {
        uint64_t b = R2->getTuples()[i].getKey();
        REQUIRE(a <= b);
        a = b;

    }

    count = 0;
    for (uint64_t i = 0; i <size2 ; i++)
    {
        count+= R2->getTuples()[i].getPayload();
    }
    REQUIRE( count == (size2*(size2-1)/2));

    delete matrix1;
    delete matrix2;
    delete R1;
    delete R2;
}


TEST_CASE("Radix sort test with small test files", "[RADIXSMALL]")
{

    const char* file1 = "relA_small";
    Tuple t1 = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(t1.getPayload(),t1.getKey());
    REQUIRE(matrix1->setMatrix(file1));


    const char* file2 = "relB_small";
    Tuple t2 = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(t2.getPayload(),t2.getKey());
    REQUIRE(matrix2->setMatrix(file2));


    Relation *R1,*R2;
    R1 = matrix1->getRelation(0);
    R2 = matrix2->getRelation(0);

    uint64_t size1 = R1->getNumTuples();
    uint64_t size2 = R2->getNumTuples();

    REQUIRE(size1 == 10000);
    REQUIRE(size2 == 30000);

    Radixsort(R1,0,size1-1);

    uint64_t  a = R1->getTuples()[0].getKey();
    for (uint64_t i = 1; i <size1 ; i++)
    {
        uint64_t b = R1->getTuples()[i].getKey();
        REQUIRE(a <= b);
        a = b;

    }
    uint64_t count = 0;

    for (uint64_t i = 0; i <size1 ; i++)
    {
        count+= R1->getTuples()[i].getPayload();
    }
    REQUIRE( count == (size1*(size1-1)/2));


    Radixsort(R2,0,size2-1);

    a = R2->getTuples()[0].getKey();
    for (uint64_t i = 1; i < size2; i++)
    {
        uint64_t b = R2->getTuples()[i].getKey();
        REQUIRE(a <= b);
        a = b;

    }

    count = 0;
    for (uint64_t i = 0; i <size2 ; i++)
    {
        count+= R2->getTuples()[i].getPayload();
    }
    REQUIRE( count == (size2*(size2-1)/2));

    delete matrix1;
    delete matrix2;
    delete R1;
    delete R2;
}

TEST_CASE("Radix sort test with medium test files", "[RADIXMEDIUM]")
{

    const char* file1 = "relA";
    Tuple t1 = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(t1.getPayload(),t1.getKey());
    REQUIRE(matrix1->setMatrix(file1));


    const char* file2 = "relB";
    Tuple t2 = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(t2.getPayload(),t2.getKey());
    REQUIRE(matrix2->setMatrix(file2));


    Relation *R1,*R2;
    R1 = matrix1->getRelation(0);
    R2 = matrix2->getRelation(0);

    uint64_t size1 = R1->getNumTuples();
    uint64_t size2 = R2->getNumTuples();

    REQUIRE(size1 == 100000);
    REQUIRE(size2 == 1000000);

    Radixsort(R1,0,size1-1);

    uint64_t  a = R1->getTuples()[0].getKey();
    for (uint64_t i = 1; i <size1 ; i++)
    {
        uint64_t b = R1->getTuples()[i].getKey();
        REQUIRE(a <= b);
        a = b;

    }
    uint64_t count = 0;

    for (uint64_t i = 0; i <size1 ; i++)
    {
        count+= R1->getTuples()[i].getPayload();
    }
    REQUIRE( count == (size1*(size1-1)/2));


    Radixsort(R2,0,size2-1);

    a = R2->getTuples()[0].getKey();
    for (uint64_t i = 1; i < size2; i++)
    {
        uint64_t b = R2->getTuples()[i].getKey();
        REQUIRE(a <= b);
        a = b;

    }

    count = 0;
    for (uint64_t i = 0; i <size2 ; i++)
    {
        count+= R2->getTuples()[i].getPayload();
    }
    REQUIRE( count == (size2*(size2-1)/2));

    delete matrix1;
    delete matrix2;
    delete R1;
    delete R2;
}