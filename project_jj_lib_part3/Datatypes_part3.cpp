//
// Created by karolos on 5/1/20.
//

#include <cstdio>
#include <cmath>
#include <iostream>
#include "Datatypes_part3.h"

bool JobScheduler::Init(int num_of_threads) {

    if(num_of_threads < 1) return false;

    //Initialize variables
    queueSize = 0;
    num_of_threads = num_of_threads;
    totalJobs = 0;
    jobsCompleted = 0;
    pthread_mutex_init(&mutex,nullptr);
    pthread_cond_init(&cond_signal,nullptr);

    // Create threads
    threads = new pthread_t[num_of_threads];

    for(int i = 0; i < num_of_threads; i++){
        if(pthread_create(&threads[i],nullptr,thread_work,(void*)this) != 0){
            perror("Error creating thread\n");
            return false;
        }
    }



    return true;
}

bool JobScheduler::Destroy() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_signal);

    delete[] threads;
}

void* thread_work(void* scheduler_){
    auto* scheduler = (JobScheduler*)scheduler_;

    while(true){
        //get a job to execute...

        break;
    }

    pthread_exit(nullptr);
}

Prediction *Prediction::JoinPrediction(Prediction *predB,uint64_t indexA,uint64_t colA,uint64_t indexB,uint64_t colB) {
    if(predB== nullptr) return nullptr;

    auto* newPr = new Prediction();
    newPr->numOfMatrices = this->numOfMatrices + predB->numOfMatrices;
    newPr->matrices = new MatrixPrediction[numOfMatrices];
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

    //First update the stats from the 2 columns being joined
    Stats* s1 = &A->stats[colA];
    Stats* s2 = &B->stats[colB];

    if(s1->I > s2->I) s2->I = s1->I;
    else s1->I = s2->I;

    if(s1->U < s2->U) s2->U = s1->U;
    else s1->U = s2->U;

    uint64_t n = s1->U - s1->I + 1;

    s1->f = s2->f = (s1->f*s2->f)/n;

    double fraction1 = s1->d;
    double fraction2 = s2->d;

    s1->d = s2->d = (s1->d*s2->d)/n;

    fraction1 = (double)s1->d/fraction1;
    fraction2 = (double)s2->d/fraction2;

    //update the rest
    //Rest from matrixPrediction A
    for(uint64_t i =0; i<A->numOfColumns;i++){
        if(i == colA) continue;

        Stats* s = &A->stats[i];

        s->d = (uint64_t)((double)s->d*(1-pow(1-fraction1,(double)s->f/(double)s->d)));

        s->f = s1->f;
    }
    //Rest from matrixPrediction B
    for(uint64_t i =0; i<B->numOfColumns;i++){
        if(i == colB) continue;

        Stats* s = &B->stats[i];

        s->d = (uint64_t)((double)s->d*(1-pow(1-fraction2,(double)s->f/(double)s->d)));

        s->f = s2->f;
    }

    for(uint64_t j=0; j< this->numOfMatrices;j++){
        if(newPr->matrices[j].matrixIndex == indexA) continue;

        A = &newPr->matrices[j];
        for(uint64_t i =0; i<A->numOfColumns;i++){
            Stats* s = &A->stats[i];

            s->d = (uint64_t)((double)s->d*(1-pow(1-fraction1,(double)s->f/(double)s->d)));

            s->f = s1->f;
        }
    }
    for(uint64_t j=this->numOfMatrices; j < newPr->numOfMatrices;j++){
        if(newPr->matrices[j].matrixIndex == indexB) continue;

        B = &newPr->matrices[j];
        for(uint64_t i =0; i<B->numOfColumns;i++){
            Stats* s = &B->stats[i];

            s->d = (uint64_t)((double)s->d*(1-pow(1-fraction2,(double)s->f/(double)s->d)));

            s->f = s2->f;
        }
    }
    return newPr;
}

Prediction::~Prediction() {
    delete[] matrices;
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
