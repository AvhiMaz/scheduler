#include "lock_map.h"

int hash(char *key) {

    int sum = 0;
    for (int i = 0; i < 32; i++) {
        sum += key[i];
    };

    return sum % LOCK_MAP_SIZE;
}
