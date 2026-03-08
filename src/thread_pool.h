#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "defines.h"
#include "transaction.h"
#include <pthread.h>

typedef struct {
    Transaction    *queue[MAX_TRANSACTIONS];
    int             queue_size;
    pthread_t       thread[MAX_THREAD_POOL_SIZE];
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    int             shutdown;
} ThreadPool;

#endif
