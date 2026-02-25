#ifndef MENU_INTERACTION_H
#define MENU_INTERACTION_H

#include <stdbool.h>
#include "menu.h"

#define INTERACTIVE_MENU_MAX_TOP 32
#define INTERACTIVE_MENU_MAX_ITEMS 96

typedef struct {
    char label[80];
    char command_id[64];
} InteractiveMenuLeaf;

typedef struct {
    char name[32];
    InteractiveMenuLeaf leaves[INTERACTIVE_MENU_MAX_ITEMS];
    int leaf_count;
} InteractiveMenuTop;

typedef struct {
    InteractiveMenuTop tops[INTERACTIVE_MENU_MAX_TOP];
    int top_count;
    bool active;
    int selected_top;
    int selected_leaf;
} InteractiveMenuState;

void menu_interaction_init(InteractiveMenuState *s);
void menu_interaction_build(InteractiveMenuState *s, const EditorMenu *menu);
void menu_interaction_toggle(InteractiveMenuState *s);
void menu_interaction_close(InteractiveMenuState *s);
void menu_interaction_move_left(InteractiveMenuState *s);
void menu_interaction_move_right(InteractiveMenuState *s);
void menu_interaction_move_up(InteractiveMenuState *s);
void menu_interaction_move_down(InteractiveMenuState *s);
const char *menu_interaction_activate(const InteractiveMenuState *s);
void menu_interaction_append_overlay(const InteractiveMenuState *s, char *out, int out_len);

#endif
