//
// Created by karolos on 18/1/20.
//

#include "Barrier.h"

void init_barrier(barrier_t *barrier, int n)
{
    barrier->n = n;
    barrier->count = 0;
    sem_init(&barrier->mutex, 0, 1);
    sem_init(&barrier->turnstile, 0, 0);
    sem_init(&barrier->turnstile2, 0, 0);
}

void phase1_barrier(barrier_t *barrier)
{
    sem_wait(&barrier->mutex);
    if (++barrier->count == barrier->n) {
        int i;
        for (i = 0; i < barrier->n; i++) {
            sem_post(&barrier->turnstile);
        }
    }
    sem_post(&barrier->mutex);
    sem_wait(&barrier->turnstile);
}

void phase2_barrier(barrier_t *barrier)
{
    sem_wait(&barrier->mutex);
    if (--barrier->count == 0) {
        int i;
        for (i = 0; i < barrier->n; i++) {
            sem_post(&barrier->turnstile2);
        }
    }
    sem_post(&barrier->mutex);
    sem_wait(&barrier->turnstile2);
}

void wait_barrier(barrier_t *barrier)
{
    phase1_barrier(barrier);
    phase2_barrier(barrier);
}