#include "priority_queue.h"
#include "thread_pool.h"
#include <stdio.h>
#include <unistd.h>

void execute_tx(void *args) {
    Transaction *tx = (Transaction *)args;
    printf("executing tx id=%d fee=%d\n", tx->id, tx->priority_fee);
}

int main() {
    PriorityQueue pq;
    pq.size = 0;

    Transaction t1 = {.id = 1,
                      .priority_fee = 50,
                      .status = TX_PENDING,
                      .execute = execute_tx};
    Transaction t2 = {.id = 2,
                      .priority_fee = 90,
                      .status = TX_PENDING,
                      .execute = execute_tx};
    Transaction t3 = {.id = 3,
                      .priority_fee = 30,
                      .status = TX_PENDING,
                      .execute = execute_tx};
    Transaction t4 = {.id = 4,
                      .priority_fee = 70,
                      .status = TX_PENDING,
                      .execute = execute_tx};
    Transaction t5 = {.id = 5,
                      .priority_fee = 10,
                      .status = TX_PENDING,
                      .execute = execute_tx};

    t1.args = &t1;
    t2.args = &t2;
    t3.args = &t3;
    t4.args = &t4;
    t5.args = &t5;

    push_pq(&pq, &t1);
    push_pq(&pq, &t2);
    push_pq(&pq, &t3);
    push_pq(&pq, &t4);
    push_pq(&pq, &t5);

    ThreadPool tp;
    tp_init(&tp);

    while (pq.size > 0) {
        Transaction *tx = pop_pq(&pq);
        tp_submit(&tp, tx);
    }

    sleep(1);
    tp_shutdown(&tp);

    return 0;
}
