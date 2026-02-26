#include "editor_tools.h"
#include <string.h>

static void s_copy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

void editor_tools_init(EditorToolRegistry *r) {
    if (!r) return;
    r->count = 0;
    r->active_tool_id = -1;
}

int editor_tools_register(EditorToolRegistry *r, const char *name, const char *category, const char *shortcut, bool enabled) {
    EditorToolDef *t;
    if (!r || r->count >= EDITOR_MAX_TOOLS) return -1;
    t = &r->tools[r->count];
    t->id = r->count + 1;
    s_copy(t->name, EDITOR_TOOL_NAME_MAX, name);
    s_copy(t->category, EDITOR_TOOL_CAT_MAX, category);
    s_copy(t->shortcut, 16, shortcut);
    t->enabled = enabled;
    r->count++;
    if (r->active_tool_id < 0 && enabled) r->active_tool_id = t->id;
    return t->id;
}

bool editor_tools_set_active(EditorToolRegistry *r, int tool_id) {
    EditorToolDef *t = editor_tools_find(r, tool_id);
    if (!t || !t->enabled) return false;
    r->active_tool_id = tool_id;
    return true;
}

EditorToolDef *editor_tools_find(EditorToolRegistry *r, int tool_id) {
    int i;
    if (!r) return NULL;
    for (i = 0; i < r->count; i++) {
        if (r->tools[i].id == tool_id) return &r->tools[i];
    }
    return NULL;
}

int editor_tools_count_by_category(const EditorToolRegistry *r, const char *category) {
    int i, n = 0;
    if (!r || !category) return 0;
    for (i = 0; i < r->count; i++) {
        if (strcmp(r->tools[i].category, category) == 0) n++;
    }
    return n;
}
