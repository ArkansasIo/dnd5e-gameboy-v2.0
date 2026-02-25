#ifndef PROJECT_H
#define PROJECT_H

#include <stdbool.h>

#define PROJECT_MAX_FILES 256
#define PROJECT_NAME_MAX 64

// Project file entry
typedef struct {
    char path[256];
    bool is_open;
} ProjectFileEntry;

// Project structure
typedef struct {
    char name[PROJECT_NAME_MAX];
    ProjectFileEntry files[PROJECT_MAX_FILES];
    int file_count;
} Project;

Project *project_create(const char *name);
void project_destroy(Project *proj);
bool project_add_file(Project *proj, const char *path);
bool project_remove_file(Project *proj, const char *path);
ProjectFileEntry *project_find_file(Project *proj, const char *path);

#endif // PROJECT_H
