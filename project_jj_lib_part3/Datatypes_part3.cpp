//
// Created by karolos on 5/1/20.
//

#include <cstdio>
#include <cmath>
#include <iostream>
#include "Datatypes_part3.h"

Prediction *Prediction::JoinPrediction(Prediction *predB,uint64_t indexA,uint64_t colA,uint64_t indexB,uint64_t colB, int predicateIndex) {
    if(predB== nullptr) return nullptr;

    auto* newPr = new Prediction();
    newPr->numOfMatrices = this->numOfMatrices + predB->numOfMatrices;
    newPr->matrices = new MatrixPrediction[newPr->numOfMatrices];
    newPr->numOfPredicates = this->numOfPredicates + predB->numOfPredicates + 1;
    newPr->predicateOrder = new int[newPr->numOfPredicates];
    for(uint64_t  i =0; i < this->numOfPredicates; i++)
        newPr->predicateOrder[i] = this->predicateOrder[i];
    newPr->predicateOrder[newPr->numOfPredicates-1] = predicateIndex;

    //copy matricesPredictions data
    for(uint64_t i=0; i< this->numOfMatrices;i++)
        newPr->matrices[i].copy(&this->matrices[i]);
    for(uint64_t i=this->numOfMatrices,j=0; i < newPr->numOfMatrices;i++)
        newPr->matrices[i].copy(&predB->matrices[j++]);

    MatrixPrediction* A = nullptr, *B = nullptr;
    for(uint64_t i=0; i< newPr->numOfMatrices;i++){
        if(newPr->matrices[i].matrixIndex == indexA)
            A = &newPr->matrices[i];
        if(newPr->matrices[i].matrixIndex == indexB)
            B = &newPr->matrices[i];
    }

    if(A == nullptr or B == nullptr){
        delete newPr;
        return nullptr;
    }

    if(!(indexA == indexB and colA == colB)) {
        //First update the stats from the 2 columns being joined
        Stats* s1 = &A->stats[colA];
        Stats* s2 = &B->stats[colB];

        if (s1->I > s2->I) s2->I = s1->I;
        else s1->I = s2->I;

        if (s1->U < s2->U) s2->U = s1->U;
        else s1->U = s2->U;

        uint64_t n = s1->U - s1->I + 1;

        double fraction1 = s1->f;
        double fraction2 = s2->f;

        s1->f = s2->f = (s1->f * s2->f) / n;

        if (s1->f == 0) {
            delete newPr;
            return nullptr;
        }

        if (fraction1 != 0) fraction1 = (double) s1->f / fraction1;
        if (fraction2 != 0) fraction2 = (double) s2->f / fraction2;

        s1->d = s2->d = (s1->d * s2->d) / n;


        //update the rest
        //Rest from matrixPrediction A
        for (uint64_t i = 0; i < A->numOfColumns; i++) {
            if (i == colA) continue;

            Stats *s = &A->stats[i];

            s->d = (uint64_t) ((double) s->d * (1 - pow(1 - fraction1, (double) s->f / (double) s->d)));

            s->f = s1->f;
        }
        //Rest from matrixPrediction B
        for (uint64_t i = 0; i < B->numOfColumns; i++) {
            if (i == colB) continue;

            Stats *s = &B->stats[i];

            s->d = (uint64_t) ((double) s->d * ((double) 1.0 - pow(1.0 - fraction2, (double) s->f / (double) s->d)));

            s->f = s2->f;
        }

        for (uint64_t j = 0; j < this->numOfMatrices; j++) {
            if (newPr->matrices[j].matrixIndex == indexA) continue;

            A = &newPr->matrices[j];
            for (uint64_t i = 0; i < A->numOfColumns; i++) {
                Stats *s = &A->stats[i];

                s->d = (uint64_t) ((double) s->d * (1.0 - pow(1.0 - fraction1, (double) s->f / (double) s->d)));

                s->f = s1->f;
            }
        }
        for (uint64_t j = this->numOfMatrices; j < newPr->numOfMatrices; j++) {
            if (newPr->matrices[j].matrixIndex == indexB) continue;

            B = &newPr->matrices[j];
            for (uint64_t i = 0; i < B->numOfColumns; i++) {
                Stats *s = &B->stats[i];

                s->d = (uint64_t) ((double) s->d * (1.0 - pow(1.0 - fraction2, (double) s->f / (double) s->d)));

                s->f = s2->f;
            }
        }
    }
    else{   //self join
        std::cout << "Self join" << std::endl;
        Stats* s1 = &A->stats[colA];

        uint64_t n = s1->U - s1->I + 1;

        double fraction1 = s1->f;

        s1->f  = (s1->f * s1->f) / n;

        if (s1->f == 0) {
            delete newPr;
            return nullptr;
        }

        if (fraction1 != 0) fraction1 = (double) s1->f / fraction1;


        //update the rest
        //Rest from matrixPrediction A
        for (uint64_t i = 0; i < A->numOfColumns; i++) {
            if (i == colA) continue;

            Stats *s = &A->stats[i];


            s->f = s1->f;
        }

        for (uint64_t j = 0; j < this->numOfMatrices; j++) {
            if (newPr->matrices[j].matrixIndex == indexA) continue;

            A = &newPr->matrices[j];
            for (uint64_t i = 0; i < A->numOfColumns; i++) {
                Stats *s = &A->stats[i];

                s->d = (uint64_t) ((double) s->d * (1.0 - pow(1.0 - fraction1, (double) s->f / (double) s->d)));

                s->f = s1->f;
            }
        }
    }
    return newPr;
}

Prediction::~Prediction() {
    delete[] matrices;
    delete[] predicateOrder;
}

bool Prediction::matrixInPrediction(int index) {
    for(int i =0; i<numOfMatrices; i++)
        if(matrices[i].matrixIndex == index)
            return true;
    return false;
}

void Prediction::print() {
    std::cout << "Prediction: " ;
    for(int i =0; i < numOfMatrices; i++)
        std::cout << matrices[i].matrixIndex << " " ;

    std::cout <<": " << matrices[0].stats[0].f << std::endl;
}

int Prediction::getCost() {
    if(numOfMatrices==0) return 0;
    else return matrices[0].stats[0].f;
}

bool Prediction::predicateInPrediction(int predicate) {
    for(int i=0; i<numOfPredicates;i++)
        if(predicateOrder[i]==predicate)
            return true;
    return false;
}

Prediction *Prediction::EqualityFilterPrediction(uint64_t indexA,uint64_t colA,uint64_t indexB,uint64_t colB, int predicateIndex) {

    auto* newPr = new Prediction();
    newPr->numOfMatrices = this->numOfMatrices;
    newPr->matrices = new MatrixPrediction[newPr->numOfMatrices];
    newPr->numOfPredicates = this->numOfPredicates +1;
    newPr->predicateOrder = new int[newPr->numOfPredicates];
    for(uint64_t  i =0; i < this->numOfPredicates; i++)
        newPr->predicateOrder[i] = this->predicateOrder[i];
    newPr->predicateOrder[newPr->numOfPredicates-1] = predicateIndex;

    //copy matricesPredictions data
    for(uint64_t i=0; i< this->numOfMatrices;i++)
        newPr->matrices[i].copy(&this->matrices[i]);

    MatrixPrediction* A = nullptr, *B = nullptr;
    for(uint64_t i=0; i< newPr->numOfMatrices;i++){
        if(newPr->matrices[i].matrixIndex == indexA)
            A = &newPr->matrices[i];
        if(newPr->matrices[i].matrixIndex == indexB)
            B = &newPr->matrices[i];
    }

    if(A == nullptr or B == nullptr){
        delete newPr;
        return nullptr;
    }
    //First update the stats from the 2 columns being joined
    Stats* s1 = &A->stats[colA];
    Stats* s2 = &B->stats[colB];

    if (s1->I > s2->I) s2->I = s1->I;
    else s1->I = s2->I;

    if (s1->U < s2->U) s2->U = s1->U;
    else s1->U = s2->U;

    uint64_t n = s1->U - s1->I + 1;

    double fraction1 = s1->f;

    s1->f = s2->f = s1->f/n;

    if (s1->f == 0) {
        delete newPr;
        return nullptr;
    }

    if (fraction1 != 0) fraction1 = (double) s1->f / fraction1;

    s1->d = s2->d = (uint64_t) ((double) s1->d * (1 - pow(1 - fraction1, (double) s1->f / (double) s1->d)));


    //update the rest
    //Rest from matrixPrediction A
    for (uint64_t i = 0; i < A->numOfColumns; i++) {
        if (i == colA) continue;

        Stats *s = &A->stats[i];

        s->d = (uint64_t) ((double) s->d * (1 - pow(1 - fraction1, (double) s->f / (double) s->d)));

        s->f = s1->f;
    }
    //Rest from matrixPrediction B
    for (uint64_t i = 0; i < B->numOfColumns; i++) {
        if (i == colB) continue;

        Stats *s = &B->stats[i];

        s->d = (uint64_t) ((double) s->d * ((double) 1.0 - pow(1.0 - fraction1, (double) s->f / (double) s->d)));

        s->f = s2->f;
    }

    for (uint64_t j = 0; j < this->numOfMatrices; j++) {
        if (newPr->matrices[j].matrixIndex == indexA) continue;

        A = &newPr->matrices[j];
        for (uint64_t i = 0; i < A->numOfColumns; i++) {
            Stats *s = &A->stats[i];

            s->d = (uint64_t) ((double) s->d * (1.0 - pow(1.0 - fraction1, (double) s->f / (double) s->d)));

            s->f = s1->f;
        }
    }
    for (uint64_t j = this->numOfMatrices; j < newPr->numOfMatrices; j++) {
        if (newPr->matrices[j].matrixIndex == indexB) continue;

        B = &newPr->matrices[j];
        for (uint64_t i = 0; i < B->numOfColumns; i++) {
            Stats *s = &B->stats[i];

            s->d = (uint64_t) ((double) s->d * (1.0 - pow(1.0 - fraction1, (double) s->f / (double) s->d)));

            s->f = s2->f;
        }
    }

}

void MatrixPrediction::copy( MatrixPrediction* p2) {
    matrixIndex = p2->matrixIndex;
    numOfColumns = p2->numOfColumns;
    stats = new Stats[numOfColumns];
    for(uint64_t i =0; i<numOfColumns; i++)
        stats[i].copy(&p2->stats[i]);

}

MatrixPrediction::~MatrixPrediction() {
    delete[] stats;
}

//Stats::Stats(const Stats &s2) {
//    I = s2.I;
//    U = s2.U;
//    f = s2.f;
//    d = s2.d;
//}

void Stats::copy(Stats* s2) {
    I = s2->I;
    U = s2->U;
    f = s2->f;
    d = s2->d;
}
