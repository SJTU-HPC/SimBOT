#include <stdlib.h>
#include <stdio.h>
#include "command.h"
#include "mpi_simulation.h"
#include "cJSON.h"
#include "LogGOPSim.h"
#include "common.h"

extern int my_rank;
extern int node_num;

/* Parameters of LogGOPS Model*/
int o = -1;
int O = -1;
int g = -1;
int L = -1;
int G = -1;
int S = -1;
int mpi_model_flag = 0;

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
    int num_of_rank) 
{
    if (mpi_model_flag == 0) {
        printf("Please set LogGOPS model's parameters\n");
        exit(0);
    }
    char file_path[1024];
    sprintf(file_path, "%s/%s.bin", arguments.outfile, filename);
    simulation(o, O, g, L, G, S, file_path, init_bias, comm_time, num_of_rank);
}

void init_par(const char *par, cJSON *mpi_json) {
    if (cJSON_GetObjectItem(mpi_json, par) == NULL) {
        printf("[Error] Please specify %s in LogGOPS model!\n", par);
        exit(1);
    }

    switch (par[0]) {
    case 'o':
        o = string_to_int(cJSON_GetObjectItem(mpi_json, "o")->valuestring);
        break;
    case 'O':
        O = string_to_int(cJSON_GetObjectItem(mpi_json, "O")->valuestring);
        break;
    case 'g':
        g = string_to_int(cJSON_GetObjectItem(mpi_json, "g")->valuestring);
        break;
    case 'L':
        L = string_to_int(cJSON_GetObjectItem(mpi_json, "L")->valuestring);
        break;
    case 'G':
        G = string_to_int(cJSON_GetObjectItem(mpi_json, "G")->valuestring);
        break;
    case 'S':
        S = string_to_int(cJSON_GetObjectItem(mpi_json, "S")->valuestring);
        break;
    default:
        break;
    }
}

void parse_mpi(cJSON *mpi_json) {
    if (mpi_json == NULL) return;

    cJSON *loggpos_json = cJSON_GetObjectItem(mpi_json, "LogGOPS");
    if (loggpos_json == NULL) return;

    init_par("o", loggpos_json);
    init_par("O", loggpos_json);
    init_par("g", loggpos_json);
    init_par("L", loggpos_json);
    init_par("G", loggpos_json);
    init_par("S", loggpos_json);
    mpi_model_flag = 1;
    if (my_rank == 0) printf("[SIMBOT INFO] LogGOPS Model: o:%d, O:%d, g:%d, L:%d, G:%d, S:%d\n", \
                                o, O, g, L, G, S);
}

// schedule file
void gen_schedule(char *comm_ptrn, int msg_bytes) {
    // schedgen -d <mst_bytes> -s <node_num> -p <comm_ptrn>
    char cmd[1024];
    sprintf(cmd, "schedgen -d %d -s %d -p %s -o %s/%s.goal", \
            msg_bytes, node_num, comm_ptrn, arguments.outfile, comm_ptrn);
    system(cmd);

    // convert goal to bin
    sprintf(cmd, "txt2bin -i %s/%s.goal -o %s/%s.bin", 
            arguments.outfile, comm_ptrn, arguments.outfile, comm_ptrn);
    system(cmd);
}

void gen_comm_file(cJSON *program_json) {
    cJSON *comm_json = cJSON_GetObjectItem(program_json, "communications");

    if (comm_json == NULL) return;

    comm_json = comm_json->child;
    while (comm_json != NULL) {
        printf("[SIMBOT INFO] Generating %s goal file\n", comm_json->string);
        cJSON *msg_bytes_json = cJSON_GetObjectItem(comm_json, "messageBytes");
        if (msg_bytes_json == NULL) {
            printf("[Error] Please set message bytes for %s\n", comm_json->string);
            exit(0);
        }
        gen_schedule(comm_json->string, msg_bytes_json->valueint);
        comm_json = comm_json->next;
    }
}
