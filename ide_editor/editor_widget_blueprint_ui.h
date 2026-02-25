#ifndef EDITOR_WIDGET_BLUEPRINT_UI_H
#define EDITOR_WIDGET_BLUEPRINT_UI_H

#include "editor_widget_blueprint.h"

// Blueprint editor UI wrapper
typedef struct {
    EditorWidgetBlueprint *widget;
    int is_active;
} EditorWidgetBlueprintUI;

EditorWidgetBlueprintUI *editor_widget_blueprint_ui_create(void);
void editor_widget_blueprint_ui_destroy(EditorWidgetBlueprintUI *ui);
void editor_widget_blueprint_ui_draw(const EditorWidgetBlueprintUI *ui);
void editor_widget_blueprint_ui_handle_input(EditorWidgetBlueprintUI *ui, int key);

#endif // EDITOR_WIDGET_BLUEPRINT_UI_H
