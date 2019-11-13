//
// Created by wmsksf on 04/11/19.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../project_jj_lib/DataTypes.h"
#include "../project_jj_lib/Utils.h"

#define RELSIZE 100000
#define ONEVALUE 453467

#define FILE_A_TINY "../data/Datasets/tiny/relA"
#define FILE_B_TINY "../data/Datasets/tiny/relB"

#define FILE_A_SMALL "../data/Datasets/small/relA"
#define FILE_B_SMALL "../data/Datasets/small/relB"

#define FILE_A_MEDIUM "../data/Datasets/medium/relA"
#define FILE_B_MEDIUM "../data/Datasets/medium/relB"

TEST_CASE("Set matrix object and get relation object from it", "[SETMTRX]")
{
    SECTION("Get size of matrix object")
    {
        char *file = (char*) malloc((strlen(FILE_A_SMALL) + 1) * sizeof(char));
        REQUIRE(file != NULL);
        memcpy(file, FILE_A_SMALL, (strlen(FILE_A_SMALL) + 1) * sizeof(char));

        Tuple a = getMatrixSize(file);
        REQUIRE(a.getKey() != 0);
        REQUIRE(a.getPayload() != 0);

        SECTION("Set matrix object")
        {
            Matrix* matrix = new Matrix(a.getPayload(),a.getKey());
            REQUIRE(matrix->setMatrix(file) != false);

            SECTION("Get relation object from matrix object")
            {
            Relation *rel = matrix->getRelation(1);
            REQUIRE(rel != nullptr);
            }
        }

        free(file);
    }
}

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

TEST_CASE("Radix sort test with tiny test files", "[RADIXTINY]")
{
    char *file1 = (char*) malloc((strlen(FILE_A_TINY) + 1) * sizeof(char));
    REQUIRE(file1 != NULL);
    memcpy(file1, FILE_A_TINY, (strlen(FILE_A_TINY) + 1) * sizeof(char));

    Tuple t1 = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(t1.getPayload(),t1.getKey());
    REQUIRE(matrix1 != nullptr);
    REQUIRE(matrix1->setMatrix(file1));

    char *file2 = (char*) malloc((strlen(FILE_B_TINY) + 1) * sizeof(char));
    REQUIRE(file2 != NULL);
    memcpy(file2, FILE_B_TINY, (strlen(FILE_B_TINY) + 1) * sizeof(char));

    Tuple t2 = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(t2.getPayload(),t2.getKey());
    REQUIRE(matrix2 != nullptr);
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
        count+= R1->getTuples()[i].getPayload();
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
        count+= R2->getTuples()[i].getPayload();
    REQUIRE( count == (size2*(size2-1)/2));

    delete matrix1; delete matrix2;
    delete R1; delete R2;
    free(file1); free(file2);
}

TEST_CASE("Radix sort test with small test files", "[RADIXSMALL]")
{
    char *file1 = (char*) malloc((strlen(FILE_A_SMALL) + 1) * sizeof(char));
    REQUIRE(file1 != NULL);
    memcpy(file1, FILE_A_SMALL, (strlen(FILE_A_SMALL) + 1) * sizeof(char));

    Tuple t1 = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(t1.getPayload(),t1.getKey());
    REQUIRE(matrix1 != nullptr);
    REQUIRE(matrix1->setMatrix(file1));

    char *file2 = (char*) malloc((strlen(FILE_B_SMALL) + 1) * sizeof(char));
    REQUIRE(file2 != NULL);
    memcpy(file2, FILE_B_SMALL, (strlen(FILE_B_SMALL) + 1) * sizeof(char));

    Tuple t2 = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(t2.getPayload(),t2.getKey());
    REQUIRE(matrix2 != nullptr);
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
        count+= R1->getTuples()[i].getPayload();
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
        count+= R2->getTuples()[i].getPayload();
    REQUIRE( count == (size2*(size2-1)/2));

    delete matrix1; delete matrix2;
    delete R1; delete R2;
    free(file1); free(file2);
}

TEST_CASE("Radix sort test with medium test files", "[RADIXMEDIUM]")
{
    char *file1 = (char*) malloc((strlen(FILE_A_MEDIUM) + 1) * sizeof(char));
    REQUIRE(file1 != NULL);
    memcpy(file1, FILE_A_MEDIUM, (strlen(FILE_A_MEDIUM) + 1) * sizeof(char));

    Tuple t1 = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(t1.getPayload(),t1.getKey());
    REQUIRE(matrix1 != nullptr);
    REQUIRE(matrix1->setMatrix(file1));

    char *file2 = (char*) malloc((strlen(FILE_B_MEDIUM) + 1) * sizeof(char));
    REQUIRE(file2 != NULL);
    memcpy(file2, FILE_B_MEDIUM, (strlen(FILE_B_MEDIUM) + 1) * sizeof(char));

    Tuple t2 = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(t2.getPayload(),t2.getKey());
    REQUIRE(matrix2 != nullptr);
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
        count+= R1->getTuples()[i].getPayload();
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
        count+= R2->getTuples()[i].getPayload();
    REQUIRE( count == (size2*(size2-1)/2));

    delete matrix1; delete matrix2;
    delete R1; delete R2;
    free(file1); free(file2);
}

TEST_CASE("SortMergeJoin test with tiny test files", "[SMJTINY]")
{

    char *file1 = (char*) malloc((strlen(FILE_A_TINY) + 1) * sizeof(char));
    REQUIRE(file1 != NULL);
    memcpy(file1, FILE_A_TINY, (strlen(FILE_A_TINY) + 1) * sizeof(char));

    Tuple t1 = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(t1.getPayload(),t1.getKey());
    REQUIRE(matrix1 != nullptr);
    REQUIRE(matrix1->setMatrix(file1));

    char *file2 = (char*) malloc((strlen(FILE_B_TINY) + 1) * sizeof(char));
    REQUIRE(file2 != NULL);
    memcpy(file2, FILE_B_TINY, (strlen(FILE_B_TINY) + 1) * sizeof(char));

    Tuple t2 = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(t2.getPayload(),t2.getKey());
    REQUIRE(matrix2 != nullptr);
    REQUIRE(matrix2->setMatrix(file2));

    Relation *R1,*R2;
    R1 = matrix1->getRelation(0);
    R2 = matrix2->getRelation(0);

    uint64_t size1 = R1->getNumTuples();
    uint64_t size2 = R2->getNumTuples();

    REQUIRE(size1 == 1000);
    REQUIRE(size2 == 2000);

    uint64_t count = 0;
    SortMergeJoin(R1,R2,count);

    REQUIRE(count == 640000);

    delete matrix1; delete matrix2;
    delete R1; delete R2;
    free(file1); free(file2);
}

TEST_CASE("SortMergeJoin test with small test files", "[SMJSMALL]")
{
    char *file1 = (char*) malloc((strlen(FILE_A_SMALL) + 1) * sizeof(char));
    REQUIRE(file1 != NULL);
    memcpy(file1, FILE_A_SMALL, (strlen(FILE_A_SMALL) + 1) * sizeof(char));

    Tuple t1 = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(t1.getPayload(),t1.getKey());
    REQUIRE(matrix1 != nullptr);
    REQUIRE(matrix1->setMatrix(file1));

    char *file2 = (char*) malloc((strlen(FILE_B_SMALL) + 1) * sizeof(char));
    REQUIRE(file2 != NULL);
    memcpy(file2, FILE_B_SMALL, (strlen(FILE_B_SMALL) + 1) * sizeof(char));

    Tuple t2 = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(t2.getPayload(),t2.getKey());
    REQUIRE(matrix2 != nullptr);
    REQUIRE(matrix2->setMatrix(file2));

    Relation *R1,*R2;
    R1 = matrix1->getRelation(0);
    R2 = matrix2->getRelation(0);

    uint64_t size1 = R1->getNumTuples();
    uint64_t size2 = R2->getNumTuples();

    REQUIRE(size1 == 10000);
    REQUIRE(size2 == 30000);

    uint64_t count = 0;
    SortMergeJoin(R1,R2,count,true);

    REQUIRE(count == 50000000);

    delete matrix1; delete matrix2;
    delete R1; delete R2;
    free(file1); free(file2);
}

TEST_CASE("SortMergeJoin test with medium test files", "[SMJMEDIUM]")
{
    char *file1 = (char*) malloc((strlen(FILE_A_MEDIUM) + 1) * sizeof(char));
    REQUIRE(file1 != NULL);
    memcpy(file1, FILE_A_MEDIUM, (strlen(FILE_A_MEDIUM) + 1) * sizeof(char));

    Tuple t1 = getMatrixSize(file1);
    Matrix* matrix1 = new Matrix(t1.getPayload(),t1.getKey());
    REQUIRE(matrix1 != nullptr);
    REQUIRE(matrix1->setMatrix(file1));

    char *file2 = (char*) malloc((strlen(FILE_B_MEDIUM) + 1) * sizeof(char));
    REQUIRE(file2 != NULL);
    memcpy(file2, FILE_B_MEDIUM, (strlen(FILE_B_MEDIUM) + 1) * sizeof(char));

    Tuple t2 = getMatrixSize(file2);
    Matrix* matrix2 = new Matrix(t2.getPayload(),t2.getKey());
    REQUIRE(matrix2 != nullptr);
    REQUIRE(matrix2->setMatrix(file2));

    Relation *R1,*R2;
    R1 = matrix1->getRelation(0);
    R2 = matrix2->getRelation(0);

    uint64_t size1 = R1->getNumTuples();
    uint64_t size2 = R2->getNumTuples();

    REQUIRE(size1 == 100000);
    REQUIRE(size2 == 1000000);

    uint64_t count = 0;
    SortMergeJoin(R1,R2,count,true);

    REQUIRE(count == 900000000);

    delete matrix1; delete matrix2;
    delete R1; delete R2;
    free(file1); free(file2);
}
