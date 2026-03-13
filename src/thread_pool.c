#include "thread_pool.h"
#include "defines.h"
#include "priority_queue.h"
#include "transaction.h"
#include <pthread.h>

void *worker(void *args) {
    ThreadPool *tp = (ThreadPool *)args;

    while (1) {
        pthread_mutex_lock(&tp->mutex);

        while (tp->queue.size == 0 && !tp->shutdown) {
            pthread_cond_wait(&tp->cond, &tp->mutex);
        }

        if (tp->shutdown) {
            pthread_mutex_unlock(&tp->mutex);
            return NULL;
        } else {
            Transaction *tx = pop_pq(&tp->queue);
            pthread_mutex_unlock(&tp->mutex);
            tx->execute(tx->args);
        }
    }
}

void tp_init(ThreadPool *tp) {
    tp->queue.size = 0;
    tp->shutdown = 0;

    pthread_mutex_init(&tp->mutex, NULL);
    pthread_cond_init(&tp->cond, NULL);

    for (int i = 0; i < MAX_THREAD_POOL_SIZE; i++) {
        pthread_create(&tp->thread[i], NULL, worker, tp);
    }
}

void tp_submit(ThreadPool *tp, Transaction *tx) {

    pthread_mutex_lock(&tp->mutex);
    push_pq(&tp->queue, tx);
    pthread_cond_signal(&tp->cond);
    pthread_mutex_unlock(&tp->mutex);
}
