#include "menu.h"
#include <stdlib.h>
#include <string.h>

struct EditorMenu {
    int item_count;
    char paths[MENU_MAX_ITEMS][96];
    char command_ids[MENU_MAX_ITEMS][48];
};

EditorMenu *menu_create(void) {
    EditorMenu *menu = (EditorMenu *)malloc(sizeof(EditorMenu));
    if (!menu) return NULL;
    menu->item_count = 0;
    return menu;
}
void menu_destroy(EditorMenu *menu) {
    free(menu);
}

bool menu_add_item(EditorMenu *menu, const char *path, const char *command_id) {
    if (!menu || !path || !command_id || menu->item_count >= MENU_MAX_ITEMS) return false;
    strncpy(menu->paths[menu->item_count], path, sizeof(menu->paths[0]) - 1);
    menu->paths[menu->item_count][sizeof(menu->paths[0]) - 1] = '\0';
    strncpy(menu->command_ids[menu->item_count], command_id, sizeof(menu->command_ids[0]) - 1);
    menu->command_ids[menu->item_count][sizeof(menu->command_ids[0]) - 1] = '\0';
    menu->item_count++;
    return true;
}

int menu_item_count(const EditorMenu *menu) {
    return menu ? menu->item_count : 0;
}

bool menu_get_item(const EditorMenu *menu, int index, const char **path, const char **command_id) {
    if (!menu || index < 0 || index >= menu->item_count) return false;
    if (path) *path = menu->paths[index];
    if (command_id) *command_id = menu->command_ids[index];
    return true;
}

const char *menu_find_command(const EditorMenu *menu, const char *path) {
    int i;
    if (!menu || !path) return NULL;
    for (i = 0; i < menu->item_count; i++) {
        if (strcmp(menu->paths[i], path) == 0) return menu->command_ids[i];
    }
    return NULL;
}
