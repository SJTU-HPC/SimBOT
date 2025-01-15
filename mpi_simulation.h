#ifndef MPI_SIMULATION
#define MPI_SIMULATION

#include <stdint.h>
#include "cJSON.h"

extern int o;
extern int O;
extern int g;
extern int L;
extern int G;
extern int S;

void parse_mpi(cJSON *mpi_json);
void gen_comm_file(cJSON *program_json);
void mpi_simulation(
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
