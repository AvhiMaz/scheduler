#include "priority_queue.h"
#include "thread_pool.h"
#include <stdio.h>
#include <unistd.h>

void execute_tx(void *args) {
    Transaction *tx = (Transaction *)args;
    printf("executing tx id=%d fee=%d\n", tx->id, tx->priority_fee);
}

int main() {
    Transaction txs[30] = {
        {.id = 1,
         .priority_fee = 50,
         .status = TX_PENDING,
         .execute = execute_tx},
        {.id = 2,
         .priority_fee = 90,
         .status = TX_PENDING,
         .execute = execute_tx},
        {.id = 3,
         .priority_fee = 30,
         .status = TX_PENDING,
         .execute = execute_tx},
        {.id = 4,
         .priority_fee = 70,
         .status = TX_PENDING,
         .execute = execute_tx},
        {.id = 5,
         .priority_fee = 10,
         .status = TX_PENDING,
         .execute = execute_tx},
    };

    PriorityQueue pq;
    pq.size = 0;

    for (int i = 0; i < 5; i++) {
        txs[i].args = &txs[i];
        push_pq(&pq, &txs[i]);
    }

    ThreadPool tp;
    tp_init(&tp);

    while (pq.size > 0) {
        Transaction *tx = pop_pq(&pq);
        tp_submit(&tp, tx);
    }

    tp_start(&tp);

    sleep(1);
    tp_shutdown(&tp);

    return 0;
}
