//
// Created by wmsksf on 22/11/19.
//

#include <cstdlib>
#include <climits>
#include <cstring>
#include <iostream>
#include "Utils_part2.h"

Matrix* Get_Relations(uint64_t &size)
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
            if (Datasets[relcount-1] == nullptr)
            {
                std::cerr << "Malloc of Datasets[i] failed." << std::endl;
                exit(1);
            }
            else
                memcpy(Datasets[relcount-1], filename, sizeof(char)*PATH_MAX);
        }
        else
        {
            More_Datasets = (char**) realloc(Datasets, sizeof(char*)*relcount);
            if (More_Datasets != nullptr)
            {
                More_Datasets[relcount] =  (char*) malloc(sizeof(char)*PATH_MAX);
                if (More_Datasets[relcount] == nullptr)
                {
                    std::cerr << "Malloc of More_Datasets[relcount] failed." << std::endl;
                    exit(1);
                }
                else
                    memcpy((More_Datasets + relcount), filename, sizeof(char)*PATH_MAX);
            }
            else
            {
                free(Datasets);
                std::cerr << "Malloc of More_Datasets failed." << std::endl;
                exit(1);
            }
        }
    }

//    exclude "DONE"
    relcount--;
    for(int i =0 ; i< relcount;i++) std::cout << Datasets[i] << std::endl;

    Matrix *matrices = (Matrix*) malloc(sizeof(Matrix)*relcount);
    if (matrices == nullptr)
    {
        std::cerr << "Malloc of array of Matrix objects failed." << std::endl;
        exit(1);
    }

    for (int i = 0; i < relcount; i++)
        matrices[i].setMatrix(Datasets[i]);

    for (int i = 0; i < relcount; i++)
    {
        std::cout << "rel" << i << std::endl;
        matrices[i].printMatrix();
    }

    size = relcount;
    return matrices;
}
