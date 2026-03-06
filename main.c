#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_ACCOUNTS 64
#define PUBKEY_SIZE 32

#define TX_PENDING 0
#define TX_RUNNING 1
#define TX_DONE 2
#define TX_FAILED 3

typedef struct {
    uint16_t id;
    uint16_t priority_fee;
    char accounts[MAX_ACCOUNTS][PUBKEY_SIZE];
    int num_accounts;
    bool is_writable[MAX_ACCOUNTS];
    void (*execute)(void *);
    void *args;
    int status;
} Transaction;

int main() {
    printf("Hello, World!\n");
    return 0;
};
