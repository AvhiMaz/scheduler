#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "defines.h"
#include "lock_map.h"
#include "priority_queue.h"
#include "transaction.h"
#include <pthread.h>

typedef struct {
    PriorityQueue   queue;
    pthread_t       thread[MAX_THREAD_POOL_SIZE];
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int             shutdown;
    LockedHash      lm;
} ThreadPool;

void tp_init(ThreadPool *tp);
void tp_start(ThreadPool *tp);
void tp_submit(ThreadPool *tp, Transaction *tx);
void tp_shutdown(ThreadPool *tp);

#endif
