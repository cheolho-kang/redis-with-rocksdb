#include "server_spy.h"

unsigned int dictTestHash(const void *key) {
    unsigned int hash = 401;
    char *str = (char *)key;

    while (*str != '\0') {
        hash = ((hash << 4) + (int)(*str)) % 10;
        str++;
    }

    return hash;
}
