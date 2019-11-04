//
// Created by wmsksf on 04/11/19.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../project_jj_lib/DataTypes.h"
#include "../project_jj_lib/Utils.h"

#define RELSIZE 10000

TEST_CASE("Set matrix object and get relation object from it", "[SETMATRIXANDGETRELATIONBJECTSTEST]")
{
    SECTION("Get size of matrix object")
    {
        const char* file = "relA";
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
    }
}

TEST_CASE("Iterative quick sort", "[ITERATIVEQUICKSORT]")
{
    Tuple rel[RELSIZE];

    SECTION("Random values to be sorted")
    {
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
    }

    SECTION("Zero values")
    {
        for (uint64_t i = 0; i < RELSIZE; i++)
        {
            rel[i].setKey(0);
            REQUIRE(rel[i].getKey() == 0);

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
    }
}
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