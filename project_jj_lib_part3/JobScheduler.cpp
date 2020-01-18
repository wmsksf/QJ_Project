//
// Created by wmsksf on 14/01/20.
//

#include "JobScheduler.h"
#include "../project_jj_lib/Utils.h"

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

queryJob::queryJob(char *inq) {
    q.parse(inq);
}
void queryJob::run() {
    q.exec();
}

queryJob::~queryJob() {}


sortJob::sortJob(Relation *R, uint64_t start, uint64_t end, uint64_t current_byte, Relation *RR)
{
    this->R = R;
    this->RR = RR;
    this->start = start;
    this->end = end;
    this->current_byte = current_byte;

//    quick = false;
}

//sortJob::sortJob(Tuple *A, uint64_t lo, uint64_t hi)
//{
//    this->A = A;
//    this->lo = lo;
//    this->hi = hi;
//
//    quick = true;
//}
void sortJob::run() {
//    if (!quick)
    parallerRadixsort(R, start, end, current_byte, RR);
//    else
//        OptQuicksort(A, lo, hi);
}

sortJob::~sortJob() {}
//----------------------------------------------------------------------------------------------------------------------
pthread_mutex_t s_cout::s_mux{};
s_cout::~s_cout()
{
    pthread_mutex_lock(&s_mux);
    std::cout << this->str();
    pthread_mutex_unlock(&s_mux);
}

JoinJob::JoinJob(Relation* A,Relation* B,int AStart, int AEnd, int BStart, int BEnd, List** result, long int* resultRows) {
    this->A = A;
    this->B = B;
    this->Bstart = BStart;
    this->Aend = AEnd;
    this->Astart = AStart;
    this->Bend = BEnd;
    this->result = result;
    this->resultRows = resultRows;
}

void JoinJob::run() {
//    std::ostringstream oss;
//    s_cout{} << "join job: " << Astart << "-" << Aend << "  " << Bstart << "-" << Bend << std::endl;
//    std::string var = oss.str();
//    std::cout << var;

    Query::parallerJoin(A,B,Astart,Aend,Bstart,Bend,result,resultRows);
}

JoinJob::~JoinJob() {

}
