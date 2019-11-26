//
// Created by karolos on 26/11/19.
//

#include <iostream>
#include "Datatypes_part2.h"

Query::Query(int numOfMatrices, int numOfPredices, int numOfResults, int *matrices, double *results, Predicate *predicates)
:numOfMatrices(numOfMatrices), numOfPredicates(numOfPredices), numOfResults(numOfResults),
matrices(matrices), results(results), predicates(predicates)
{std::cout << "Iz a me, the query" << std::endl;}
