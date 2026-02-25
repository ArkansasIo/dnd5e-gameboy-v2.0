#include "editor_widget_code_ui.h"
#include <stdlib.h>
#include <stdio.h>

EditorWidgetCodeUI *editor_widget_code_ui_create(const char *filename) {
    EditorWidgetCodeUI *ui = (EditorWidgetCodeUI *)malloc(sizeof(EditorWidgetCodeUI));
    ui->widget = editor_widget_code_create(filename);
    ui->is_active = 1;
    return ui;
}

void editor_widget_code_ui_destroy(EditorWidgetCodeUI *ui) {
    if (ui) {
        editor_widget_code_destroy(ui->widget);
        free(ui);
    }
}

void editor_widget_code_ui_draw(const EditorWidgetCodeUI *ui) {
    if (!ui || !ui->widget) return;
    editor_widget_code_draw(ui->widget);
}

void editor_widget_code_ui_handle_input(EditorWidgetCodeUI *ui, int key) {
    (void)ui;
    (void)key;
    // Placeholder for input handling
}
