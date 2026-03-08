#include "lock_map.h"
#include <stdlib.h>
#include <string.h>

int hash(char *key) {

    int sum = 0;
    for (int i = 0; i < 32; i++) {
        sum += key[i];
    };

    return sum % LOCK_MAP_SIZE;
}

int lm_lock(LockedHash *lm, char *key) {
    int       index = hash(key);

    LockNode *node = lm->buckets[index];

    while (node != NULL) {
        if (strcmp(node->key, key) == 0) {
            node->status = 1;
            return 1;
        }

        node = node->next;
    }

    LockNode *new_node = malloc(sizeof(LockNode));
    memcpy(new_node->key, key, 32);
    new_node->status = 1;
    new_node->next = lm->buckets[index];
    lm->buckets[index] = new_node;

    return 1;
}
