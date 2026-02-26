#include "sidebar.h"
#include <stdlib.h>
#include <string.h>

struct EditorSidebar {
    int item_count;
    char labels[SIDEBAR_MAX_ITEMS][96];
};

EditorSidebar *sidebar_create(void) {
    EditorSidebar *sidebar = (EditorSidebar *)malloc(sizeof(EditorSidebar));
    if (!sidebar) return NULL;
    sidebar->item_count = 0;
    return sidebar;
}

void sidebar_destroy(EditorSidebar *sidebar) {
    free(sidebar);
}

bool sidebar_add_item(EditorSidebar *sidebar, const char *label) {
    if (!sidebar || !label || sidebar->item_count >= SIDEBAR_MAX_ITEMS) return false;
    strncpy(sidebar->labels[sidebar->item_count], label, sizeof(sidebar->labels[0]) - 1);
    sidebar->labels[sidebar->item_count][sizeof(sidebar->labels[0]) - 1] = '\0';
    sidebar->item_count++;
    return true;
}

int sidebar_item_count(const EditorSidebar *sidebar) {
    return sidebar ? sidebar->item_count : 0;
}
