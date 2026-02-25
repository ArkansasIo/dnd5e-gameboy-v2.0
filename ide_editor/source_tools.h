#ifndef SOURCE_TOOLS_H
#define SOURCE_TOOLS_H

#include <stdbool.h>
#include "editor_file.h"

#define SOURCE_MAX_FILES 1024

typedef struct {
    char path[260];
    char name[96];
    char ext[16];
} SourceFileEntry;

typedef struct {
    char root[260];
    SourceFileEntry files[SOURCE_MAX_FILES];
    int file_count;
    int active_index;
    EditorFileBuffer *active_buffer;
    char active_path[260];
    char last_status[160];
} SourceWorkspace;

void source_workspace_init(SourceWorkspace *ws, const char *root);
void source_workspace_free(SourceWorkspace *ws);
bool source_workspace_scan(SourceWorkspace *ws);
bool source_workspace_next(SourceWorkspace *ws);
bool source_workspace_prev(SourceWorkspace *ws);
const SourceFileEntry *source_workspace_active(const SourceWorkspace *ws);
bool source_workspace_open_active(SourceWorkspace *ws);
bool source_workspace_save_active(SourceWorkspace *ws);
bool source_workspace_compile_active(SourceWorkspace *ws, const char *mode);

#endif
