//
// Created by karolos on 18/1/20.
//

#ifndef PROJECT_JJ_BARRIER_H
#define PROJECT_JJ_BARRIER_H

#include <semaphore.h>

typedef struct {
    int n;
    int count;
    sem_t mutex;
    sem_t turnstile;
    sem_t turnstile2;
} barrier_t;

void init_barrier(barrier_t *barrier, int n);

void phase1_barrier(barrier_t *barrier);

void phase2_barrier(barrier_t *barrier);

void wait_barrier(barrier_t *barrier);

#endif //PROJECT_JJ_BARRIER_H
