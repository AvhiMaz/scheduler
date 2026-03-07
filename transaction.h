#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdbool.h>
#include <stdint.h>

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

#endif


