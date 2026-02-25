#include "project_ui.h"
#include <stdlib.h>
#include <stdio.h>

ProjectUI *project_ui_create(const char *project_name) {
    ProjectUI *ui = (ProjectUI *)malloc(sizeof(ProjectUI));
    ui->project = project_create(project_name);
    ui->selected_file = -1;
    ui->is_active = 1;
    return ui;
}

void project_ui_destroy(ProjectUI *ui) {
    if (ui) {
        project_destroy(ui->project);
        free(ui);
    }
}

void project_ui_draw(const ProjectUI *ui) {
    if (!ui || !ui->project) return;
    printf("--- Project: %s ---\n", ui->project->name);
    for (int i = 0; i < ui->project->file_count; ++i) {
        printf("%s%s\n", (i == ui->selected_file) ? "> " : "  ", ui->project->files[i].path);
    }
}

void project_ui_handle_input(ProjectUI *ui, int key) {
    (void)ui;
    (void)key;
    // Placeholder for input handling
}
