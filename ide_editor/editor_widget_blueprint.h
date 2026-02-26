#ifndef EDITOR_WIDGET_BLUEPRINT_H
#define EDITOR_WIDGET_BLUEPRINT_H

#include "blueprint.h"

// Blueprint visual scripting editor widget
typedef struct {
    BlueprintGraph *graph;
    int selected_node;
    int is_active;
} EditorWidgetBlueprint;

EditorWidgetBlueprint *editor_widget_blueprint_create(void);
void editor_widget_blueprint_destroy(EditorWidgetBlueprint *w);
void editor_widget_blueprint_draw(const EditorWidgetBlueprint *w);
void editor_widget_blueprint_handle_input(EditorWidgetBlueprint *w, int key);

#endif // EDITOR_WIDGET_BLUEPRINT_H
