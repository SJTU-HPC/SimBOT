#include "print_structure.h"
#include "frame_init.h"
#include "command.h"

/**
 * generate program praph
 * 
 * @param structure_graph pointer to program graph
 * @param config_json pointer to program configuration json
 */
void
generate_call_structure(graph *structure_graph, cJSON *config_json){
    /* init structure_graph */
    structure_graph->node = (graph_node**) malloc(sizeof(graph_node*) * MAX_NODE_NUM); 
    structure_graph->node_num = 0;
    structure_graph->root_node = NULL;

    init_nodes(structure_graph, config_json);//add nodes
    init_edges(structure_graph, config_json); //add edges  
}

void
__parse_call_structure(graph_node *node, char **outbuf_ptr) {
    if (node->child_num == 0) {
        return;
    }
    memcpy(outbuf_ptr[0], node->name, strlen(node->name) * sizeof(char));
    outbuf_ptr[0] += strlen(node->name);
    outbuf_ptr[0][0] = '(';
    outbuf_ptr[0] ++;
    outbuf_ptr[0][0] = ')';
    outbuf_ptr[0] ++;
    outbuf_ptr[0][0] = ' ';
    outbuf_ptr[0] ++;
    outbuf_ptr[0][0] = '{';
    outbuf_ptr[0] += 1;
    outbuf_ptr[0][0] = '\n';
    outbuf_ptr[0] += 1;
    for (int i = 0; i < node->child_num; i ++) {
        /* Number of function the loop contain */
        int count = 0;
        if (node->loop_nums[i] > 1) {
            int j = i;
            while (j < node->child_num) {
                if(node->loop_nums[j] > 1){
                    count ++;
                }
                j ++;
            }
        }
        if (count > 0) {
            /* add loop to outbuf */
            char str[50] = {};
            sprintf(str,"    loop(%d) {\n",node->loop_nums[i]);
            int len = strlen(str);
            memcpy(outbuf_ptr[0], str, len);
            outbuf_ptr[0] += len;
            for (int j = 0; j < count; j++){
                for (int k = 0; k < 8; k ++){
                    outbuf_ptr[0][0] = ' ';
                    outbuf_ptr[0] ++;
                }
                len = strlen(node->childs[i + j]->name);
                memcpy(outbuf_ptr[0], node->childs[i + j]->name, len);
                outbuf_ptr[0] += len;
                outbuf_ptr[0][0] = ';';
                outbuf_ptr[0] += 1;
                outbuf_ptr[0][0] = '\n';
                outbuf_ptr[0] += 1;
            }
            for (int k = 0; k < 4; k ++){
                outbuf_ptr[0][0] = ' ';
                outbuf_ptr[0] ++;
            }
            outbuf_ptr[0][0] = '}';
            outbuf_ptr[0] ++;
            outbuf_ptr[0][0] = '\n';
            outbuf_ptr[0] ++;
            i += count;
        }else{
            for (int k = 0; k < 4; k ++){
                outbuf_ptr[0][0] = ' ';
                outbuf_ptr[0] ++;
            }
            int len = strlen(node->childs[i]->name);
            memcpy(outbuf_ptr[0], node->childs[i]->name, len);
            outbuf_ptr[0] += len;
            outbuf_ptr[0][0] = ';';
            outbuf_ptr[0] += 1;
            outbuf_ptr[0][0] = '\n';
            outbuf_ptr[0] += 1; 
        }

    }
    outbuf_ptr[0][0] = '}';
    outbuf_ptr[0] += 1;
    outbuf_ptr[0][0] = '\n';
    outbuf_ptr[0] += 1;
    outbuf_ptr[0][0] = '\n';
    outbuf_ptr[0] += 1;

    for (int i = 0; i < node->child_num; i ++){
        __parse_call_structure(node->childs[i], outbuf_ptr);
    }
}

/**
 * Parse function call structure.
 * 
 * @param structure_graph pointer to program graph
 * @param outbuf_ptr pointer to output buffer
 */
void
parse_call_structure(graph *structure_graph, char *outbuf_ptr){
    __parse_call_structure(structure_graph->root_node, &outbuf_ptr);
}

/**
 * Parse config file and output to file.
 * @param config_buf pointer to configuration 
 */
void
print_structure(cJSON *config_json) {
    graph *structure_graph = (graph*) malloc(sizeof(graph));
    char *outbuf = (char*) malloc(sizeof(char) * MAX_FILE_SIZE);
    memset(outbuf, 0, sizeof(char) * MAX_FILE_SIZE);
    generate_call_structure(structure_graph, config_json);
    parse_call_structure(structure_graph, outbuf);


    FILE *file;
    char filename[MAX_FILE_NAME];
    sprintf(filename, "%s/%s", arguments.outfile, "structure.log");
    file = fopen(filename,  "w");
    fprintf(file, "%s", outbuf);

    fflush(file);

    /* clean */
    fclose(file);
    free(outbuf);
    clear_graph(structure_graph);
}