#include "priority_queue.h"
#include "thread_pool.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void execute_tx(void *args) {
    Transaction *tx = (Transaction *)args;
    printf("executing tx id=%d fee=%d\n", tx->id, tx->priority_fee);
}

int main() {
    Transaction txs[5] = {
        {.id = 1,
         .priority_fee = 50,
         .status = TX_PENDING,
         .execute = execute_tx,
         .num_accounts = 2},
        {.id = 2,
         .priority_fee = 90,
         .status = TX_PENDING,
         .execute = execute_tx,
         .num_accounts = 2},
        {.id = 3,
         .priority_fee = 30,
         .status = TX_PENDING,
         .execute = execute_tx,
         .num_accounts = 1},
        {.id = 4,
         .priority_fee = 70,
         .status = TX_PENDING,
         .execute = execute_tx,
         .num_accounts = 2},
        {.id = 5,
         .priority_fee = 10,
         .status = TX_PENDING,
         .execute = execute_tx,
         .num_accounts = 1},
    };

    // tx1: alice(w), bob(w)
    memcpy(txs[0].accounts[0], "alice", 32);
    txs[0].is_writable[0] = true;
    memcpy(txs[0].accounts[1], "bob", 32);
    txs[0].is_writable[1] = true;

    // tx2: alice(w), charlie(w) this conflicts with tx1 on alice
    memcpy(txs[1].accounts[0], "alice", 32);
    txs[1].is_writable[0] = true;
    memcpy(txs[1].accounts[1], "charlie", 32);
    txs[1].is_writable[1] = true;

    // tx3: dave(w) there is no conflict
    memcpy(txs[2].accounts[0], "dave", 32);
    txs[2].is_writable[0] = true;

    // tx4: bob(w), eve(w) there is a conflicts with tx1 on bob
    memcpy(txs[3].accounts[0], "bob", 32);
    txs[3].is_writable[0] = true;
    memcpy(txs[3].accounts[1], "eve", 32);
    txs[3].is_writable[1] = true;

    // tx5: frank(w) there no conflict
    memcpy(txs[4].accounts[0], "frank", 32);
    txs[4].is_writable[0] = true;

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
