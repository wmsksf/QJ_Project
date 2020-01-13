//
// Created by wmsksf on 12/01/20.
//

//TESTING
#include <iostream>
#include <queue>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
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
    void schedule(Job &Job);
    void barrier();

    int ThreadPoolSize;
    std::vector<pthread_t> ThreadPool;
    std::queue<Job*> JobsQueue;

    pthread_mutex_t WorkerMtx;
    pthread_cond_t WorkerCV;

    bool WorkerIsRunning;

    // barrier_t barrier_var;
};

void* worker_thread(void*);
void* manager_thread(void*);

JobScheduler::JobScheduler() : WorkerIsRunning{true}
{
    pthread_mutex_init(&WorkerMtx,nullptr);
    pthread_cond_init(&WorkerCV,nullptr);
}

JobScheduler::~JobScheduler()
{
    s_cout{} << "~JobScheduler start\n";
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

void JobScheduler::init(int threads)
{
    ThreadPoolSize = threads;
    pthread_t thread;
    for (int i = 0; i <ThreadPoolSize; ++i) {
        pthread_create(&thread,nullptr,worker_thread, (void*)this);
        ThreadPool.push_back(thread);
    }
}

void JobScheduler::schedule(Job &Job)
{
    pthread_mutex_lock(&WorkerMtx);
    JobsQueue.push(&Job);
    pthread_cond_signal(&WorkerCV);
    pthread_mutex_unlock(&WorkerMtx);
}

void JobScheduler::barrier()
{

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
        if (!scheduler->WorkerIsRunning) {
            pthread_mutex_unlock(&(scheduler->WorkerMtx));
            break;
        }

        Job *p = scheduler->JobsQueue.front();
        scheduler->JobsQueue.pop();
        pthread_mutex_unlock(&(scheduler->WorkerMtx));

        p->run();
        delete p;
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

int main(int argc, char **argv)
{
    if (argc < 2) {
        s_cout{} << "give #threads...\n";
        exit(1);
    }
    JobScheduler sched;
    sched.init(atoi(argv[1]));

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