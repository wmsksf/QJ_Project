//
// Created by wmsksf on 12/01/20.
//

//TESTING
#include <iostream>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <vector>

#include <sstream>
class s_cout: public std::ostringstream
{
public:
    s_cout() = default;
    ~s_cout()
    {
        pthread_mutex_lock(&s_mux);
        std::cout << this->str();
        pthread_mutex_unlock(&s_mux);
    }

private:
    static pthread_mutex_t s_mux;
};
pthread_mutex_t s_cout::s_mux{};


class Job {
public:
    virtual void run() = 0;
    virtual ~Job() { }
};

class JobScheduler {
public:
    JobScheduler();
    ~JobScheduler();

    void schedule(Job &Job);

    const int ThreadPoolSize;
    std::vector<pthread_t> ThreadPool;
    std::vector<Job*> JobsReadyToRunQueue;
    std::queue<Job*> allJobsQueue;

    pthread_t ManagerThr;
    pthread_mutex_t ManagerMtx, WorkerMtx;
    pthread_cond_t ManagerCV, WorkerCV;

    bool WorkerIsRunning;
    bool ManagerIsRunning;
};

void* worker_thread(void*);
void* manager_thread(void*);

JobScheduler::JobScheduler() : ThreadPoolSize{16}, WorkerIsRunning{true}, ManagerIsRunning{true}
{
    pthread_t thread;
    for (int i = 0; i <ThreadPoolSize; ++i) {
        pthread_create(&thread,nullptr,worker_thread, (void*)this);
        ThreadPool.push_back(thread);
    }
    pthread_create(&ManagerThr,nullptr,manager_thread, (void*)this);

    pthread_mutex_init(&WorkerMtx,nullptr);
    pthread_cond_init(&WorkerCV,nullptr);
    pthread_mutex_init(&ManagerMtx,nullptr);
    pthread_cond_init(&ManagerCV,nullptr);
}

JobScheduler::~JobScheduler()
{
    s_cout{} << "~JobScheduler start\n";
    pthread_mutex_lock(&ManagerMtx);
    ManagerIsRunning = false;
    pthread_cond_signal(&ManagerCV);
    pthread_mutex_unlock(&ManagerMtx);
    pthread_join(ManagerThr, nullptr);
    s_cout{} << "cleaning of manager done\n";
    s_cout{} << "cleaning of workers...\n";
    pthread_mutex_lock(&WorkerMtx);
    WorkerIsRunning = false;
    pthread_cond_broadcast(&WorkerCV);
    s_cout{} << "broadcast...\n";
    pthread_mutex_unlock(&WorkerMtx);
    s_cout{} << "join...\n";
    for (auto &t : ThreadPool)
        pthread_join(t, nullptr);
    s_cout{} << "~JobScheduler end\n";
}

void JobScheduler::schedule(Job &Job)
{
    pthread_mutex_lock(&ManagerMtx);
    allJobsQueue.push(&Job);
    pthread_cond_signal(&ManagerCV);
    pthread_mutex_unlock(&ManagerMtx);
}

void* worker_thread(void *arg)
{
    auto* scheduler = (JobScheduler*)arg;
    for (;;)
    {
        pthread_mutex_lock(&(scheduler->WorkerMtx));

        while (!scheduler->JobsReadyToRunQueue.size() && scheduler->WorkerIsRunning) {
            pthread_cond_wait(&(scheduler->WorkerCV), &(scheduler->WorkerMtx));
        }
        if (!scheduler->WorkerIsRunning) {
            pthread_mutex_unlock(&(scheduler->WorkerMtx));
            break;
        }

        Job *p = scheduler->JobsReadyToRunQueue.back();
        scheduler->JobsReadyToRunQueue.pop_back();
        pthread_mutex_unlock(&(scheduler->WorkerMtx));

        p->run();
        delete p;
    }
}

void* manager_thread(void *arg)
{
    auto* scheduler = (JobScheduler*)arg;
    for (;;)
    {
        pthread_mutex_lock(&(scheduler->ManagerMtx));
        if (!scheduler->ManagerIsRunning) {
            pthread_mutex_unlock(&(scheduler->ManagerMtx));
            break;
        }

        if (scheduler->allJobsQueue.size() != 0)
        {
            Job *p = scheduler->allJobsQueue.front();

            pthread_mutex_lock(&(scheduler->WorkerMtx));
            scheduler->JobsReadyToRunQueue.push_back(p);
            pthread_cond_signal(&(scheduler->WorkerCV));
            pthread_mutex_unlock(&(scheduler->WorkerMtx));

            scheduler->allJobsQueue.pop();
        }
        pthread_mutex_unlock(&(scheduler->ManagerMtx));
    }
}

class job : public Job {
    int n;
public:
    job(int n=0) : n{n} { }
    void run() override
    {
        s_cout{} << "Start Job " << n << std::endl;;
        sleep(2);
        s_cout{} << " Stop Job " << n << std::endl;;
    }
};

int main()
{
    JobScheduler sched;

    Job *t0 = new job{0};
    Job *t1 = new job{1};
    Job *t2 = new job{2};
    Job *t3 = new job{3};
    Job *t4 = new job{4};
    Job *t5 = new job{5};

    sched.schedule(*t0);
    sched.schedule(*t1);
    sched.schedule(*t2);
    sched.schedule(*t3);
    sched.schedule(*t4);
    sched.schedule(*t5);

    sleep(10);
}