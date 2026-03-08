#ifndef LOCK_MAP_H
#define LOCK_MAP_H

#include "defines.h"

typedef struct LockNode {
    char             key[PUBKEY_SIZE];
    int              status;
    struct LockNode *next;
} LockNode;

typedef struct {
    LockNode *buckets[LOCK_MAP_SIZE];
} LockedHash;

#endif
