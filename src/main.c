#include "lock_map.h"
#include "priority_queue.h"
#include <stdio.h>

int main() {
    PriorityQueue pq;
    pq.size = 0;

    Transaction t1 = {.id = 1, .priority_fee = 50, .status = TX_PENDING};
    Transaction t2 = {.id = 2, .priority_fee = 90, .status = TX_PENDING};
    Transaction t3 = {.id = 3, .priority_fee = 30, .status = TX_PENDING};

    push_pq(&pq, &t1);
    push_pq(&pq, &t2);
    push_pq(&pq, &t3);

    while (pq.size > 0) {
        Transaction *tx = pop_pq(&pq);
        printf("id=%d fee=%d\n", tx->id, tx->priority_fee);
    }

    LockedHash lm = {0};

    char       key1[32] = "alice";
    char       key2[32] = "bob";

    lm_lock(&lm, key1);
    lm_lock(&lm, key2);

    printf("alice locked: %d\n", lm_is_locked(&lm, key1));
    printf("bob locked:   %d\n", lm_is_locked(&lm, key2));

    lm_unlock(&lm, key1);
    printf("alice locked: %d\n", lm_is_locked(&lm, key1));

    return 0;
}
