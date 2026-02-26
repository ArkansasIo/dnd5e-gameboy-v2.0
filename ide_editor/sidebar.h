#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <stdbool.h>

#define SIDEBAR_MAX_ITEMS 128

typedef struct EditorSidebar EditorSidebar;

EditorSidebar *sidebar_create(void);
void sidebar_destroy(EditorSidebar *sidebar);
bool sidebar_add_item(EditorSidebar *sidebar, const char *label);
int sidebar_item_count(const EditorSidebar *sidebar);

#endif // SIDEBAR_H
