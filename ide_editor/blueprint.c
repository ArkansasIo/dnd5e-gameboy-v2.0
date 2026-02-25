#include "blueprint.h"
#include <stdlib.h>
#include <string.h>

BlueprintGraph *blueprint_graph_create(void) {
    BlueprintGraph *graph = (BlueprintGraph *)malloc(sizeof(BlueprintGraph));
    graph->node_count = 0;
    return graph;
}

void blueprint_graph_destroy(BlueprintGraph *graph) {
    for (int i = 0; i < graph->node_count; ++i) {
        blueprint_node_destroy(graph->nodes[i]);
    }
    free(graph);
}

BlueprintNode *blueprint_node_create(BlueprintNodeType type, const char *name, int x, int y) {
    BlueprintNode *node = (BlueprintNode *)malloc(sizeof(BlueprintNode));
    node->type = type;
    strncpy(node->name, name, 63);
    node->name[63] = '\0';
    node->x = x;
    node->y = y;
    node->input_count = 0;
    node->output_count = 0;
    for (int i = 0; i < 4; ++i) {
        node->inputs[i] = NULL;
        node->outputs[i] = NULL;
    }
    return node;
}

void blueprint_node_destroy(BlueprintNode *node) {
    free(node);
}

void blueprint_graph_add_node(BlueprintGraph *graph, BlueprintNode *node) {
    if (graph->node_count < 128) {
        graph->nodes[graph->node_count++] = node;
    }
}
