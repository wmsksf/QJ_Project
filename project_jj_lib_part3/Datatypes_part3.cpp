//
// Created by karolos on 5/1/20.
//

#include <cstdio>
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