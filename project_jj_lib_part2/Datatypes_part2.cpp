//
// Created by karolos on 26/11/19.
//

#include <iostream>
#include <cstring>
#include <cmath>
#include "Datatypes_part2.h"
#include "MACROS.h"
#include "../project_jj_lib/Utils.h"

Predicate::Predicate() {
    operation = '\0';

    Matrices[0] = Matrices[1] = -1;
    RowIds[0] = RowIds[1] = -1;

    filter = 0;
}

char Predicate::getOperation() {
    return operation;
}

Query::Query() {
    NumOfMatrices = NumOfPredicates = NumOfResults = 0;
    Matrices = nullptr;
    Results = nullptr;
    Predicates = nullptr;
    FilteredMatrices[0] = FilteredMatrices[1] = FilteredMatrices[2] = FilteredMatrices[3] = nullptr;
    ListOfResults = nullptr;
    MatricesJoined = nullptr;
}

inline void parse_err() {
    std::cerr << "Invalid inq to parser" << std::endl;
    exit(EXIT_FAILURE);
}

void Query::parse(char *inq) {
    char *matrices = strtok(inq, "|");
    char *predicates = strtok(nullptr, "|");
    char *results = strtok(nullptr, "");

//  calc num of matrices
    for (int i = 1; i < strlen(matrices) + 1; i++) {
        if ((matrices[i - 1] < '0' or matrices[i - 1] > '9') and matrices[i - 1] != ' ') parse_err();
        if (matrices[i - 1] >= '0' and matrices[i - 1] <= '9' and (matrices[i] == '\0' or matrices[i] == ' '))
            NumOfMatrices++;
    }
//    alloc Matrices and set
    Matrices = new int[NumOfMatrices];
    ALLOC_CHECK(Matrices);

    char *tmp = strtok(matrices, " ");
    Matrices[0] = atoi(tmp);
    for (int i = 1; i < NumOfMatrices; i++) {
        tmp = strtok(nullptr, " ");
        Matrices[i] = atoi(tmp);
        if (Matrices[i] < 0) parse_err();
        else if (Matrices[i] == 0 and strcmp(tmp, "0") != 0) parse_err();
    }

//    calc num of results
    for (int i = 1; i < strlen(results) + 1; i++) {
        if ((results[i - 1] < '0' or results[i - 1] > '9') and results[i - 1] != ' ' and
            results[i - 1] != '.')
            parse_err();
        if (results[i - 1] >= '0' and results[i - 1] <= '9' and (results[i] == '\0' or results[i] == ' '))
            NumOfResults++;
    }
//    alloc Results and set
    Results = new double[NumOfResults];
    ALLOC_CHECK(Results);

    tmp = strtok(results, " ");
    Results[0] = atof(tmp);
    for (int i = 1; i < NumOfResults; i++) {
        tmp = strtok(nullptr, " ");
        Results[i] = atof(tmp);
        if (Results[i] < 0) parse_err();
    }

//    calc num of predicates
    for (int i = 1; i < strlen(predicates) + 1; i++) {
        // Looking for illegal characters in the predicates
        if ((predicates[i - 1] < '0' or predicates[i - 1] > '9') and predicates[i - 1] != '>' and
            predicates[i - 1] != '<'
            and predicates[i - 1] != '=' and predicates[i - 1] != '&' and predicates[i - 1] != '.')
            parse_err();
        // Counting the predicates
        if (predicates[i - 1] >= '0' and predicates[i - 1] <= '9' and (predicates[i] == '\0' or predicates[i] == '&'))
            NumOfPredicates++;
    }

    Predicates = new Predicate[NumOfPredicates];
    ALLOC_CHECK(Predicates);
    char *strArray[NumOfPredicates];

    strArray[0] = strtok(predicates, "&"); // reading the 1st predicate
    for (int i = 1; i < NumOfPredicates; i++)
        strArray[i] = strtok(nullptr, "&");

    for (int j = 0; j < NumOfPredicates; j++) {
        char operation = 'a';
        bool operationFound = false;

        tmp = strtok(strArray[j], "&");

        for (int i = 0; i < strlen(tmp); i++) {
            //Looking for the operation symbol '>' or '<' or '='
            if (tmp[i - 1] == '>' or tmp[i - 1] == '=' or tmp[i - 1] == '<') {
                //More than 1 operation symbol is not allowed within the same predicate
                if (operation == 'a') {  //checking for more than 1 ope
                    operation = tmp[i - 1];
                    operationFound = true;
                } else parse_err();
            }

            if (operationFound and tmp[i - 1] == '.')
                operation = 'j';
        }

        char del[2] = {operation};
        if (del[0] == 'j')
            del[0] = '=';

        tmp = strtok(tmp, ".");
//        in predicates as given when there is <rel>.<relcolmn>, <rel> is index of rel in Matrices[], in class Query, given .pdf file of second part of project
        Predicates[j].Matrices[0] = Matrices[atoi(tmp)];
        tmp = strtok(nullptr, del);
        Predicates[j].RowIds[0] = atoi(tmp);
        if (operation == 'j') {
            tmp = strtok(nullptr, ".");
            Predicates[j].Matrices[1] = Matrices[atoi(tmp)];
            tmp = strtok(nullptr, "\0");
            Predicates[j].RowIds[1] = atoi(tmp);
        } else {
            tmp = strtok(nullptr, "\0");
            Predicates[j].filter = atoll(tmp);
        }
        Predicates[j].operation = operation;
    }
}

bool Query::filtering(uint64_t &size) {
//    calc num of filters
    uint64_t v = 0;
    for (uint64_t i = 0; i < NumOfPredicates; i++)
        if (Predicates[i].filter) v++;

    for (uint64_t i = 0, vv = 0; i < NumOfPredicates; i++)
        if (Predicates[i].filter) {
            Relation *rel;

            rel = MATRICES[Predicates[i].Matrices[0]].getRelation(Predicates[i].RowIds[0]);
            if (rel == nullptr)
                return false;

            auto vector = new Vector();

            Tuple *tuples = rel->getTuples();
            uint64_t numOfTuples = rel->getNumTuples();
            switch (Predicates[i].operation) {
                case '>':
                    for (uint64_t j = 0; j < numOfTuples; j++)
                        if (tuples[j].getKey() > Predicates[i].filter)
                            vector->push_back(tuples[j].getPayloads()[0]);
                    vv++;
                    break;
                case '<':
                    for (uint64_t j = 0; j < rel->getNumTuples(); j++)
                        if (tuples[j].getKey() < Predicates[i].filter)
                            vector->push_back(tuples[j].getPayloads()[0]);
                    vv++;
                    break;
                case '=':
                    for (uint64_t j = 0; j < rel->getNumTuples(); j++)
                        if (tuples[j].getKey() == Predicates[i].filter)
                            vector->push_back(tuples[j].getPayloads()[0]);
                    vv++;
                    break;
                default:
                    std::cout << "Invalid operation for filtering!" << std::endl;
                    return false;
            }

            for (int x = 0; x < NumOfMatrices; x++) {
                if (Matrices[x] == Predicates[i].Matrices[0])
                    FilteredMatrices[x] = vector;
            }
        }

    size = v;
    return true;
}

void Query::exec() {
//    start with filtering query
    uint64_t f = 0;
    bool filters = filtering(f);
    if (!filters) {
        empty_sum();
        return;
    }

    //Then we execute the joins

    for (int i = 0; i < NumOfPredicates; i++) {
        char operation = Predicates[i].getOperation();
        if (operation != 'j')  //not a join operation
            continue;

        //Get the first relation of the predicate, filter it and sort it
        Relation *R1 = MATRICES[Predicates[i].Matrices[0]].getRelation(Predicates[i].RowIds[0]);
//        for (int j = 0; j < NumOfMatrices; j++) {
//            if (Predicates[i].Matrices[0] == Matrices[j]) {
//                R1->filter(FilteredMatrices[j]);
//                break;
//            }
//        }

        if (R1->getNumTuples() == 0) {
            empty_sum();
            return;
        }
        R1->print();

        Radixsort(R1, 0, R1->getNumTuples() - 1);

        //Same thing for the second relation of the predicate
        Relation *R2 = MATRICES[Predicates[i].Matrices[1]].getRelation(Predicates[i].RowIds[1]);
//        for (int j = 0; j < NumOfMatrices; j++) {
//            if (Predicates[i].Matrices[1] == Matrices[j]) {
//                R2->filter(FilteredMatrices[j]);
//                break;
//            }
//        }

        if (R2->getNumTuples() == 0) {
            empty_sum();
            return;
        }
        std::cout << "next rel\n";
        R2->print();
        Radixsort(R2, 0, R2->getNumTuples() - 1);


        if (MatricesJoined != nullptr and MatricesJoined->search(Predicates[i].Matrices[0])) {
            //Save the joined result in an array
            LinkedList *tmp = SortMergeJoin(R1, R2);
          //  expandResultsList(tmp, Predicates[i].Matrices[0], Predicates[i].Matrices[1]);
        } else {
            LinkedList *tmp = SortMergeJoin(R2, R1);
            //expandResultsList(tmp, Predicates[i].Matrices[1], Predicates[i].Matrices[0]);
        }

    }

    calc_sum();
}


//void Query::expandResultsList(LinkedList *latestJoin, uint64_t A, uint64_t B) {
//
//    if (latestJoin == nullptr) return;
//
//    if (ListOfResults == nullptr) {  //List of Results is empty
//        MatricesJoined = new Vector();
//        MatricesJoined->push_back(A);   //Write the names of the first 2 joined matrices
//        MatricesJoined->push_back(B);
//        ListOfResults = new List();
//
//        //Add all joined tuples to our list of results
//        for (struct node *tmp = latestJoin->getHead(); tmp != nullptr; tmp = tmp->next) {
//            Tuple *t = tmp->Data.getBuffer();
//            uint64_t size = tmp->Data.getIndex();
//            for (uint64_t i = 0; i < size; i++) {
//                struct Node *node = ListOfResults->insert_node();
//                ListOfResults->insert(node, t[i].getKey());
//                ListOfResults->insert(node, t[i].getPayload());
//            }
//        }
//    } else {   //At least 2 matrices are in list of results
//        MatricesJoined->push_back(B);
//        int x = -1;
//        for (int i = 0; i < MatricesJoined->size(); i++) {
//            if ((*MatricesJoined)[i] == A) x = i;   //get the x position of matrix A in the list of arrays
//        }
//        if (x == -1) {
//            std::cout << "Matrix A not in MatricesJoined" << std::endl;
//            exit(1);
//        }
//        List *newList = new List();
//
//        //create a new list with
//        for (struct node *tmp = latestJoin->getHead(); tmp != nullptr; tmp = tmp->next) {
//            Tuple *t = tmp->Data.getBuffer();
//            uint64_t size = tmp->Data.getIndex();
//            for (uint64_t i = 0; i < size; i++) {
//                for (struct Node *node = ListOfResults->getHead(); node != nullptr; node = node->next) {
//                    if (t[i].getKey() == (node->data[x])) {
//                        struct Node *newNode = newList->insert_node();
//                        newNode->data = node->data;
//                        newNode->data.push_back(t[i].getPayload());
//                    }
//                }
//            }
//        }
//        //delete ListOfResults;
//        ListOfResults = newList;
//    }
//}

int fracto_int(double number, int dec_num) {
    double dummy;
    double frac = modf(number, &dummy);
    return round(frac * pow(10, dec_num));
}

void Query::empty_sum() {
    for (uint64_t i = 0; i < NumOfResults; i++)
        std::cout << "NULL ";
    std::cout << std::endl;
}

void Query::calc_sum() {
    Vector sum;
    Tuple *data;
    uint64_t s = 0;
    for (uint64_t i = 0; i < NumOfResults; i++) {
        double frack, intprt;
        int x, y;
        frack = modf(Results[i], &intprt);
        x = (int) intprt;
        y = fracto_int(frack, 1);

        if (!MatricesJoined->search(Matrices[x])) {
            std::cout << "No such relation in joined ones!" << std::endl;
            return;
        }

        int indx = MatricesJoined->getIndex(Matrices[x]);
        if (indx != -1) {
            Relation *rel = MATRICES[Matrices[x]].getRelation(y);
            data = rel->getTuples();
            for (struct Node *h = ListOfResults->getHead(); h != nullptr; h = h->next) {
                if (h->data[indx] > rel->getNumTuples()) {
                    std::cout << "RowId OUT OF BOUNDS: " << "max " << rel->getNumTuples()
                              << " while indx " << h->data[indx] << std::endl;
                    return;
                }
                s += data[h->data[indx]].getKey();
            }
            sum.push_back(s);
        } else {
            std::cout << "No such relation in List object" << std::endl;
            return;
        }
    }

    for (uint64_t i = 0; i < sum.size(); i++)
        std::cout << sum[i] << " ";

    std::cout << std::endl;
}
