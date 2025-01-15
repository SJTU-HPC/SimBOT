#include <stdint.h>
#include <stdio.h>
#include "LogGOPSim_.h"

void _simulation(
  const int o,
  const int O,
  const int g,
  const int L,
  const int G,
  const int S,
  char *filename,
  uint64_t *init_bias,
  uint64_t *comm_time,
  int num_of_rank);

#ifdef __cplusplus
extern "C" {
#endif

void simulation(
    const int o,
    const int O,
    const int g,
    const int L,
    const int G,
    const int S,
    char *filename,
    uint64_t *init_bias,
    uint64_t *comm_time,
    int num_of_rank)
{
    _simulation(o, O, g, L, G, S, filename, init_bias, comm_time, num_of_rank);
}

#ifdef __cplusplus
}
#endif
