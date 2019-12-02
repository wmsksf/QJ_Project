//
// Created by wmsksf on 02/12/19.
//

#ifndef PROJECT_JJ_EXECUTION_H
#define PROJECT_JJ_EXECUTION_H

#include "Datatypes_part2.h"
#include "List.h"

class Execution
{
    Predicate *filters;
    Relation *filtered_rels;

    Vector *relations;
    List intermediate_structure;
};

#endif //PROJECT_JJ_EXECUTION_H
