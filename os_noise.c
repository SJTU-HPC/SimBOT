#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <mpi.h>
#include "common.h"
#include "os_noise.h"
#include "cJSON.h"

extern int my_rank;
extern int core_num;
extern int node_num;
char *os_trace_dir;
int trace_n = 0;
uint64_t **time = NULL;
uint64_t **detour = NULL;
uint64_t *node_start_time = NULL;
int os_noise_flag = 0;
uint64_t *core_time;

uint64_t** malloc_2d(int height, int width) {
    uint64_t *mat = (uint64_t *) malloc(sizeof(uint64_t) * height * width);
    uint64_t **mat_2d = (uint64_t **) malloc(sizeof(uint64_t *) * height);
    for (int i = 0; i < height; ++i) {
        mat_2d[i] = &mat[i * width];
    }
    return mat_2d;
}

void free_2d(uint64_t **mat_2d) {
    if (mat_2d != NULL) {
        if (mat_2d[0] != NULL) free(mat_2d[0]);
        free(mat_2d);
    }
}

void free_os_noise() {
    free_2d(time);
    free_2d(detour);
    free(node_start_time);
}

int file_exist(char *path) {
    return !access(path, F_OK);
}

void check_trace_dir(char *os_dir) {
    if (!file_exist(os_dir)) {
        printf("[Error] Directory %s not found\n", os_dir);
        exit(0);
    }
    DIR *dir_p;
    dir_p = opendir(os_dir);
    struct dirent *ent;
    int trace_count = 0;
    while ((ent = readdir (dir_p)) != NULL) {
        trace_count++;
    }
    closedir (dir_p);

    if (trace_count < core_num) {
        printf("[Error] Lack of trace file");
        exit(0);
    }
}

int get_trace_num(char *file_name) {
    FILE *fp = fopen(file_name, "r");
    char buffer[1024];
    int count = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (buffer[0] > '9' || buffer[0] < '0') continue;
        count++;
    }
    fclose(fp);
    return count;
}

void get_trace_data(char *file_name, uint64_t *trace_time, uint64_t *trace_detour) {
    FILE *fp = fopen(file_name, "r");
    char buffer[1024];
    int i = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (buffer[0] > '9' || buffer[0] < '0') continue;
        sscanf(buffer, "%llu\t%llu", &trace_time[i], &trace_detour[i]);
        trace_time[i] *= 1000;  // nanosecond to picosecond 
        trace_detour[i] *= 1000;
        i++;
    }
    fclose(fp);
}

void read_trace(char *os_dir) {
    if (my_rank == 0) {
        printf("[SimBOT INFO] Reading trace file\n");
        char file_name[1024];
        sprintf(file_name, "%s/%s", os_dir, "trace.0");
        trace_n = get_trace_num(file_name);
    }
    MPI_Bcast(&trace_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    time = malloc_2d(core_num, trace_n);
    detour = malloc_2d(core_num, trace_n);
    if (my_rank == 0) {
        for (int i = 0; i < core_num; ++i) {
            char file_name[1024];
            sprintf(file_name, "%s/trace.%d", os_dir, i);
            if (!file_exist(file_name)) {
                printf("[Error] file:%s doesn't exist!\n", file_name);
                exit(0);
            }
            get_trace_data(file_name, time[i], detour[i]);
        }
    }
    MPI_Bcast(time[0], core_num * trace_n, MPI_UINT64_T, 0, MPI_COMM_WORLD);
    MPI_Bcast(detour[0], core_num * trace_n, MPI_UINT64_T, 0, MPI_COMM_WORLD);
}

void gen_rand_start_time() {
    node_start_time = (uint64_t*) malloc(sizeof(uint64_t) * node_num);
    if (my_rank == 0) {
        uint64_t mseed;
        get_urandom(&mseed);
        srand(mseed);
        for (int i = 0; i < node_num; ++i) {
            node_start_time[i] = rand() % time[0][trace_n - 1];
        }
    }
    MPI_Bcast(node_start_time, node_num, MPI_UINT64_T, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("[SimBOT INFO] start time, ");
        for (int i = 0; i < node_num; ++i) {
            printf("node%d:%llu ", i, node_start_time[i]);
        }
        printf("\n");
    }
}

void parse_os_noise(cJSON *os_noise_json) {
    if (os_noise_json == NULL) {
        if (my_rank == 0) printf("[SimBOT INFO] No operating system noise\n");
        return;
    }

    cJSON *dir_json = cJSON_GetObjectItem(os_noise_json, "osNoiseTraceDir");
    if (dir_json == NULL) {
        if (my_rank == 0) printf("[Error] Please specify osNoiseTraceDir!\n");
        exit(0);
    }
    os_trace_dir = dir_json->valuestring;

    if (my_rank == 0) check_trace_dir(os_trace_dir);
    
    read_trace(os_trace_dir);
    gen_rand_start_time();
}

int find_max_less_than(const uint64_t *arr, int l, int r, const uint64_t target) {
    if (arr[r] <= target) return r;
    while (l < r) {
        int mid = (l + r) / 2;
        if (arr[mid] > target) r = mid;
        if (arr[mid] <= target) l = mid + 1;
    }
    return l - 1;
}

uint64_t get_os_noise(
    const int global_core_id,
    uint64_t start_time, uint64_t end_time) 
{
    uint64_t os_noise = 0;
    const int node_id = global_core_id / core_num;
    const int core_id_node = global_core_id % core_num;   // core id of its node
    uint64_t *noise_time = time[core_id_node];
    uint64_t *noise_detour = detour[core_id_node];
    
    uint64_t compute_time = end_time - start_time;
    start_time = (start_time + node_start_time[node_id]) % \
                (noise_time[trace_n - 1] - compute_time);    // end_time must less than the last sample time
    end_time = start_time + compute_time;       // update end_time

    int pos = find_max_less_than(noise_time, 0, trace_n - 1, start_time);

    if (noise_time[pos] + noise_detour[pos] > start_time) {
        os_noise += noise_time[pos] + noise_detour[pos] - start_time;
    }

    if (pos == trace_n - 1) {
        end_time -= noise_time[trace_n - 1];         // adjust end time
        pos = 0;                        // set position to first 
    }

    // if we reach into next sample - then add the whole time of sample
    while (end_time > noise_time[pos + 1]) {
        pos++;
        os_noise += noise_detour[pos];
        // if we're at the end of samples - wrap around
        if (pos == trace_n - 1) {
            end_time -= noise_time[trace_n - 1];
            pos = 0;
        }
    }

    return os_noise;
}
