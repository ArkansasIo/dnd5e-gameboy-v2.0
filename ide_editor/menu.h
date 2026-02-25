#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

#define MENU_MAX_ITEMS 64

typedef struct EditorMenu EditorMenu;

EditorMenu *menu_create(void);
void menu_destroy(EditorMenu *menu);
bool menu_add_item(EditorMenu *menu, const char *path, const char *command_id);
int menu_item_count(const EditorMenu *menu);
bool menu_get_item(const EditorMenu *menu, int index, const char **path, const char **command_id);
const char *menu_find_command(const EditorMenu *menu, const char *path);

#endif // MENU_H
