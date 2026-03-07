#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_ACCOUNTS     64
#define PUBKEY_SIZE      32

#define TX_PENDING       0
#define TX_RUNNING       1
#define TX_DONE          2
#define TX_FAILED        3

#define MAX_TRANSACTIONS 256

typedef void (*ExecuteFn)(void *);

typedef struct {
    uint16_t  id;
    uint16_t  priority_fee;
    char      accounts[MAX_ACCOUNTS][PUBKEY_SIZE];
    int       num_accounts;
    bool      is_writable[MAX_ACCOUNTS];
    int       status;
    ExecuteFn execute;
    void     *args;
} Transaction;

typedef struct {
    Transaction *data[MAX_TRANSACTIONS];
    int          size;
} PriorityQueue;

void push_pq(PriorityQueue *pq, Transaction *tx) {
    pq->data[pq->size] = tx;
    pq->size++;

    int i = pq->size - 1;

    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->data[i]->priority_fee > pq->data[parent]->priority_fee) {

            Transaction *temp = pq->data[i];
            pq->data[i] = pq->data[parent];
            pq->data[parent] = temp;
            i = parent;

        } else {
            break;
        }
    }
}
