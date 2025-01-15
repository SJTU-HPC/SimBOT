#ifndef COMMON
#define COMMON

#include <stdint.h>
#include <stdio.h>

uint64_t string_to_uint64(char *str);
int64_t string_to_int64(char *str);
int string_to_int(char *str);
int get_urandom(uint64_t *x);

#endif