//
// Created by wmsksf on 14/01/20.
//

#include "JobScheduler.h"

JobScheduler::JobScheduler() : WorkerIsRunning{true}, JobsToRun{0}, JobsCompleted{0}
{
    pthread_mutex_init(&WorkerMtx,nullptr);
    pthread_cond_init(&WorkerCV,nullptr);

    pthread_mutex_init(&mtx,nullptr);
    pthread_cond_init(&cv,nullptr);
}

JobScheduler::~JobScheduler()
{
    s_cout{} << "~JobScheduler...\n";

    pthread_mutex_destroy(&WorkerMtx);
    pthread_cond_destroy(&WorkerCV);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cv);

    s_cout{} << "~JobScheduler.\n";
}

void JobScheduler::init(int threads)
{
    ThreadPoolSize = threads;
    pthread_t thread;
    for (int i = 0; i <ThreadPoolSize; ++i) {
        pthread_create(&thread,nullptr,worker_thread, (void*)this);
        ThreadPool.push_back(thread);
    }
}

void JobScheduler::stop()
{
    s_cout{} << "stop...\n";

    pthread_mutex_lock(&WorkerMtx);
    WorkerIsRunning = false;

    s_cout{} << "broadcast...\n";
    pthread_cond_broadcast(&WorkerCV);
    pthread_mutex_unlock(&WorkerMtx);

    s_cout{} << "join...\n";
    for (auto &t : ThreadPool)
        pthread_join(t, nullptr);

    s_cout{} << "stop.\n";
}

void JobScheduler::schedule(Job &Job)
{
    pthread_mutex_lock(&WorkerMtx);

    pthread_mutex_lock(&mtx);
    JobsToRun++;
    pthread_mutex_unlock(&mtx);

    JobsQueue.push(&Job);
    pthread_cond_signal(&WorkerCV);
    pthread_mutex_unlock(&WorkerMtx);
}

void JobScheduler::barrier()
{
    s_cout{} << "barrier...\n";

    pthread_mutex_lock(&mtx);
    while(JobsToRun-JobsCompleted != 0) {
        pthread_cond_wait(&cv, &mtx);
    }

    pthread_mutex_unlock(&mtx);

    s_cout{} << "barrier.\n";

}

void* worker_thread(void *arg)
{
    auto* scheduler = (JobScheduler*)arg;
    for (;;)
    {
        pthread_mutex_lock(&(scheduler->WorkerMtx));

        while (!scheduler->JobsQueue.size() && scheduler->WorkerIsRunning) {
            pthread_cond_wait(&(scheduler->WorkerCV), &(scheduler->WorkerMtx));
        }
        if (!scheduler->WorkerIsRunning && !scheduler->JobsQueue.size()) {
            pthread_mutex_unlock(&(scheduler->WorkerMtx));
            break;
        }

        Job *p = scheduler->JobsQueue.front();
        scheduler->JobsQueue.pop();
        pthread_mutex_unlock(&(scheduler->WorkerMtx));

        p->run();
        delete p;

        pthread_mutex_lock(&(scheduler->mtx));
        (scheduler->JobsCompleted)++;
        if (!scheduler->JobsCompleted - scheduler->JobsToRun) {
            pthread_cond_signal(&(scheduler->cv));
        }
        pthread_mutex_unlock(&(scheduler->mtx));
    }
}

job::job(int n) : n{n} { }
void job::run()
{
    s_cout{} << "Start Job " << n << std::endl;
    sleep(2);
    s_cout{} << " Stop Job " << n << std::endl;
}
//----------------------------------------------------------------------------------------------------------------------
pthread_mutex_t s_cout::s_mux{};
s_cout::~s_cout()
{
    pthread_mutex_lock(&s_mux);
    std::cout << this->str();
    pthread_mutex_unlock(&s_mux);
}