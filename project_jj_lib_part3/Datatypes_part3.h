//
// Created by karolos on 5/1/20.
//

#ifndef PROJECT_JJ_DATATYPES_PART3_H
#define PROJECT_JJ_DATATYPES_PART3_H

#include <pthread.h>
#include <cstdint>
#define MAX_DISTINCT_VALUES 49979693


// Abstract Class Job
class Job {
public:
    Job() = default;
    virtual ~Job() {}
// This method should be implemented by subclasses.
    virtual int Run() = 0;
};


// Class JobScheduler
class JobScheduler {
public:
    JobScheduler() = default;
    ~JobScheduler() = default;
// Initializes the JobScheduler with the number of open threads.
// Returns true if everything done right false else.
    bool Init(int num_of_threads);
// Free all resources that are allocated by JobScheduler
// Returns true if everything done right false else.
    bool Destroy();
// Waits Until executed all jobs in the queue.
    void Barrier();
// Add a job in the queue and returns a JobId
   // JobID Schedule(Job* job);
// Waits until all threads finish their job, and after that close all threads.
    void Stop();


    //variables
    pthread_mutex_t mutex;
    pthread_cond_t cond_signal;
    Job* queue;
    int queueSize;
    int numOfThreads;
    pthread_t* threads;
    int totalJobs;
    int jobsCompleted;

};

void* thread_work(void*);

class Stats{
public:
    uint64_t I, U, f, d; // min, max, # of rows, # of unique values

    void copy(Stats*);
};

class MatrixPrediction{
public:
    uint64_t matrixIndex;
    uint64_t numOfColumns;
    Stats* stats = nullptr;

    void copy(MatrixPrediction* p2);
    ~MatrixPrediction();

};

class Prediction{
public:
    uint64_t numOfMatrices;
    MatrixPrediction* matrices = nullptr;

    Prediction* JoinPrediction(Prediction* predB,uint64_t indexA,uint64_t colA,uint64_t indexB,uint64_t colB);
    ~Prediction();
    bool matrixInPrediction(int index);
    void print();
};

#endif //PROJECT_JJ_DATATYPES_PART3_H
