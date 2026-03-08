#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "transaction.h"

typedef struct {
    Transaction *data[MAX_TRANSACTIONS];
    int          size;
} PriorityQueue;

void         push_pq(PriorityQueue *pq, Transaction *tx);
Transaction *pop_pq(PriorityQueue *pq);

#endif
