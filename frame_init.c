#include "frame_init.h"
#include "print_structure.h"
#include "common.h"

/*
 * add edge to program graph
 * 
 * @param child_name
 * @param parent_name
 * @param loop_num
 * @param m_graph
 * @return int
 */
int add_edge(char *child_name, char *parent_name, int loop_num, graph *m_graph) {
	graph_node *child;
	graph_node *parent;
	child = find_node(child_name, m_graph);
	parent = find_node(parent_name, m_graph);
	if(child == NULL){
		printf("[Error] could not find %s, please check your config file.\n", child_name);
		exit(-1);
	}
	if(parent == NULL){
		printf("[Error] could not find %s, please check your config file.\n", parent_name);
		exit(-1);
	}
	child->parents[child->parent_num] = parent;
	child->parent_num ++;
	parent->childs[parent->child_num] = child;
	parent->loop_nums[parent->child_num] = loop_num;
	parent->child_num ++;
}

/**
 * add node to program graph
 * 
 * @param mnode_name function/kernel name
 * @param mrun_time standard runtime
 * @param m_noise noise type
 * @param sync sync flag
 * @param record_flag recode flag
 * @param m_graph pointer to program graph
 * @return int 
 */
int add_node(
	const char *mnode_name,
	uint64_t mrun_time,
	noise *m_noise,
	int sync,
	int record_flag,
	int type_flag,
	graph *m_graph)
{
	graph_node *new_node = (graph_node*) malloc(sizeof(graph_node));
	char *node_name = (char*) malloc(sizeof(char) * MAX_NAME_SIZE);
	strcpy(node_name, mnode_name);
	new_node->name = node_name;
	new_node->run_time = mrun_time;
	new_node->std_run_time = mrun_time;	
	new_node->m_noise = m_noise;
	new_node->parents = (graph_node**) malloc(sizeof(graph_node*) * MAX_PARENT_NUM);
	new_node->childs = (graph_node**) malloc(sizeof(graph_node*) * MAX_CHILD_NUM);
	new_node->parent_num = 0;
	new_node->child_num = 0;
	new_node->loop_nums = (int*) malloc(sizeof(int) * MAX_CHILD_NUM);
	new_node->sync = sync;
	new_node->record_flag = record_flag;
	new_node->mrunning_time = 0;
	new_node->total_mrunning_time = 0;
	new_node->m_type = type_flag;
	m_graph->node[m_graph->node_num] = new_node;
	if(m_graph->node_num == 0)
		m_graph->root_node = new_node;
	m_graph->node_num ++;
}

/**
 * clean
 * 
 * @param mNode 
 */
void clear_node(graph_node *mNode){
	if (mNode->name) {
		free(mNode->name);
	}
	if (mNode->m_noise) {
		free(mNode->m_noise->noise_type);
		free(mNode->m_noise->parameters);
	}
	if (mNode->loop_nums) {
		free(mNode->loop_nums);
	}
	if (mNode->parents) {
		free(mNode->parents);
	}
	if (mNode->childs) {
		free(mNode->childs);
	}
}

/**
 * get pointer to node
 * 
 * @param name node name
 * @param m_graph 
 * @return graph_node* pointer to the node
 */
graph_node* find_node(char *name, graph *m_graph){
	for (int i = 0; i < m_graph->node_num; i ++) {
		if (strcmp(m_graph->node[i]->name, name) == 0) {
			return m_graph->node[i];
		}
	}
	return NULL;
}

/**
 * clean
 * 
 * @param m_graph 
 */
void clear_graph(graph *m_graph){
	for (int i = 0; i < m_graph->node_num; i ++) {
		clear_node(m_graph->node[i]);
	}
	if (m_graph->node) {
		free(m_graph->node);
	}
}

/**
 * read file to buffer
 * 
 * @param file_name  file name
 * @param config_buf  buffer
 * @return int 
 */
int read_to_buf(const char *file_name,char *config_buf){
	FILE* fp = fopen(file_name, "r");
	if (!fp) {
        printf("[Error] Config file %s not found.\n", file_name);
        return 0;
    }
    fread(config_buf, MAX_FILE_SIZE, 1, fp);
    fclose(fp);
	return 1;
}

/**
 * parse noise configuration
 * 
 * @param noise_json pointer to noise_json
 * @param m_noise pointer to noise
 */
void parse_noise(cJSON *noise_json, noise **m_noise){
	cJSON *noise_type;
	cJSON *noise_parameter;

	/* check noise */
	if (noise_json == NULL) {
		if (*m_noise) {
			free(*m_noise);
		}
		*m_noise = NULL;
		return;
	}

	/* parse noise */
	noise_type = cJSON_GetObjectItem(noise_json, "noiseType");
	(*m_noise)-> noise_type = (char*) malloc(MAX_NOISE_NAME);
	strcpy((*m_noise)-> noise_type, noise_type->valuestring);
	noise_parameter = cJSON_GetObjectItem(noise_json, "noiseParameters");
	(*m_noise)->parameter_num = cJSON_GetArraySize(noise_parameter);
	(*m_noise)->parameters = (double*)malloc(sizeof(double) * (*m_noise)->parameter_num);
	for (int i = 0;i < (*m_noise)->parameter_num; i ++) {
		(*m_noise)->parameters[i] = cJSON_GetArrayItem(noise_parameter, i)->valuedouble;
	}

}

/**
 * add main node, function nodes,
 * kernel nodes to the program graph
 * 
 * @param m_graph 
 * @param config_json 
 */
void init_nodes(graph *m_graph, cJSON *config_json) {
	/* add main */
	add_node("__main__", 0, NULL, 1, 1, COMPUTE_TYPE, m_graph);

	/* add functions */
	cJSON *functions = cJSON_GetObjectItem(config_json, "functions")->child;
	while (functions != NULL) {
		int sync_flag = 0;
		int record_flag = 0;

		/* check if sync */
		cJSON *sync_Json = cJSON_GetObjectItem(functions, "sync");
		if(sync_Json != NULL){
			if(strcmp(sync_Json->valuestring, "yes") == 0){
				sync_flag = 1;
			}
		}

		/* check if record runtime*/
		cJSON *recode_Json = cJSON_GetObjectItem(functions, "recordTime");
		if(recode_Json != NULL){
			if(strcmp(recode_Json->valuestring, "yes") == 0){
				record_flag = 1;
			}
		}		

		add_node(functions->string, 0, NULL, sync_flag, record_flag, COMPUTE_TYPE, m_graph); //functions has no runntime
		functions = functions->next;//next
	}

	/* add kernels */
	cJSON *kernels = cJSON_GetObjectItem(config_json, "kernels")->child;
	while (kernels != NULL) {
		uint64_t runtime = 0;
		int record_flag = 0;

		/* add runtime */
		noise *m_noise = (noise*) malloc(sizeof(noise));
		parse_noise(cJSON_GetObjectItem(kernels,"noise"), &m_noise);
		cJSON *run_time_json = cJSON_GetObjectItem(kernels, "runTime");
		if(run_time_json != NULL){
			runtime = string_to_uint64(run_time_json->valuestring);
		}

		/* check if record runtime */
	    cJSON *recode_Json = cJSON_GetObjectItem(kernels, "recordTime");
	    if(recode_Json != NULL){
			if(strcmp(recode_Json->valuestring, "yes") == 0){
				record_flag = 1;
			}
		}

		add_node(kernels->string, runtime, m_noise, 0, record_flag, COMPUTE_TYPE, m_graph);
		kernels = kernels->next;// next
	}

	/* add communications */
	cJSON *communications = cJSON_GetObjectItem(config_json, "communications")->child;
	while (communications != NULL) {
		add_node(communications->string, 0, NULL, 0, 0, COMMUNCATE_TYPE, m_graph);
		communications = communications->next; // next
	}
}

/**
 * add edges to program graph
 * 
 * @param functions 
 * @param m_graph 
 */
void add_func_edges(cJSON *functions, graph *m_graph) {
	while (functions != NULL) {
		/* add call */
		cJSON *call = cJSON_GetObjectItem(functions, "call");
		if (call != NULL) {
			int call_num = cJSON_GetArraySize(call);
			char* ptr = functions -> string;
			for(int i = 0; i < call_num; ++i) {
				add_edge(cJSON_GetArrayItem(call, i)->valuestring, ptr, 1, m_graph);
			}
		}

		/* add loop */
		cJSON *loop = cJSON_GetObjectItem(functions, "loop");
		if (loop != NULL) {
			call = cJSON_GetObjectItem(loop, "call");
			cJSON *loop_num_json = cJSON_GetObjectItem(loop, "loopNum");
			if (loop_num_json == NULL) {
				printf("[Error] please set %s's loopNum value.\n", functions->string);
				exit(-1);
			}
			int loop_num = string_to_int(loop_num_json->valuestring);
			if (call != NULL) {
				int call_num = cJSON_GetArraySize(call);
				char *ptr = functions->string;
				for (int i = 0; i < call_num; ++i) {
					add_edge(cJSON_GetArrayItem(call, i)->valuestring, ptr, loop_num, m_graph);
				}
			}
		}

		functions = functions->next;//next function
	}
}

/**
 * init edges of program graph 
 * 
 * @param m_graph 
 * @param config_json 
 */
void init_edges(graph *m_graph, cJSON *program_json) {
	/* add main */
	cJSON *entrypoint = cJSON_GetObjectItem(program_json, "__main__");
	if (entrypoint != NULL) {
		int call_num = cJSON_GetArraySize(entrypoint);
		char *parent = "__main__";
		for (int i = 0; i < call_num; ++i) {
			add_edge(cJSON_GetArrayItem(entrypoint, i)->valuestring, parent, 1, m_graph);
		}
	}

	/* add functions and kernels */
	cJSON *functions = cJSON_GetObjectItem(program_json, "functions")->child;
	cJSON *kernels = cJSON_GetObjectItem(program_json, "kernels")->child;
	add_func_edges(functions, m_graph);
	add_func_edges(kernels, m_graph);
}

/**
 * init program graph
 * 
 * @param m_graph 
 * @param config_buf configuration 
 */
void init_graph(graph ***m_graph, cJSON *program_json) {
	if (program_json == NULL && my_rank == 0) {
		printf("[Error] parse config file fail. Please check your configuration file.\n");
		exit(-1);
	}

	/* init program graph */
	*m_graph = (graph**) malloc(sizeof(graph*) * local_sz);
	for (int i = 0; i < local_sz; i ++) {
		(*m_graph)[i] = (graph*) malloc(sizeof(graph));
		(*m_graph)[i]->node = (graph_node**) malloc(sizeof(graph_node*) * MAX_NODE_NUM);
		(*m_graph)[i]->node_num = 0;
		(*m_graph)[i]->root_node = NULL;
		
		/* add nodes to graph */
		init_nodes((*m_graph)[i], program_json);

		/* add edges to graph */
		init_edges((*m_graph)[i], program_json);
	}
}

/**
 * read program configuration and parse 
 * 
 * @param m_graph 
 * @param program_json 
 */
void parse_program(graph ***m_graph, cJSON *program_json) {
	/* output function call structure */
	if (my_rank == 0) {
    	print_structure(program_json);
	}

	/* init program graph */
	init_graph(m_graph, program_json);

	return;
}

/**
 * parse node configuration and parse 
 * 
 * @param core_json 
 * @param node_json 
 */
void parse_node_config(cJSON* core_json, cJSON* node_json){
	/* parse */
	if (core_json == NULL) {
		printf("[Error] node.json has no core configration.\n");
		exit(-1);
	} else {
		core_num = string_to_int(cJSON_GetObjectItem(core_json, "coreNum")->valuestring);
	}
	if (node_json == NULL) {
		printf("[Error] node.json has no node configration.\n");
		exit(-1);
	} else {
		node_num = string_to_int(cJSON_GetObjectItem(node_json, "nodeNum")->valuestring);
		cJSON* noise_json = cJSON_GetObjectItem(node_json, "noise");
		if (noise_json == NULL) {
			node_noise = NULL;
		} else {
			node_noise = (noise*) malloc(sizeof(noise));
			parse_noise(noise_json, &node_noise);
		}
	}
}
