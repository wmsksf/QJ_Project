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

Query* Parser(char* input){

    Query* Q;

    char* matrices = strtok(input,"|");
    char* predicates = strtok(NULL,"|");
    char* results = strtok(NULL,"");
    printf("%s\n%s\n%s\n",matrices,predicates, results);

    int numOfMatrices = 0;
    for(int i = 1; i < strlen(matrices)+1; i++){
        if((matrices[i-1] < '0' or matrices[i-1] > '9') and matrices[i-1] != ' '){
            std::cerr << "Invalid input to parser" << std::endl;
            return Q;
        }
        if(matrices[i-1] >= '0' and matrices[i-1] <= '9' and (matrices[i] == '\0' or matrices[i] == ' '))
            numOfMatrices++;
    }
    int* matrixArray= new int[numOfMatrices];
    char* tmp = strtok(matrices," ");
    matrixArray[0] = atoi(tmp);
    for(int i =1; i < numOfMatrices; i++){
        tmp = strtok(NULL," ");
        matrixArray[i] = atoi(tmp);
        if(matrixArray[i] < 0){
            std::cerr << "Invalid input to parser" << std::endl;
            return Q;
        }
        else if(matrixArray[i] == 0 and strcmp(tmp,"0")!=0){
            std::cerr << "Invalid input to parser" <<  std::endl;
            return Q;
        }
    }

    int numOfResults = 0;
    for(int i = 1; i < strlen(results)+1; i++){
        if((results[i-1] < '0' or results[i-1] > '9') and results[i-1] != ' ' and results[i-1] != '.'){
            std::cerr << "Invalid input to parser" << std::endl;
            return Q;
        }
        if(results[i-1] >= '0' and results[i-1] <= '9' and (results[i] == '\0' or results[i] == ' '))
            numOfResults++;
    }
    double* matrixResults= new double[numOfResults];
    tmp = strtok(results," ");
    matrixResults[0] = atof(tmp);
    for(int i =1; i < numOfResults; i++){
        tmp = strtok(NULL," ");
        matrixResults[i] = atof(tmp);
        if(matrixResults[i] < 0){
            std::cerr << "Invalid input to parser" << std::endl;
            return Q;
        }
    }

    int numOfPredicates = 0;
    for(int i = 1; i < strlen(predicates)+1; i++){
        // Looking for illegal characters in the predicates
        if((predicates[i-1] < '0' or predicates[i-1] > '9') and predicates[i-1] != '>' and predicates[i-1] != '<'
                and predicates[i-1] != '=' and predicates[i-1] != '&' and predicates[i-1] != '.'){
            std::cerr << "Invalid input to parser" << std::endl;
            return Q;
        }
        // Counting the predicates
        if(predicates[i-1] >= '0' and predicates[i-1] <= '9' and (predicates[i] == '\0' or predicates[i] == '&'))
            numOfPredicates++;
    }

    Predicate* predicatesArray = new Predicate[numOfPredicates];

    std::cout << predicates << std::endl;

    char* strArray[numOfPredicates];

    strArray[0] = strtok(predicates,"&"); // reading the 1st predicate
    for(int i =1; i< numOfPredicates; i++)
        strArray[i] = strtok(NULL,"&");

    for(int j =0; j<numOfPredicates;j++) {
        char operation = 'a';
        bool operationFound = false;

        tmp = strtok(strArray[j],"&");

        for(int i =0; i < strlen(tmp);i++) {
            //Looking for the operation symbol '>' or '<' or '='
            if (tmp[i - 1] == '>' or tmp[i - 1] == '=' or tmp[i - 1] == '<') {
                if (operation == 'a') {  //checking for more than 1 ope
                    operation = tmp[i - 1];
                    operationFound = true;
                } else {
                    std::cerr << "Invalid input to parser" << std::endl;
                    return Q;
                }
            }

            if (operationFound and tmp[i - 1] == '.')
                operation = 'j';
        }

        char del[2] = {operation};
        if(del[0] == 'j')
            del[0] = '=';

        tmp = strtok(tmp,".");
        predicatesArray[j].Matrices[0] = atoi(tmp);
        tmp = strtok(NULL,del);
        predicatesArray[j].RowIds[0] = atoi(tmp);
        if(operation == 'j') {
            tmp = strtok(NULL,".");
            predicatesArray[j].Matrices[1] = atoi(tmp);
            tmp = strtok(NULL, "\0");
            predicatesArray[j].Matrices[1] = atoi(tmp);
        }else{
            tmp = strtok(NULL, "\0");
            predicatesArray[j].filter = atoll(tmp);
        }

    }

    Q = new Query(numOfMatrices, numOfPredicates, numOfResults,matrixArray,matrixResults,predicatesArray);

}