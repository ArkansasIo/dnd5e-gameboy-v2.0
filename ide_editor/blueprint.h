#ifndef BLUEPRINT_H
#define BLUEPRINT_H

#include <stdint.h>
#include <stdbool.h>

// Blueprint Node Types
typedef enum {
    BP_NODE_EVENT,
    BP_NODE_FUNCTION,
    BP_NODE_VARIABLE,
    BP_NODE_BRANCH,
    BP_NODE_CUSTOM,
    BP_NODE_COMMENT
} BlueprintNodeType;

// Blueprint Node
typedef struct BlueprintNode {
    BlueprintNodeType type;
    char name[64];
    int x, y; // Position in editor
    struct BlueprintNode *inputs[4];
    struct BlueprintNode *outputs[4];
    int input_count;
    int output_count;
    // Additional data for node
} BlueprintNode;

// Blueprint Graph
typedef struct {
    BlueprintNode *nodes[128];
    int node_count;
} BlueprintGraph;

BlueprintGraph *blueprint_graph_create(void);
void blueprint_graph_destroy(BlueprintGraph *graph);
BlueprintNode *blueprint_node_create(BlueprintNodeType type, const char *name, int x, int y);
void blueprint_node_destroy(BlueprintNode *node);
void blueprint_graph_add_node(BlueprintGraph *graph, BlueprintNode *node);

#endif // BLUEPRINT_H
