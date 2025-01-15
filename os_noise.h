#ifndef OS_NOISE
#define OS_NOISE
#include <stdint.h>
#include "cJSON.h"

extern int os_noise_flag;
void parse_os_noise(cJSON *os_noise_json);
void free_os_noise();
uint64_t get_os_noise(int global_core_id, uint64_t start_time, uint64_t end_time);

#endif