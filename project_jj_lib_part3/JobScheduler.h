//
// Created by wmsksf on 14/01/20.
//

#ifndef PROJECT_JJ_JOBSCHEDULER_H
#define PROJECT_JJ_JOBSCHEDULER_H

#include <pthread.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <queue>

#include "../project_jj_lib_part2/Datatypes_part2.h"

typedef pthread_t JOBID;

class Job {
public:
    virtual void run() = 0;
    virtual ~Job() { }
};

class JobScheduler {
public:
    JobScheduler();
    ~JobScheduler();

    void init(int threads);
    void stop();

    void schedule(Job &Job);
    void barrier();

    int ThreadPoolSize;
    std::vector<pthread_t> ThreadPool;
    std::queue<Job*> JobsQueue;
//    std::vector<class buffer_data> buffer;

    pthread_mutex_t WorkerMtx, mtx;
    pthread_cond_t WorkerCV, cv;

    bool WorkerIsRunning;
    int JobsToRun, JobsCompleted;
};

void* worker_thread(void*);

class queryJob : public Job {
    Query q;
public:
    queryJob(char *inq);
    ~queryJob();
    void run() override;
};

class sortJob : public Job {
    Relation *R, *RR;
    uint64_t start, end, current_byte;
//    Tuple* A;
//    uint64_t lo, hi;
    bool quick;

public:
    sortJob(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte, Relation* RR);
//    sortJob(Tuple* A, uint64_t lo, uint64_t hi);
    ~sortJob();
    void run() override;
};

//----------------------------------------------------------------------------------------------------------------------

class s_cout: public std::ostringstream
{
public:
    s_cout() = default;
    ~s_cout() override;

private:
    static pthread_mutex_t s_mux;
};

//class buffer_data {
//public:
//    JOBID job_id;
//    Vector sum;
//};

extern JobScheduler job_scheduler;

#endif //PROJECT_JJ_JOBSCHEDULER_H
