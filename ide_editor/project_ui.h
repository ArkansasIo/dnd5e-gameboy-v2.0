#ifndef PROJECT_UI_H
#define PROJECT_UI_H

#include "project.h"

// Project management UI widget
typedef struct {
    Project *project;
    int selected_file;
    int is_active;
} ProjectUI;

ProjectUI *project_ui_create(const char *project_name);
void project_ui_destroy(ProjectUI *ui);
void project_ui_draw(const ProjectUI *ui);
void project_ui_handle_input(ProjectUI *ui, int key);

#endif // PROJECT_UI_H
