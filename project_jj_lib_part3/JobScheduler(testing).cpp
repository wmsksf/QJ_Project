//
// Created by wmsksf on 12/01/20.
//

#include <iostream>
#include <unistd.h>
#include <vector>
#include <pthread.h>

//TESTING
#include <iostream>
#include <unistd.h>
#include <vector>
#include <pthread.h>

#include <sstream>
/** Thread safe cout class
  * Exemple of use:
  *    s_cout{} << "Hello world!" << std::endl;
  */
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
    virtual ~Job() {}
};

class JobScheduler {
public:
    JobScheduler();
    ~JobScheduler();

    void schedule(Job &job);
    void barrier();

    pthread_mutex_t WorkerMux;
    pthread_cond_t WorkerCond;

    const int ThreadPoolSize;
    std::vector<pthread_t> ThreadPool;
    std::vector<Job*> JobsReadyToRunQueue;
};

void* worker_thread(void*);

// get max #threads of h/w
JobScheduler::JobScheduler() : ThreadPoolSize{4}
{
    pthread_t thread;
    for (int i = 0; i <ThreadPoolSize; ++i) {
        pthread_create(&thread,nullptr,worker_thread, (void*)this);
        ThreadPool.push_back(thread);
    }

    pthread_mutex_init(&WorkerMux,nullptr);
    pthread_cond_init(&WorkerCond,nullptr);
}

JobScheduler::~JobScheduler()
{
    s_cout{} << "~JobScheduler start\n";
    pthread_mutex_lock(&WorkerMux);
    // in barrier!
    // in order to wake up waiting threads so as cond to be destroyed
    // pthread_cond_broadcast(&WorkerCond);
    for (auto &t : ThreadPool)
        pthread_join(t, nullptr);
    pthread_cond_destroy(&WorkerCond);

    pthread_mutex_unlock(&WorkerMux);
    pthread_mutex_destroy(&WorkerMux);
    std::cout << "~JobScheduler end\n";
}

void JobScheduler::schedule(Job &Job)
{
    pthread_mutex_lock(&WorkerMux);

    JobsReadyToRunQueue.push_back(&Job);
    pthread_cond_signal(&WorkerCond);

    pthread_mutex_unlock(&WorkerMux);
}

void JobScheduler::barrier()
{

}

void* worker_thread(void *arg)
{
    auto* scheduler = (JobScheduler*)arg;
    for (;;)
    {
        pthread_mutex_lock(&(scheduler->WorkerMux));

        while(!scheduler->JobsReadyToRunQueue.size())
        {
            pthread_cond_wait(&(scheduler->WorkerCond), &(scheduler->WorkerMux));
        }
        Job *p = scheduler->JobsReadyToRunQueue.front();
        scheduler->JobsReadyToRunQueue.erase(scheduler->JobsReadyToRunQueue.begin());

        pthread_mutex_unlock(&(scheduler->WorkerMux));

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
        s_cout{} << "Start of job " << n << std::endl;;
        sleep(2);
        s_cout{} << "End of job " << n << std::endl;;
    }
};

int main()
{
    JobScheduler scheduler;

    Job *t0 = new job{0};
    Job *t1 = new job{1};
    Job *t2 = new job{2};
    Job *t3 = new job{3};
    Job *t4 = new job{4};
    Job *t5 = new job{5};

    scheduler.schedule(*t0);
    scheduler.schedule(*t1);
    scheduler.schedule(*t2);
    scheduler.schedule(*t3);
    scheduler.schedule(*t4);
    scheduler.schedule(*t5);

    sleep(20);
}