//
// Created by karolos on 26/11/19.
//

#include <iostream>
#include <cstring>
#include <cmath>
#include "Datatypes_part2.h"
#include "MACROS.h"
#include "../project_jj_lib/Utils.h"
#include <time.h>

Predicate::Predicate() {
    operation = '\0';

    Matrices[0] = Matrices[1] = -1;
    RowIds[0] = RowIds[1] = -1;
    MatricesIndex[0] = MatricesIndex[1] = -1;

    filter = 0;
}

Query::Query() {
    NumOfMatrices = NumOfPredicates = NumOfResults = 0;
    Matrices = nullptr;
    Results = nullptr;
    Predicates = nullptr;
    FilteredMatrices[0] = FilteredMatrices[1] = FilteredMatrices[2] = FilteredMatrices[3] = nullptr;
    ListOfResults = nullptr;
    MatricesJoined = nullptr;
    rowsInResults = 0;
}

Query::~Query()
{
    delete[] Matrices;
    delete[] Results;
    delete[] Predicates;

    delete MatricesJoined;
    delete ListOfResults;

    for(auto & FilteredMatrice : FilteredMatrices){
        delete FilteredMatrice;
    }

    for(int i = 0 ; i < NumOfMatrices; i++)
        delete[] stats[i];
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
    for (uint64_t i = 1; i < strlen(matrices) + 1; i++) {
        if ((matrices[i - 1] < '0' or matrices[i - 1] > '9') and matrices[i - 1] != ' ') parse_err();
        if (matrices[i - 1] >= '0' and matrices[i - 1] <= '9' and (matrices[i] == '\0' or matrices[i] == ' '))
            NumOfMatrices++;
    }
//    alloc Matrices and set
    Matrices = new int[NumOfMatrices];

    char *tmp = strtok(matrices, " ");
    Matrices[0] = atoi(tmp);
    for (int i = 1; i < NumOfMatrices; i++) {
        tmp = strtok(nullptr, " ");
        Matrices[i] = atoi(tmp);
        if (Matrices[i] < 0) parse_err();
        else if (Matrices[i] == 0 and strcmp(tmp, "0") != 0) parse_err();
    }

//    calc num of results
    for (uint64_t i = 1; i < strlen(results) + 1; i++) {
        if ((results[i - 1] < '0' or results[i - 1] > '9') and results[i - 1] != ' ' and
            results[i - 1] != '.')
            parse_err();
        if (results[i - 1] >= '0' and results[i - 1] <= '9' and (results[i] == '\0' or results[i] == ' '))
            NumOfResults++;
    }
//    alloc Results and set
    Results = new double[NumOfResults];

    tmp = strtok(results, " ");
    Results[0] = atof(tmp);
    for (int i = 1; i < NumOfResults; i++) {
        tmp = strtok(nullptr, " ");
        Results[i] = atof(tmp);
        if (Results[i] < 0) parse_err();
    }

//    calc num of predicates
    for (uint64_t i = 1; i < strlen(predicates) + 1; i++) {
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
    char *strArray[NumOfPredicates];

    strArray[0] = strtok(predicates, "&"); // reading the 1st predicate
    for (int i = 1; i < NumOfPredicates; i++)
        strArray[i] = strtok(nullptr, "&");

    for (int j = 0; j < NumOfPredicates; j++) {
        char operation = 'a';
        bool operationFound = false;

        tmp = strtok(strArray[j], "&");

        for (uint64_t i = 0; i < strlen(tmp); i++) {
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
        Predicates[j].Matrices[0] = Matrices[atoi(tmp)];
        Predicates[j].MatricesIndex[0] = atoi(tmp);
        tmp = strtok(nullptr, del);
        Predicates[j].RowIds[0] = atoi(tmp);
        if (operation == 'j') {
            tmp = strtok(nullptr, ".");
            Predicates[j].Matrices[1] = Matrices[atoi(tmp)];
            Predicates[j].MatricesIndex[1] = atoi(tmp);
            tmp = strtok(nullptr, "\0");
            Predicates[j].RowIds[1] = atoi(tmp);
        } else {
            tmp = strtok(nullptr, "\0");
            Predicates[j].filter = atoll(tmp);
        }
        Predicates[j].operation = operation;
    }
}

bool Query::filtering(uint64_t &size){
    //    calc num of filters
    uint64_t v = 0;
    for (int i = 0; i < NumOfPredicates; i++) {
        if (Predicates[i].filter) v++;
        if(Predicates[i].operation == 'x') v++;
    }

    for (int i = 0; i < NumOfPredicates; i++)
        if (Predicates[i].filter or Predicates[i].operation == 'x') {
            Relation *rel;


            rel = MATRICES[Predicates[i].Matrices[0]].getRelation(Predicates[i].RowIds[0]);
            for (int j = 0; j < NumOfMatrices; j++) {
                if (Predicates[i].Matrices[0] == Matrices[j]) {
                    rel->filter(FilteredMatrices[j]);
                    break;
                }
            }
            if (rel == nullptr)
                return false;

            auto vector = new Vector();

            Tuple *tuples = rel->getTuples();
            uint64_t numOfTuples = rel->numTuples;
            switch (Predicates[i].operation) {
                case '>': {
                    for (uint64_t j = 0; j < numOfTuples; j++)
                        if (tuples[j].key > Predicates[i].filter)
                            vector->push_back(tuples[j].getPayloads()[0]);
                    //updating stats
                    Stats *s = &stats[Predicates[i].MatricesIndex[0]][Predicates[i].RowIds[0]];
                    if(Predicates[i].filter+1 > s->I){
                        double fraction =(double)(s->U - Predicates[i].filter+1)/(double)(s->U - s->I);
                        s->I = Predicates[i].filter+1;
                        s->d = (uint64_t)(fraction*(double)s->d);
                        uint64_t f = s->f;
                        s->f = (uint64_t)(fraction*(double)s->f);
                        double fraction2 = (double)s->f/(double)f;
                        for(int j = 0; j<MATRICES[Predicates[i].Matrices[0]].numOfColumns;j++){
                            if(Predicates[i].RowIds[0] == j) continue;
                            Stats* s2 = &stats[Predicates[i].MatricesIndex[0]][j];
                            s2->d=(uint64_t)((double)s2->d*(1-pow(1-fraction2,(double)s2->f/(double)s2->d)));
                            s2->f = s->f;
                        }
                    }
                    break;
                }
                case '<': {
                    for (uint64_t j = 0; j < rel->numTuples; j++)
                        if (tuples[j].key < Predicates[i].filter)
                            vector->push_back(tuples[j].getPayloads()[0]);
                    //updating stats
                    Stats *s = &stats[Predicates[i].MatricesIndex[0]][Predicates[i].RowIds[0]];
                    if(Predicates[i].filter-1 < s->I){
                        double fraction =(double)(Predicates[i].filter-1-s->I)/(double)(s->U - s->I);
                        s->U = Predicates[i].filter-1;
                        s->d = (uint64_t)(fraction*(double)s->d);
                        uint64_t f = s->f;
                        s->f = (uint64_t)(fraction*(double)s->f);
                        double fraction2 = (double)s->f/(double)f;
                        for(int j = 0; j<MATRICES[Predicates[i].Matrices[0]].numOfColumns;j++){
                            if(Predicates[i].RowIds[0] == j) continue;
                            Stats* s2 = &stats[Predicates[i].MatricesIndex[0]][j];
                            s2->d=(uint64_t)((double)s2->d*(1-pow(1-fraction2,(double)s2->f/(double)s2->d)));
                            s2->f = s->f;
                        }
                    }
                    break;
                }
                case '=': {
                    for (uint64_t j = 0; j < rel->numTuples; j++)
                        if (tuples[j].key == Predicates[i].filter)
                            vector->push_back(tuples[j].getPayloads()[0]);
                    //updating stats
                    Stats *s = &stats[Predicates[i].MatricesIndex[0]][Predicates[i].RowIds[0]];
                    if(vector->size() > 0){
                        s->I = Predicates[i].filter;
                        s->U = Predicates[i].filter;
                        uint64_t f = s->f;
                        s->f = (uint64_t)((double)s->f/(double)s->d);
                        double fraction2 = (double)s->f/(double)f;
                        s->d = 1;
                        for(int j = 0; j<MATRICES[Predicates[i].Matrices[0]].numOfColumns;j++){
                            if(Predicates[i].RowIds[0] == j) continue;
                            Stats* s2 = &stats[Predicates[i].MatricesIndex[0]][j];
                            s2->d=(uint64_t)((double)s2->d*(1-pow(1-fraction2,(double)s2->f/(double)s2->d)));
                            s2->f = s->f;
                        }
                    }
                    break;
                }
                default:
                    std::cerr << "Invalid operation for filtering!" << std::endl;
                    return false;
            }
            FilteredMatrices[Predicates[i].MatricesIndex[0]] = vector;
            delete rel;
        }

    size = v;
    return true;
}

Relation* Query::FltrRel(uint64_t mat, uint64_t index,uint64_t rel)
{
    Relation *R = MATRICES[mat].getRelation(rel);
    if(FilteredMatrices[index] != nullptr)
        R->filter(FilteredMatrices[index]);

    return R;
}

bool Query::prev_predicate(int cur1, int cur2, int i)
{
    if (((Predicates[i-1].Matrices[0] == cur1 && Predicates[i-1].Matrices[1] == cur2))
                   || ((Predicates[i-1].Matrices[0] == cur2 && Predicates[i-1].Matrices[1] == cur1)))
            return true;
    return false;
}

void Query::rearrange_predicates()
{
    int c = 0;
    for (int i = 0; i < NumOfPredicates; i++)
        if (Predicates[i].operation == 'j') c++;

    if (c <= 2) return;

    for (int i = 0; i < NumOfPredicates; i++)
    {
        if (Predicates[i].operation != 'j') continue;
        for (int j = i+1; j < NumOfPredicates; j++)
        {
            if (Predicates[j].operation != 'j') continue;
            if ((Predicates[i].Matrices[0] == Predicates[j].Matrices[0] && Predicates[i].Matrices[1] == Predicates[j].Matrices[1])
                || (Predicates[i].Matrices[0] == Predicates[j].Matrices[1] && Predicates[i].Matrices[1] == Predicates[j].Matrices[0]))
            {
                if (j == i+1) continue;
                else
                {
                    int tmp = Predicates[j-1].Matrices[0];
                    Predicates[j-1].Matrices[0] = Predicates[j].Matrices[0];
                    Predicates[j].Matrices[0] = tmp;

                    tmp = Predicates[j-1].Matrices[1];
                    Predicates[j-1].Matrices[1] =  Predicates[j].Matrices[1];
                    Predicates[j].Matrices[1] = tmp;

                    tmp = Predicates[j-1].RowIds[0];
                    Predicates[j-1].RowIds[0] = Predicates[j].RowIds[0];
                    Predicates[j].RowIds[0] = tmp;

                    tmp = Predicates[j-1].RowIds[1];
                    Predicates[j-1].RowIds[1] = Predicates[j].RowIds[1];
                    Predicates[j].RowIds[1] = tmp;

                    tmp = Predicates[j-1].MatricesIndex[0];
                    Predicates[j-1].MatricesIndex[0] = Predicates[j].MatricesIndex[0];
                    Predicates[j].MatricesIndex[0] = tmp;

                    tmp = Predicates[j-1].MatricesIndex[1];
                    Predicates[j-1].MatricesIndex[1] = Predicates[j].MatricesIndex[1];
                    Predicates[j].MatricesIndex[1] = tmp;
                }
            }
        }
    }
}

void Query::exec()
{
    //get matrices stats
    for(int i = 0 ; i < NumOfMatrices; i++){
        stats[i] = new Stats[MATRICES[Matrices[i]].numOfColumns];
        for(int j=0; j<MATRICES[Matrices[i]].numOfColumns;j++){
            stats[i][j].copy(&MATRICES[Matrices[i]].stats[j]);
        }
    }

    uint64_t f = 0;
    bool filters = filtering(f);
    if (!filters)
    {
        empty_sum();
        return;
    }

    plan_predicates();

    rearrange_predicates();
    for (int i = 0; i < NumOfPredicates; i++)
    {
        if (Predicates[i].operation != 'j') continue;

        Relation *R1 = nullptr, *R2 = nullptr;
        if (MatricesJoined == nullptr)
        {
            MatricesJoined = new Vector();
            MatricesJoined->push_back(Predicates[i].MatricesIndex[0]);
            MatricesJoined->push_back(Predicates[i].MatricesIndex[1]);

            R1 = FltrRel(Predicates[i].Matrices[0],Predicates[i].MatricesIndex[0], Predicates[i].RowIds[0]);
            R2 = FltrRel(Predicates[i].Matrices[1],Predicates[i].MatricesIndex[1], Predicates[i].RowIds[1]);
            if (!R1->numTuples || !R2->numTuples) {
                empty_sum();
                delete R1;
                delete R2;
                return;
            }

            ListOfResults = join(R1, R2);

        }
        else if (MatricesJoined->search(Predicates[i].MatricesIndex[0]))
        {
            R1 = MATRICES[Predicates[i].Matrices[0]].getRelation(ListOfResults,MatricesJoined->getIndex(Predicates[i].MatricesIndex[0]),rowsInResults,Predicates[i].RowIds[0]);
            if (R1 == nullptr)
            {
                empty_sum();
                return;
            }

            if (!MatricesJoined->search(Predicates[i].MatricesIndex[1]))
            {
                delete ListOfResults;
                ListOfResults = nullptr;
                MatricesJoined->push_back(Predicates[i].MatricesIndex[1]);

                R2 = FltrRel(Predicates[i].Matrices[1],Predicates[i].MatricesIndex[1], Predicates[i].RowIds[1]);
                if (!R2->numTuples)
                {
                    empty_sum();
                    delete R1; delete R2;
                    return;
                }
                ListOfResults = join(R1, R2);
            }
            else
            {
                R2 = MATRICES[Predicates[i].Matrices[1]].getRelationKeys(ListOfResults,MatricesJoined->getIndex(Predicates[i].MatricesIndex[1]),rowsInResults,Predicates[i].RowIds[1]);
                if (R2 == nullptr)
                {
                    empty_sum();
                    delete  R1; delete R2;
                    return;
                }

                if (prev_predicate(Predicates[i].Matrices[0], Predicates[i].Matrices[1], i))
                    equality_filter(R1, R2);
                else
                {
                    std::cerr << "Should not reach this else!" << std::endl;
                    return;
                }
            }
        }
        else if (MatricesJoined->search(Predicates[i].MatricesIndex[1]))
        {
            R1 = MATRICES[Predicates[i].Matrices[1]].getRelation(ListOfResults,MatricesJoined->getIndex(Predicates[i].MatricesIndex[1]),rowsInResults,Predicates[i].RowIds[1]);
            if (R1 == nullptr)
            {
                empty_sum();
                return;
            }

            delete ListOfResults;
            ListOfResults = nullptr;
            MatricesJoined->push_back(Predicates[i].MatricesIndex[0]);
            R2 = FltrRel(Predicates[i].Matrices[0],Predicates[i].MatricesIndex[0], Predicates[i].RowIds[0]);
            if (!R2->numTuples)
            {
                empty_sum();
                delete R1; delete R2;
                return;
            }
            ListOfResults = join(R1, R2);
        }
        else
        {
            std::cerr << "Shouldn't reach here!" << std::endl;
            return;
        }

        if (ListOfResults == nullptr)
        {
            empty_sum();
            delete R1; delete R2;
            return;
        }
        delete R1; delete R2;
    }

    calc_sum();
}

void Query::equality_filter(Relation *r1, Relation *r2)
{
    clock_t start,end;
    start = clock();

    Tuple *tup1 = r1->getTuples();
    Tuple *tup2 = r2->getTuples();

    struct Node* j = nullptr;
    struct Node* n = ListOfResults->head;
    int i =0;
    while(n != nullptr)
    {
        if (tup1[i].key
            != tup2[i].key)
        {
            j = n;
            n = n->next;
            ListOfResults->remove_node(j);
            rowsInResults--;
        }
        else n = n->next;
        i++;
    }

    end = clock();
//    std::cout << "Equality filter part: " << ((double) end-start)/CLOCKS_PER_SEC << "s" << std::endl;
}

List* Query::join(Relation *relA, Relation *relB)
{
    clock_t start,end;

    start = clock();
    if(!relA->isSorted())
        Radixsort(relA,0,relA->numTuples-1);
    end = clock();
//    std::cout << "Radix 1: " << ((double) end-start)/CLOCKS_PER_SEC  << "s" << std::endl;

    start = clock();
    if(!relB->isSorted())
        Radixsort(relB,0,relB->numTuples-1);
    end = clock();
//    std::cout << "Radix 2: " << ((double) end-start)/CLOCKS_PER_SEC << "s" << std::endl;
    start = clock();

    if (!relA->isSorted() || !relB->isSorted()) return nullptr;

    Tuple* tupA = relA->getTuples();
    Tuple* tupB = relB->getTuples();

    if(tupA == nullptr or tupB == nullptr) return nullptr;

    uint64_t sizeA = relA->numTuples;
    uint64_t sizeB = relB->numTuples;
    uint64_t j=0;
    uint64_t jj=0;
    bool flag = false;
    uint64_t counter = 0;
    struct Node* N;

    List* results = new List();
    for(uint64_t i = 0; i<sizeA; i++){

        if(tupA[i].key == tupB[j].key){
            N = results->insert_node();
            for(int x =0; x < (int) tupA[i].payloads.size(); x++)
                results->insert(N,tupA[i].payloads[x]);
            results->insert(N,tupB[j].payloads[0]);
            counter++;

            if(j == sizeB-1) continue;
            jj = j;
            while(tupA[i].key == tupB[++j].key){
                N = results->insert_node();
                for(int x =0; x < (int) tupA[i].payloads.size(); x++)
                    results->insert(N,tupA[i].payloads[x]);
                results->insert(N,tupB[j].payloads[0]);
                counter++;
                if(j == sizeB-1) break;
            }
            j = jj;
        }
        else if(tupA[i].key > tupB[j].key){

            if(j == sizeB-1) break;
            while(tupA[i].key > tupB[++j].key){
                if (j == sizeB-1) {
                    flag = true;
                    break;
                }
            }
            if(flag) break;
            jj = j--;
            while(tupA[i].key == tupB[++j].key){
                N = results->insert_node();
                for(int x =0; x < (int) tupA[i].payloads.size(); x++)
                    results->insert(N,tupA[i].payloads[x]);
                results->insert(N,tupB[j].payloads[0]);
                counter++;
                if (j == sizeB-1) {
                    break;
                }
            }
            j = jj;
        }
    }
    end = clock();
//    std::cout << "Join part: " << ((double) end-start)/CLOCKS_PER_SEC << "s" << std::endl;

    if(!counter) return nullptr;
    rowsInResults = counter;
    return results;
}

int fracto_int(double number, int dec_num)
{
    double dummy;
    double frac = modf(number, &dummy);
    return round(frac * pow(10, dec_num));
}

void Query::empty_sum()
{
    std::cout << "RESULT:";
    for (int i = 0; i < NumOfResults; i++)
        std::cout << "NULL ";
    std::cout << std::endl;
}

void Query::calc_sum()
{
    clock_t start, end;
    start = clock();

    Vector sum;
    Tuple *data;
    for (int i = 0; i < NumOfResults; i++)
    {
        uint64_t s = 0;
        double frack, intprt;
        int x, y;
        frack = modf(Results[i], &intprt);
        x = (int) intprt;
        y = fracto_int(frack, 1);

        if (!MatricesJoined->search(x)) return;

        int indx = MatricesJoined->getIndex(x);
        if (indx != -1)
        {
            Relation *rel = MATRICES[Matrices[x]].getRelation(y);
            data = rel->getTuples();
            for (struct Node *h = ListOfResults->head; h != nullptr; h = h->next)
            {
                if (h->data[indx] > rel->numTuples) return;
                s += data[h->data[indx]].key;
            }
            sum.push_back(s);
            delete rel;
        }
        else return;
    }

    end = clock();
//    std::cout << "Calculation of sum part: " << ((double) end-start)/CLOCKS_PER_SEC << "s" << std::endl;

    std::cout << "RESULT:";
    for (uint64_t i = 0; i < sum.size(); i++) {
        std::cout << sum[i] << " ";
    }
    std::cout << std::endl;
}

void Query::plan_predicates() {
    int numOfJoins = 0;
    for(int i =0; i < NumOfPredicates; i++)
        if(Predicates[i].operation == 'j')
            numOfJoins++;

    auto* joins = new Predicate[numOfJoins];

    int c = 0;
    for(int i =0; i < NumOfPredicates; i++)
        if(Predicates[i].operation == 'j')
            joins[c++] = Predicates[i];

    auto* startingPrediction = new MatrixPrediction[NumOfMatrices];
    for(int i=0 ; i < NumOfMatrices;i++){
        Matrix* m = &MATRICES[Matrices[i]];
        startingPrediction[i].numOfColumns = m->numOfColumns;
        startingPrediction[i].matrixIndex = i;
        startingPrediction[i].stats = new Stats[m->numOfColumns];
        for(int j =0 ; j<startingPrediction[i].numOfColumns; j++)
            startingPrediction[i].stats[j].copy(&stats[i][j]);
    }

    int totalPermutations = 50;
    Prediction** predictions = new Prediction*[totalPermutations];
    c=0;

    for(int i=0; i<NumOfMatrices; i++){
        predictions[c] = new Prediction();
        predictions[c]->numOfMatrices = 1;
        predictions[c]->matrices = new MatrixPrediction[1];
        predictions[c]->matrices->copy(&startingPrediction[i]);
        c++;
    }

    for(int i =0; i<c; i++){
        if(predictions[i] == nullptr)continue;
        for(int j=0 ; j<numOfJoins; j++){
            if(predictions[i]->matrixInPrediction(joins[j].MatricesIndex[0])){
                if(predictions[i]->matrixInPrediction(joins[j].MatricesIndex[1])){
                    //self join or already joined
                }
                else{
                    //join with the other matrix
                    predictions[c++] = predictions[i]->JoinPrediction(predictions[joins[j].MatricesIndex[1]],
                    joins[j].MatricesIndex[0],joins[j].RowIds[0],joins[j].MatricesIndex[1],joins[j].RowIds[1],j);
                }
            }
            else if(predictions[i]->matrixInPrediction(joins[j].MatricesIndex[1])){
                predictions[c++] = predictions[i]->JoinPrediction(predictions[joins[j].MatricesIndex[0]],
                joins[j].MatricesIndex[1],joins[j].RowIds[1],joins[j].MatricesIndex[0],joins[j].RowIds[0],j);

            }
        }
    }

    for(int i =0; i<c; i++)
        if(predictions[i] != nullptr) predictions[i]->print();

    int* bestPredicateOrder = new int[numOfJoins];

    for(int i = numOfJoins-1; i > 0; i--){
        int cost = 0;
        Prediction* bestPrediction = nullptr;
        for( int j =NumOfMatrices; j< c; j++){
            if(predictions[j] == nullptr) continue;
            if(predictions[j]->numOfPredicates == i) {
                bool valid = true;
                for(int x = numOfJoins-1; x>i;x--){
                    if(predictions[j]->predicateInPrediction(bestPredicateOrder[x])){
                        valid = false;
                        break;
                    }
                }
                if(valid) {
                    if (cost == 0) {
                        cost = predictions[j]->getCost();
                        bestPrediction = predictions[j];
                    } else if (cost > predictions[j]->getCost()) {
                        cost = predictions[j]->getCost();
                        bestPrediction = predictions[j];
                    }
                }
            }
        }

        for(int j =0; j<numOfJoins; j++){
            bool missing = true;
            if(bestPrediction->predicateInPrediction(j)) continue;
            for(int x = numOfJoins-1; x>i;x--) {
                if(bestPredicateOrder[x] == j) {
                    missing = false;
                    break;
                }
            }
            if(missing)
                bestPredicateOrder[i] = j;
        }
    }
    for(int j =0; j<numOfJoins; j++){
        bool missing = true;
        for(int x = numOfJoins-1; x>0;x--) {
            if(bestPredicateOrder[x] == j) {
                missing = false;
                break;
            }
        }
        if(missing)
            bestPredicateOrder[0] = j;
    }



//    std::cout << "Best predicate order: " ;
//    for(int i =0; i<numOfJoins; i++){
//        std::cout << bestPredicateOrder[i] << " " ;
//    }
//    std::cout << std::endl;

    c=0;
    for(int i =0; i<numOfJoins;i++){
       for(int j =c; c<NumOfPredicates;i++){
           if(Predicates[c++].operation == 'j') {
               Predicates[c-1] = joins[i];
               break;
           }
       }
    }

}
