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

    pthread_mutex_t WorkerMtx;
    pthread_cond_t WorkerCV;

    bool WorkerIsRunning;
};

void* worker_thread(void*);

class job : public Job {
    int n;
public:
    job(int n=0);
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

// from Little Book of Semaphores
// typedef struct {
//   int n;
//   int count;
//   sem_t mutex;
//   sem_t turnstile;
//   sem_t turnstile2;
// } barrier_t;

// void init_barrier(barrier_t *barrier, int n)
// {
//   barrier->n = n;
//   barrier->count = 0;
//   sem_init(&barrier->mutex, 0, 1);
//   sem_init(&barrier->turnstile, 0, 0);
//   sem_init(&barrier->turnstile2, 0, 0);
// }

// void phase1_barrier(barrier_t *barrier)
// {
//   sem_wait(&barrier->mutex);
//   if (++barrier->count == barrier->n) {
//     int i;
//     for (i = 0; i < barrier->n; i++) {
//       sem_post(&barrier->turnstile);
//     }
//   }
//   sem_post(&barrier->mutex);
//   sem_wait(&barrier->turnstile);
// }

// void phase2_barrier(barrier_t *barrier)
// {
//   sem_wait(&barrier->mutex);
//   if (--barrier->count == 0) {
//     int i;
//     for (i = 0; i < barrier->n; i++) {
//       sem_post(&barrier->turnstile2);
//     }
//   }
//   sem_post(&barrier->mutex);
//   sem_wait(&barrier->turnstile2);
// }

// void wait_barrier(barrier_t *barrier)
// {
//   phase1_barrier(barrier);
//   phase2_barrier(barrier);
// }

#endif //PROJECT_JJ_JOBSCHEDULER_H
