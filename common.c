#include "command.h"
#include <stdint.h>

uint64_t string_to_uint64(char *str) {
    uint64_t res;
    sscanf(str, "%llu", &res);
    return res;
}

int64_t string_to_int64(char *str) {
    int64_t res;
    sscanf(str, "%lld", &res);
    return res;
}

int string_to_int(char *str) {
    int res;
    sscanf(str, "%d", &res);
    return res;
}

/**
 * Get the urandom object
 * 
 * @param x pointer to random number
 * @return int 
 */
int 
get_urandom(uint64_t *x) {
    FILE *fp = fopen("/dev/urandom", "r");
    if (fp == NULL) {
        printf("[Error] Failed to open random file.\n");
        return 1;
    } 

    fread(x, sizeof(uint64_t), 1, fp); 

    fclose(fp);

    return 0;
}
