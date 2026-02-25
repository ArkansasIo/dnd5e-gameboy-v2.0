#include "editor_widget_blueprint_ui.h"
#include <stdlib.h>
#include <stdio.h>

EditorWidgetBlueprintUI *editor_widget_blueprint_ui_create(void) {
    EditorWidgetBlueprintUI *ui = (EditorWidgetBlueprintUI *)malloc(sizeof(EditorWidgetBlueprintUI));
    ui->widget = editor_widget_blueprint_create();
    ui->is_active = 1;
    return ui;
}

void editor_widget_blueprint_ui_destroy(EditorWidgetBlueprintUI *ui) {
    if (ui) {
        editor_widget_blueprint_destroy(ui->widget);
        free(ui);
    }
}

void editor_widget_blueprint_ui_draw(const EditorWidgetBlueprintUI *ui) {
    if (!ui || !ui->widget) return;
    editor_widget_blueprint_draw(ui->widget);
}

void editor_widget_blueprint_ui_handle_input(EditorWidgetBlueprintUI *ui, int key) {
    (void)ui;
    (void)key;
    // Placeholder for input handling
}
