#include "../thirdparty/cJSON.h"
#include "priority_queue.h"
#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute_tx(void *args) {
    Transaction *tx = (Transaction *)args;
    printf("executing tx id=%d fee=%d\n", tx->id, tx->priority_fee);
}

int main() {
    FILE *f = fopen("data/transactions.json", "r");
    if (!f) {
        printf("failed to open transactions.json\n");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    cJSON *json = cJSON_Parse(data);
    free(data);
    if (!json) {
        printf("failed to parse json\n");
        return 1;
    }

    int          count = cJSON_GetArraySize(json);
    Transaction *txs = malloc(sizeof(Transaction) * count);

    for (int i = 0; i < count; i++) {
        cJSON *item = cJSON_GetArrayItem(json, i);

        txs[i].id = cJSON_GetObjectItem(item, "id")->valueint;
        txs[i].priority_fee =
            cJSON_GetObjectItem(item, "priority_fee")->valueint;
        txs[i].status = TX_PENDING;
        txs[i].execute = execute_tx;
        txs[i].args = &txs[i];

        cJSON *accounts = cJSON_GetObjectItem(item, "accounts");
        cJSON *is_writable = cJSON_GetObjectItem(item, "is_writable");
        txs[i].num_accounts = cJSON_GetArraySize(accounts);

        for (int j = 0; j < txs[i].num_accounts; j++) {
            char *acc = cJSON_GetArrayItem(accounts, j)->valuestring;
            memcpy(txs[i].accounts[j], acc, strlen(acc) + 1);
            txs[i].is_writable[j] =
                cJSON_IsTrue(cJSON_GetArrayItem(is_writable, j));
        }
    }

    cJSON_Delete(json);

    PriorityQueue pq;
    pq.size = 0;

    for (int i = 0; i < count; i++) {
        push_pq(&pq, &txs[i]);
    }

    ThreadPool tp;
    tp_init(&tp);

    while (pq.size > 0) {
        Transaction *tx = pop_pq(&pq);
        tp_submit(&tp, tx);
    }

    tp_start(&tp);

    tp_wait(&tp);
    tp_shutdown(&tp);
    lm_free(&tp.lm);
    free(txs);

    return 0;
}
