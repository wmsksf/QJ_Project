//
// Created by wmsksf on 22/11/19.
//

#include <cstdlib>
#include <climits>
#include <cstring>
#include <iostream>
#include "Utils_part2.h"
#include "MACROS.h"

uint64_t MATRICES_SIZE = 0;
Matrix *MATRICES = nullptr;

void Get_Relations()
{
    char **Datasets = nullptr, **More_Datasets = nullptr;
    Datasets = (char**) malloc(sizeof(char*)*MAX_DATASETS);

    uint64_t relcount = 0;
    char filename[PATH_MAX] = {};
    while (strcmp(filename, "DONE"))
    {
        std::cin >> filename;
        relcount++;

        if (relcount <= MAX_DATASETS)
        {
            Datasets[relcount-1] = (char*) malloc(sizeof(char)*PATH_MAX);
            ALLOC_CHECK(Datasets[relcount-1]);
            memcpy(Datasets[relcount-1], filename, sizeof(char)*PATH_MAX);
        }
        else
        {
            More_Datasets = (char**) realloc(Datasets, sizeof(char*)*relcount);
            if (More_Datasets != nullptr)
            {
                More_Datasets[relcount] =  (char*) malloc(sizeof(char)*PATH_MAX);
                ALLOC_CHECK(More_Datasets[relcount]);
                memcpy((More_Datasets + relcount), filename, sizeof(char)*PATH_MAX);
            }
            else
            {
                free(Datasets);
                ALLOC_CHECK(More_Datasets);
            }
        }
    }

//    exclude "DONE"
    relcount--;

    MATRICES= (Matrix*) malloc(sizeof(Matrix)*relcount);
    ALLOC_CHECK(MATRICES);

    for (int i = 0; i < relcount; i++)
       MATRICES[i].setMatrix(Datasets[i]);

    MATRICES_SIZE = relcount;
}
