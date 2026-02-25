#include "project.h"
#include <stdlib.h>
#include <string.h>

Project *project_create(const char *name) {
    Project *proj = (Project *)malloc(sizeof(Project));
    strncpy(proj->name, name, PROJECT_NAME_MAX-1);
    proj->name[PROJECT_NAME_MAX-1] = '\0';
    proj->file_count = 0;
    return proj;
}

void project_destroy(Project *proj) {
    free(proj);
}

bool project_add_file(Project *proj, const char *path) {
    if (proj->file_count >= PROJECT_MAX_FILES) return false;
    strncpy(proj->files[proj->file_count].path, path, 255);
    proj->files[proj->file_count].path[255] = '\0';
    proj->files[proj->file_count].is_open = false;
    proj->file_count++;
    return true;
}

bool project_remove_file(Project *proj, const char *path) {
    for (int i = 0; i < proj->file_count; ++i) {
        if (strcmp(proj->files[i].path, path) == 0) {
            for (int j = i; j < proj->file_count-1; ++j) {
                proj->files[j] = proj->files[j+1];
            }
            proj->file_count--;
            return true;
        }
    }
    return false;
}

ProjectFileEntry *project_find_file(Project *proj, const char *path) {
    for (int i = 0; i < proj->file_count; ++i) {
        if (strcmp(proj->files[i].path, path) == 0) {
            return &proj->files[i];
        }
    }
    return NULL;
}
