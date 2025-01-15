#include "frame_init.h"
#include "print_running_time.h"
#include "command.h"
#include "common.h"
#include "mpi_simulation.h"
#include "os_noise.h"
#include <math.h>

/* node configration */
int node_num;       // The number of node(the number of process)
int core_num;       // The number of cores per node
noise *node_noise;
int total_core_num;

/* gsl  */
const gsl_rng_type *T;
gsl_rng **r;
uint64_t *seed;

/* mpi */ 
int my_rank;
int comm_sz;
int local_start; //core
int local_sz; //core
int local_node_start; //node
int local_node_sz; //node
int *all_local_sz; //core
int *all_displs;  //core
int *all_node_sz; //node
int *all_node_displs; //node

/* sync */
uint64_t *last_sync_time;

/**
 * recursive calculation of standard runntime
 * 
 * @param node pointer to the node of program graph
 * @return uint64_t 
 */
uint64_t 
__get_std_time(graph_node *node) {
	uint64_t time = 0;
	time += node->run_time;
	for (int i = 0; i < node->child_num; i ++) {
		time += __get_std_time(node->childs[i]) * node->loop_nums[i];
	}
	return time;
}

/**
 * Get the standard runtime
 * 
 * @param m_graph 
 * @return uint64_t 
 */
uint64_t 
get_std_time(graph *m_graph) {
    if (my_rank == 0) {
        graph_node *main = m_graph->root_node;
        uint64_t time = 0;
        time = __get_std_time(main);
        printf("[SIMBOT INFO] Theoretical Time: %llu\n", time);
    }
}

/**
 * init mpi, parse argv, parse configuration
 * 
 * @param argc 
 * @param argv 
 * @param m_graph pointers to the program graph 
 */
void 
Init (int argc, char **argv, graph ***m_graph) {
    char *config_buf;
    cJSON *config_json, *core_json, *node_json, *os_json, *program_json, *mpi_json;

    /* init mpi */
    int err = MPI_Init(NULL, NULL);
    if (err) {
        printf("[Error] Failed to initilize MPI environemnt.\n");

        exit(-1);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    /* parse argv */
    command_parse(argc, argv);

    /* read configuration file */
	config_buf = (char*) malloc(MAX_FILE_SIZE);
	if (my_rank == 0) {
		if (!read_to_buf(arguments.config, config_buf)) {
            exit(-1);
        }
    }
	MPI_Bcast(config_buf, MAX_FILE_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
	config_json = cJSON_Parse(config_buf);
	if (config_json == NULL && my_rank == 0) {
		printf("[Error] parse config file fail. Please check your config file.\n");
		exit(-1);
	}
    node_json = cJSON_GetObjectItem(config_json, "node");
    core_json = cJSON_GetObjectItem(config_json, "core");
    os_json = cJSON_GetObjectItem(config_json, "osNoise");
    program_json = cJSON_GetObjectItem(config_json, "program");
    mpi_json = cJSON_GetObjectItem(config_json, "mpiSim");

    /* parse node configuration */
    parse_node_config(core_json, node_json);   

    /* distribute job */
    distribute_job();

    /* parse os noise configuration */
    parse_os_noise(os_json);

    /* parse Program configuration */
    parse_program(m_graph, program_json);

    /* parse MPI simulation model configuration */
    parse_mpi(mpi_json);

    /* generate communication bin file */
    if (my_rank == 0) gen_comm_file(program_json);

    /* init sync time */
    last_sync_time = (uint64_t*) malloc(sizeof(uint64_t) * local_sz);
    memset(last_sync_time, 0, sizeof(uint64_t) * local_sz);

    /* print */ 
    if (my_rank == 0) {
        printf("[SIMBOT INFO] Nodes: %d, Cores: %d\n", node_num, core_num);
    }

    /* clean */
    cJSON_Delete(config_json);
}

/**
 * 
 * Node performance variation
 * @param pnode  pointers to node var perf factor
 * @param n      # of nodes.
 * @return int 
 */
int
get_nodevar(double *pnode, int n) {
    if (n == 0) {
        return 0;
    }
    int i;
    double sigma, ucut, dcut;
    uint64_t mseed;

    gsl_rng_env_setup();
    gsl_rng *mr;
    const gsl_rng_type *mT;
    mT = gsl_rng_ranlux389;
    mr = gsl_rng_alloc(mT);
    get_urandom(&mseed);
    gsl_rng_set(mr, mseed);


    sigma = 3 * node_noise->parameters[0]; // 3sigma for +-5% of 99.9% ndoes.
    ucut = 0 + node_noise->parameters[1];  // Cutoff at +NORM_CUT
    dcut = 0 - node_noise->parameters[1];  // Cutoff at -NORM_CUT

    for (i = 0; i < n; i ++) {
        pnode[i] = gsl_ran_gaussian(mr, sigma);
        pnode[i] = pnode[i] > ucut? ucut: pnode[i];
        pnode[i] = pnode[i] < dcut? dcut: pnode[i];
        pnode[i] += 1;
    } 
    gsl_rng_free(mr);

    return 0;
}

/**
 * add node noise
 * 
 * @param m_graph pointers to program graph 
 */
void
add_node_noise(graph **m_graph) {
    if(node_noise == NULL) //no node noise
        return;

    /* get node performance*/
    double *node_perf = (double*) malloc(sizeof(double) * node_num);
    if(my_rank == 0) {
        get_nodevar(node_perf, node_num);
    }
    MPI_Bcast(node_perf, node_num, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    /* calc runtime */
    for (int i = 0; i < local_sz; i ++){
        int sim_node = (i + local_start) / core_num;
        for (int j = 0; j < m_graph[i]->node_num ; j ++){
            m_graph[i]->node[j]->run_time = (double)(m_graph[i]->node[j]->std_run_time) / node_perf[sim_node];
        }
    }

    /* clean */
    free(node_perf);
}

/**
 * generate random runtime  
 * 
 * @param node pointers to node of program graph
 * @param i index of sim core
 * @return double 
 */
uint64_t
add_variation(graph_node *node, int i) {
    if(node->m_noise == NULL || node->run_time == 0)
        return node->run_time;

    if (strcmp(node->m_noise->noise_type, "pareto") == 0) {
        /* pareto distribution */
        double s = gsl_ran_pareto(r[0], node->m_noise->parameters[0], 1);
        s = s > 2? 2: s;
        return((double)node->run_time * s);
    }

    return node->run_time; 
}

/**
 * update to the max runtime
 * 
 * @param totalrun_time pointers to the sim core runtime 
 */
void
update_to_global_max_time(uint64_t *totalrun_time) {
    uint64_t local_max_time = 0;
    uint64_t global_max_time = 0;
    for (int i = 0; i < local_sz; i ++){
        if(totalrun_time[i] > local_max_time)
            local_max_time = totalrun_time[i];
    }
    MPI_Allreduce(&local_max_time, &global_max_time, 1, MPI_UINT64_T, MPI_MAX, MPI_COMM_WORLD);
    
    for (int i = 0; i < local_sz; i ++){
        totalrun_time[i] = global_max_time;
    }
}

/**
 * update to the max runtime in the same process
 * 
 * @param totalrun_time pointers to the sim core runtime 
 */
void
sync_threads(uint64_t *local_total_run_time, uint64_t *init_bias) {
    uint64_t *global_total_run_time = (uint64_t*) malloc(sizeof(uint64_t) * total_core_num);
    // gather to rank 0
    MPI_Gatherv(local_total_run_time, local_sz, MPI_UINT64_T, global_total_run_time,
                all_local_sz, all_displs, MPI_UINT64_T, 0, MPI_COMM_WORLD);

    // update to the max runtime in the same process
    if (my_rank == 0) {
        for (int i = 0; i < node_num; ++i) {
            uint64_t *cur_run_time = &global_total_run_time[i * core_num];
            // get max
            int max_run_time = cur_run_time[0];
            for (int j = 1; j < core_num; ++j) {
                if (cur_run_time[j] > max_run_time) {
                    max_run_time = cur_run_time[j];
                }
            }
            // set max
            for (int j = 0; j < core_num; ++j) {
                cur_run_time[j] = max_run_time;
            }
        }
        for (int i = 0; i < node_num; ++i) {
            init_bias[i] = round(global_total_run_time[i * core_num]);
        }
    }

    // scatter
    MPI_Scatterv(global_total_run_time, all_local_sz, all_displs, MPI_UINT64_T,
                local_total_run_time, local_sz, MPI_UINT64_T, 0, MPI_COMM_WORLD);

    free(global_total_run_time);
}

/**
 * recursive calculation of real runntime
 * 
 * @param ptr pointers to the sim node
 * @param local_total_run_time pointers to the runtime
 */
void
__get_real_time(graph_node **ptr, uint64_t *local_total_run_time) {
    if (ptr[0]->m_type == COMPUTE_TYPE) {
        graph_node **new_ptr = (graph_node**) malloc(sizeof(graph_node*) * local_sz);
        /* self runtime */
        for (int i = 0; i < local_sz; i ++) {
            uint64_t time = add_variation(ptr[i], i);
            if (ptr[i]->record_flag == 1) {
                ptr[i]->mrunning_time = local_total_run_time[i]; //start time
            }
            local_total_run_time[i] += time;
        }

        /* childs' run time */
        for (int i = 0; i < ptr[0]->child_num; i ++) {
            for (int k = 0; k < local_sz; k ++)
                new_ptr[k]=ptr[k]->childs[i];
            for (int j = 0; j < ptr[0]->loop_nums[i]; j ++)
                __get_real_time(new_ptr, local_total_run_time);
        }

        /* global sync */
        if (ptr[0]->sync == 1) {
            // add os noise before sync
            for (int i = 0; i < local_sz; ++i) {
                uint64_t os_noise = get_os_noise(i + local_start, last_sync_time[i], local_total_run_time[i]);
                local_total_run_time[i] += os_noise;
                last_sync_time[i] = local_total_run_time[i];
            }
            update_to_global_max_time(local_total_run_time);
        }

        /* record mrunning time */
        if (ptr[0]->record_flag == 1) {
            for (int i = 0; i < local_sz; i ++) {
                ptr[i]->mrunning_time = local_total_run_time[i] - ptr[i]->mrunning_time;
                ptr[i]->total_mrunning_time += ptr[i]->mrunning_time;
            }
        }
        free(new_ptr);
    }

    if (ptr[0]->m_type == COMMUNCATE_TYPE) {
        uint64_t *init_bias = (uint64_t*) malloc(sizeof(uint64_t) * node_num);
        uint64_t *end_time = (uint64_t*) malloc(sizeof(uint64_t) * node_num);

        // add os noise before sync
        for (int i = 0; i < local_sz; ++i) {
            uint64_t os_noise = get_os_noise(i + local_start, last_sync_time[i], local_total_run_time[i]);
            local_total_run_time[i] += os_noise;
            last_sync_time[i] = local_total_run_time[i];
        }

        // Synchronize threads (simulate behavior of OpenMP)
        sync_threads(local_total_run_time, init_bias);

        // LogGOPS Model
        if (my_rank == 0) {
            mpi_simulation(o, O, g, L, G, S, ptr[0]->name, init_bias, end_time, node_num);
        }

        // Broadcast end_time
        MPI_Bcast(end_time, node_num, MPI_UINT64_T, 0, MPI_COMM_WORLD);

        // 1. Update current running time after communication
        // 2. Communication is seen as sync
        // 3. Ignore os noise in MPI communication
        for (int i = 0; i < local_sz; i ++) {
            local_total_run_time[i] = end_time[(local_start + i) / core_num];
            last_sync_time[i] = local_total_run_time[i];
        }

        free(end_time);
        free(init_bias);
    }
}

void 
_print_pragram_tree(graph_node *root, int dep) {
    printf("  * ");
    for (int i = 0; i < dep; ++i) {
        printf("\t\t");
    }
    printf("%s\n", root->name);
    for (int i = 0; i < root->child_num; ++i) {
        _print_pragram_tree(root->childs[i], dep + 1);
    }
}

void 
print_pragram_tree(graph_node *root) {
    printf("[SIMBOT INFO] pragram structure:\n");
    _print_pragram_tree(root, 0);
}

/**
 * get real runtime
 * 
 * @param m_graph 
 * @param local_total_run_time 
 */
void
get_real_time(graph **m_graph, uint64_t *local_total_run_time) {
    memset(local_total_run_time, 0, sizeof(uint64_t) * local_sz);

    graph_node **ptr = (graph_node**) malloc(sizeof(graph_node*) * local_sz);
    for (int i = 0; i < local_sz; i ++){
        ptr[i] = m_graph[i]->root_node;
    }

    /* get real run time */
    __get_real_time(ptr, local_total_run_time);

    /* clean */
    free(ptr);
}

/**
 * init gsl
 * 
 */
void
Init_gsl(){
    gsl_rng_env_setup();
    T = gsl_rng_ranlux389;
    r = (gsl_rng**) malloc(sizeof(gsl_rng*) * local_sz);
    seed = (uint64_t*) malloc(sizeof(uint64_t) * local_sz);
    for (int i = 0; i < local_sz; i ++){
        r[i] = gsl_rng_alloc(T);        
        get_urandom(&seed[i]);
        gsl_rng_set(r[i], seed[i]);
    } 
}

/**
 * distribute sim core and sim node jobs for mpi 
 * 
 */
void
distribute_job() {
    /* distribute sim core job*/
    total_core_num = core_num * node_num;
    int average = total_core_num / comm_sz; 
	int remain = total_core_num % comm_sz;  	
	if (my_rank < remain) {
		local_sz = average + 1;
		local_start = (average + 1) * my_rank;
	} else {
		local_sz = average;
		local_start = (average + 1) * remain + average * (my_rank - remain);
	}
    all_local_sz = (int*) malloc(sizeof(int) * comm_sz);
    all_displs= (int*) malloc(sizeof(int) * comm_sz);
    memset(all_local_sz, 0, comm_sz * sizeof(int));
    memset(all_displs, 0, comm_sz * sizeof(int));
    MPI_Allgather(&local_sz, 1, MPI_INT, all_local_sz, 1, MPI_INT, MPI_COMM_WORLD);
    for ( int j = 1; j < comm_sz ; j ++) {
        all_displs[j] += all_displs[j-1] + all_local_sz[j-1];
    }

    /* distribute sim node job */
    all_node_sz = (int*) malloc(sizeof(int) * comm_sz);
    all_node_displs = (int*) malloc(sizeof(int) * comm_sz);
    average = node_num / comm_sz; 
	remain = node_num % comm_sz;  	
	if(my_rank < remain){
		local_node_sz = (average + 1);
		local_node_start = (average + 1) * my_rank;
	}
	else{
		local_node_sz = average;
		local_node_start = (average + 1) * remain + average * (my_rank - remain);
	}
    memset(all_node_sz, 0, comm_sz * sizeof(int));
    memset(all_node_displs, 0, comm_sz * sizeof(int));
	for (int i = 0; i < comm_sz; i ++){
		if(i < remain){
			all_node_sz[i] = (average + 1) * core_num;
			all_node_displs[i] = (average + 1) * i * core_num;
		}
		else{
			all_node_sz[i] = average * core_num;
			all_node_displs[i] = (average + 1) * remain * core_num + average * (i - remain) * core_num;
		}
	}
}

/**
 * init runtime
 * 
 * @param mgraph  pointers to program graph
 */
void
init_running_time(graph **mgraph){
    for (int i = 0; i < local_sz; i ++){
        for (int j = 0; j < mgraph[i]->node_num; j ++){
            mgraph[i]->node[j]->total_mrunning_time = 0;
            mgraph[i]->node[j]->mrunning_time = 0;
        }
    }
}

int 
main(int argc, char **argv){
    /* init mpi and prase configuation*/
    graph **m_graph;
    Init(argc, argv, &m_graph);

    /* get theoretical time. */
    get_std_time(m_graph[0]);

    if (my_rank == 0) print_pragram_tree(m_graph[0]->root_node);

    uint64_t *local_total_run_time = (uint64_t*) malloc(sizeof(uint64_t) * local_sz);

    for (int i = 0; i < arguments.runnum; i ++) {
        /* init */
        Init_gsl();//init gsl   
        init_running_time(m_graph);//init running time
        memset(local_total_run_time, 0, sizeof(uint64_t) * local_sz);

        /* add noise */
        add_node_noise(m_graph); //add node noise.
        get_real_time(m_graph, local_total_run_time); //add kernels noise and os noise

        /*print run time*/
        if (my_rank == 0) {
            printf("[SIMBOT INFO] %d, simulated time: %llu\n", i + 1, local_total_run_time[0]);
        }

        /* print detail running time*/ 
        print_running_time(m_graph, i);
        
        /* clean */
        free(seed);
        for(int i = 0; i < local_sz; i++){
            gsl_rng_free(r[i]);
        }
    }

    /* clean */
    for (int i = 0; i < local_sz; i ++){
       clear_graph(m_graph[i]);
    }

    free(local_total_run_time);
    free(all_local_sz);
    free(all_displs);
    free(all_node_sz);
    free(all_node_displs);
    free(last_sync_time);
    free_os_noise();
    MPI_Finalize();
}
