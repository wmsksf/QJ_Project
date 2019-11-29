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

// if relation flag= false else (query) flag= true
char** Get_Input(bool flag, uint64_t *size)
{
    char end[5] = {};
    if (!flag)  memcpy(end, "DONE", 5);
    else memcpy(end, "F", 5);

    char **Datasets = nullptr, **More_Datasets = nullptr;
    Datasets = (char**) malloc(sizeof(char*)*MAX_DATASETS);

    uint64_t count = 0;
    char filename[PATH_MAX] = {};
    while (strcmp(filename, end))
    {
        if (!flag)
            std::cin >> filename;
        else
            std::cin.getline(filename, sizeof(char)*PATH_MAX);
        count++;

        if (count <= MAX_DATASETS)
        {
            Datasets[count-1] = (char*) malloc(sizeof(char)*PATH_MAX);
            ALLOC_CHECK(Datasets[count-1]);
            memcpy(Datasets[count-1], filename, sizeof(char)*PATH_MAX);
        }
        else
        {
//            corrupted size vs prev_size : misuse of realloc !!WILL BE FIXED LATER ON
            More_Datasets = (char**) realloc(Datasets, sizeof(char*)*count);
            if (More_Datasets != nullptr)
            {
                More_Datasets[count] =  (char*) malloc(sizeof(char)*PATH_MAX);
                ALLOC_CHECK(More_Datasets[count]);
                memcpy((More_Datasets + count), filename, sizeof(char)*PATH_MAX);
            }
            else
            {
                for (int i = 0; i < count; i++) free(Datasets[i]);
                free(Datasets);

                ALLOC_CHECK(More_Datasets);
            }
        }
    }

//    exclude end
    count--;

    if (!flag)
    {
        MATRICES= (Matrix*) malloc(sizeof(Matrix)*count);
        ALLOC_CHECK(MATRICES);

        for (int i = 0; i < count; i++)
            MATRICES[i].setMatrix(Datasets[i]);

        MATRICES_SIZE = count;

//         no need for filenames anymore
        for (int i = 0; i < count; i++) free(Datasets[i]);
        free(Datasets);

        return nullptr;
    }
    else
    {
        if (std::cin.peek() == EOF)
            return nullptr;

        *size = count;
        return Datasets;
    }
}


void Set_output()
{
    uint64_t nqueries = 0, count = 0;
    char **Queries;
    while (1)
    {
        Queries = Get_Input(true, &nqueries);
        if (Queries == nullptr)
        {
            std::cout << "End of batches of queries!" << std::endl;
            break;
        }

        std::cout << "total queries in " << count << " batch: " << nqueries << std::endl;
        for (uint64_t i = 0; i < nqueries; i++) std::cout << Queries[i] << std::endl;
        std::cout << std::endl;

        count++;

//        clean test
        for (int i = 0; i < count; i++) free(Queries[i]);
        free(Queries);
    }
}