//
// Created by wmsksf on 14/01/20.
//

#include "JobScheduler.h"

JobScheduler::JobScheduler() : WorkerIsRunning{true}
{
    pthread_mutex_init(&WorkerMtx,nullptr);
    pthread_cond_init(&WorkerCV,nullptr);
    pthread_mutex_init(&IdleMtx,nullptr);
    pthread_cond_init(&IdleCV,nullptr);
}

JobScheduler::~JobScheduler()
{
    s_cout{} << "~JobScheduler...\n";

    pthread_mutex_destroy(&WorkerMtx);
    pthread_cond_destroy(&WorkerCV);
    pthread_cond_destroy(&IdleCV);
    pthread_mutex_destroy(&IdleMtx);

    s_cout{} << "~JobScheduler.\n";
}

void JobScheduler::init(int threads)
{
    ThreadPoolSize = threads;
    for (int i = 0; i < ThreadPoolSize; ++i){
        ThreadPoolIdle.push_back(true);
        args arg(this, i);
        tmp.push_back(arg);
    }

    pthread_t thread;
    for (int i = 0; i <ThreadPoolSize; ++i) {
        pthread_create(&thread,nullptr,worker_thread, (void*)&(tmp[i]));
        ThreadPool.push_back(thread);
    }

    tmp.clear();
}

void JobScheduler::stop()
{
    s_cout{} << "stop...\n";

    pthread_mutex_lock(&WorkerMtx);
    WorkerIsRunning = false;
    pthread_cond_broadcast(&WorkerCV);
    s_cout{} << "broadcast...\n";
    pthread_mutex_unlock(&WorkerMtx);
    s_cout{} << "join...\n";
    for (auto &t : ThreadPool)
        pthread_join(t, nullptr);

    s_cout{} << "stop.\n";
}

void JobScheduler::schedule(Job &Job)
{
    pthread_mutex_lock(&WorkerMtx);
    JobsQueue.push(&Job);
    pthread_cond_signal(&WorkerCV);
    pthread_mutex_unlock(&WorkerMtx);
}

bool JobScheduler::allIdle()
{
    for (int i = 0; i < ThreadPoolSize; ++i) {
        if (!ThreadPoolIdle[i]) return false;
    }
    return true;
}

void JobScheduler::barrier()
{
    s_cout{} << "barrier...\n";

    pthread_mutex_lock(&IdleMtx);
    while(allIdle()) {
        pthread_cond_wait(&IdleCV, &IdleMtx);
    }
    pthread_mutex_unlock(&IdleMtx);

    s_cout{} << "barrier.\n";

}

void* worker_thread(void *arg)
{
    auto* argm = (args*) arg;
    auto* scheduler = (JobScheduler*)argm->scheduler;
    auto i = argm->i;

    for (;;)
    {
        pthread_mutex_lock(&(scheduler->WorkerMtx));

        while (!scheduler->JobsQueue.size() && scheduler->WorkerIsRunning) {
            pthread_cond_wait(&(scheduler->WorkerCV), &(scheduler->WorkerMtx));
        }
        if (!scheduler->WorkerIsRunning && !scheduler->JobsQueue.size()) {
            pthread_mutex_unlock(&(scheduler->WorkerMtx));
            break;
        } else if (!scheduler->JobsQueue.size()) {
            pthread_mutex_lock(&(scheduler->IdleMtx));
            scheduler->ThreadPoolIdle[i] = true;
            pthread_cond_signal(&(scheduler->IdleCV));
            pthread_mutex_unlock(&(scheduler->IdleMtx));
        }

        pthread_mutex_lock(&(scheduler->IdleMtx));
        scheduler->ThreadPoolIdle[i] = false;
        pthread_mutex_unlock(&(scheduler->IdleMtx));

        Job *p = scheduler->JobsQueue.front();
        scheduler->JobsQueue.pop();
        pthread_mutex_unlock(&(scheduler->WorkerMtx));

        p->run();
        delete p;
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

args::args(JobScheduler *scheduler_, int i_) : scheduler{scheduler_}, i{i_}
{}