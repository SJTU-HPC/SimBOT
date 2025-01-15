#ifndef LOGGOPSIM_
#define LOGGOPSIM_

#include <stdint.h>

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

#endif
