#include "thread_pool.h"
#include "defines.h"
#include "lock_map.h"
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

        int conflict = 0;

        if (tp->shutdown) {
            pthread_mutex_unlock(&tp->mutex);
            return NULL;
        } else {

            Transaction *tx = pop_pq(&tp->queue);
            for (int i = 0; i < tx->num_accounts; i++) {
                if (tx->is_writable[i] &&
                    lm_is_locked(&tp->lm, tx->accounts[i])) {
                    conflict = 1;
                    break;
                }
            }

            if (conflict) {
                push_pq(&tp->queue, tx);
                pthread_mutex_unlock(&tp->mutex);
            } else {
                for (int i = 0; i < tx->num_accounts; i++) {
                    if (tx->is_writable[i]) {
                        lm_lock(&tp->lm, tx->accounts[i]);
                    }
                }

                pthread_mutex_unlock(&tp->mutex);
                tx->execute(tx->args);
                for (int i = 0; i < tx->num_accounts; i++) {
                    if (tx->is_writable[i]) {
                        lm_unlock(&tp->lm, tx->accounts[i]);
                    }
                }
            }
        }
    }
}

void tp_init(ThreadPool *tp) {
    tp->queue.size = 0;
    tp->shutdown = 0;
    tp->lm = (LockedHash){0};

    pthread_mutex_init(&tp->mutex, NULL);
    pthread_cond_init(&tp->cond, NULL);
}

void tp_start(ThreadPool *tp) {
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

void tp_shutdown(ThreadPool *tp) {

    pthread_mutex_lock(&tp->mutex);
    tp->shutdown = 1;
    pthread_cond_broadcast(&tp->cond);
    pthread_mutex_unlock(&tp->mutex);

    for (int i = 0; i < MAX_THREAD_POOL_SIZE; i++) {
        pthread_join(tp->thread[i], NULL);
    }
}
