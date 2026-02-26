#include "editor_widget_blueprint.h"
#include <stdlib.h>
#include <stdio.h>

EditorWidgetBlueprint *editor_widget_blueprint_create(void) {
    EditorWidgetBlueprint *w = (EditorWidgetBlueprint *)malloc(sizeof(EditorWidgetBlueprint));
    w->graph = blueprint_graph_create();
    w->selected_node = -1;
    w->is_active = 1;
    return w;
}

void editor_widget_blueprint_destroy(EditorWidgetBlueprint *w) {
    if (w) {
        blueprint_graph_destroy(w->graph);
        free(w);
    }
}

void editor_widget_blueprint_draw(const EditorWidgetBlueprint *w) {
    if (!w || !w->graph) return;
    printf("--- Blueprint Editor ---\n");
    for (int i = 0; i < w->graph->node_count; ++i) {
        printf("Node %d: %s (%d,%d)\n", i, w->graph->nodes[i]->name, w->graph->nodes[i]->x, w->graph->nodes[i]->y);
    }
}

void editor_widget_blueprint_handle_input(EditorWidgetBlueprint *w, int key) {
    (void)w;
    (void)key;
    // Placeholder for input handling
}
