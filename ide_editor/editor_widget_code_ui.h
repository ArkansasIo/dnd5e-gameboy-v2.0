#ifndef EDITOR_WIDGET_CODE_UI_H
#define EDITOR_WIDGET_CODE_UI_H

#include "editor_widget_code.h"

// Code editor UI wrapper
typedef struct {
    EditorWidgetCode *widget;
    int is_active;
} EditorWidgetCodeUI;

EditorWidgetCodeUI *editor_widget_code_ui_create(const char *filename);
void editor_widget_code_ui_destroy(EditorWidgetCodeUI *ui);
void editor_widget_code_ui_draw(const EditorWidgetCodeUI *ui);
void editor_widget_code_ui_handle_input(EditorWidgetCodeUI *ui, int key);

#endif // EDITOR_WIDGET_CODE_UI_H
