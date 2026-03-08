#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>

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

#endif
