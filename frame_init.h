#ifndef FRAME_INIT
#define FRAME_INIT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <fcntl.h>
#include <stdint.h>
#include <mpi.h>

#define MAX_FILE_SIZE 10000
#define MAX_CHILD_NUM 500
#define MAX_PARENT_NUM 500
#define MAX_NAME_SIZE 200
#define MAX_NODE_NUM 2000
#define MAX_RECORD_NUM 100
#define MAX_FILE_NAME 200
#define NUM_RUN_DEFAULT 50
#define MAX_NOISE_NAME 50

#define COMPUTE_TYPE 0
#define COMMUNCATE_TYPE 1 

typedef struct noise{
	char *noise_type;
	int parameter_num;
	double *parameters;
} noise;

typedef struct graph_node graph_node;
typedef struct graph graph;

struct graph_node{
	/* self information */
	char *name;					// Function name
	uint64_t run_time;			// Actual running time
	uint64_t std_run_time;		// Standard running time
	noise *m_noise;				// Performance variation
	int m_type;					// Compute or communcate

	/* parents and childs */
	graph_node **parents;
	graph_node **childs;
	int child_num;
	int parent_num;

	int *loop_nums; //number of loop per child
	int sync; //sync：1  nonsync: 0 
	int record_flag; //if need to record time
	uint64_t mrunning_time; //time to execute once
	uint64_t total_mrunning_time; //total running time 
};

struct graph{
	int node_num;
	graph_node **node;
	graph_node *root_node; //main function
};


/* node config */
extern int node_num;
extern int core_num;
extern noise *node_noise;
extern int total_core_num;

/* gsl */
extern const gsl_rng_type *T;
extern gsl_rng **r;
uint64_t *seed;

/* mpi */
extern int my_rank;
extern int comm_sz;
extern int local_start;
extern int local_sz;
extern int local_node_start;
extern int local_node_sz;
extern int local_node_sz;
extern int *all_local_sz;
extern int *all_displs;
extern int *all_node_sz;
extern int *all_node_displs;


void clear_graph(graph *m_graph);
void clear_node(graph_node *mnode);
int add_edge(char *child_name, char *parent_name, int loop_num, graph *m_graph);
int add_node(const char *mnode_name, uint64_t mrun_time, noise *m_noise, int sync, int record_time, int type_flag, graph *m_graph);
void add_func_edges(cJSON *functions, graph *m_graph);
graph_node* find_node(char *name, graph *m_graph);
void parse_program(graph ***m_graph, cJSON *config_json);
void parse_node_config(cJSON* core_json, cJSON* node_json);
void parse_os_config(cJSON* os_json);
int read_to_buf(const char *file_name, char *config_buf);
void init_graph(graph ***m_graph, cJSON *config_json);
void init_nodes(graph *m_graph, cJSON *config_json);
void init_edges(graph *m_graph, cJSON *config_json);
uint64_t get_std_time(graph *m_graph);
void distribute_job();

#endif