#ifndef EDITOR_TOOLS_H
#define EDITOR_TOOLS_H

#include <stdbool.h>

#define EDITOR_MAX_TOOLS 64
#define EDITOR_TOOL_NAME_MAX 64
#define EDITOR_TOOL_CAT_MAX 32

typedef struct {
    int id;
    char name[EDITOR_TOOL_NAME_MAX];
    char category[EDITOR_TOOL_CAT_MAX];
    char shortcut[16];
    bool enabled;
} EditorToolDef;

typedef struct {
    EditorToolDef tools[EDITOR_MAX_TOOLS];
    int count;
    int active_tool_id;
} EditorToolRegistry;

void editor_tools_init(EditorToolRegistry *r);
int editor_tools_register(EditorToolRegistry *r, const char *name, const char *category, const char *shortcut, bool enabled);
bool editor_tools_set_active(EditorToolRegistry *r, int tool_id);
EditorToolDef *editor_tools_find(EditorToolRegistry *r, int tool_id);
int editor_tools_count_by_category(const EditorToolRegistry *r, const char *category);

#endif
