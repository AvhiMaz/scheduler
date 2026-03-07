#include "priority_queue.h"

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

Transaction *pop_pq(PriorityQueue *pq) {
    Transaction *popped = pq->data[0];

    pq->data[0] = pq->data[pq->size - 1];

    pq->size--;

    int i = 0;

    while (1) {
        int left_child = (2 * i) + 1;
        int right_child = (2 * i) + 2;

        if (left_child >= pq->size)
            break;

        int largest = left_child;
        if (right_child < pq->size && pq->data[right_child]->priority_fee >
                                          pq->data[left_child]->priority_fee) {
            largest = right_child;
        }

        if (pq->data[i]->priority_fee < pq->data[largest]->priority_fee) {
            Transaction *temp = pq->data[i];
            pq->data[i] = pq->data[largest];
            pq->data[largest] = temp;
            i = largest;
        } else {
            break;
        }
    }
    return popped;
}
