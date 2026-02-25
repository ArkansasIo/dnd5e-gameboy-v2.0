#include "menu_interaction.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

static void cpy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

static int find_top(const InteractiveMenuState *s, const char *name) {
    int i;
    if (!s || !name) return -1;
    for (i = 0; i < s->top_count; i++) {
        if (strcmp(s->tops[i].name, name) == 0) return i;
    }
    return -1;
}

static int appendf(char *buf, int cap, int pos, const char *fmt, ...) {
    va_list args;
    int wrote;
    if (!buf || cap <= 0 || pos >= cap) return pos;
    va_start(args, fmt);
    wrote = _vsnprintf(buf + pos, (size_t)(cap - pos), fmt, args);
    va_end(args);
    if (wrote < 0) return cap - 1;
    pos += wrote;
    if (pos >= cap) pos = cap - 1;
    buf[pos] = '\0';
    return pos;
}

void menu_interaction_init(InteractiveMenuState *s) {
    if (!s) return;
    memset(s, 0, sizeof(*s));
}

void menu_interaction_build(InteractiveMenuState *s, const EditorMenu *menu) {
    int i;
    if (!s || !menu) return;
    menu_interaction_init(s);

    for (i = 0; i < menu_item_count(menu); i++) {
        const char *path = NULL;
        const char *cmd = NULL;
        char full[128];
        char *slash;
        char *top_name;
        char *leaf_name;
        int top_idx;
        InteractiveMenuTop *top;

        if (!menu_get_item(menu, i, &path, &cmd)) continue;
        cpy(full, (int)sizeof(full), path);
        slash = strchr(full, '/');
        if (slash) {
            *slash = '\0';
            top_name = full;
            leaf_name = slash + 1;
        } else {
            top_name = full;
            leaf_name = full;
        }

        top_idx = find_top(s, top_name);
        if (top_idx < 0) {
            if (s->top_count >= INTERACTIVE_MENU_MAX_TOP) continue;
            top_idx = s->top_count++;
            cpy(s->tops[top_idx].name, (int)sizeof(s->tops[top_idx].name), top_name);
            s->tops[top_idx].leaf_count = 0;
        }

        top = &s->tops[top_idx];
        if (top->leaf_count >= INTERACTIVE_MENU_MAX_ITEMS) continue;
        cpy(top->leaves[top->leaf_count].label, (int)sizeof(top->leaves[top->leaf_count].label), leaf_name);
        cpy(top->leaves[top->leaf_count].command_id, (int)sizeof(top->leaves[top->leaf_count].command_id), cmd);
        top->leaf_count++;
    }
}

void menu_interaction_toggle(InteractiveMenuState *s) {
    if (!s) return;
    s->active = !s->active;
    if (s->selected_top < 0) s->selected_top = 0;
    if (s->selected_leaf < 0) s->selected_leaf = 0;
}

void menu_interaction_close(InteractiveMenuState *s) {
    if (!s) return;
    s->active = false;
}

void menu_interaction_move_left(InteractiveMenuState *s) {
    if (!s || s->top_count <= 0) return;
    s->selected_top--;
    if (s->selected_top < 0) s->selected_top = s->top_count - 1;
    s->selected_leaf = 0;
}

void menu_interaction_move_right(InteractiveMenuState *s) {
    if (!s || s->top_count <= 0) return;
    s->selected_top++;
    if (s->selected_top >= s->top_count) s->selected_top = 0;
    s->selected_leaf = 0;
}

void menu_interaction_move_up(InteractiveMenuState *s) {
    InteractiveMenuTop *top;
    if (!s || s->top_count <= 0) return;
    top = &s->tops[s->selected_top];
    if (top->leaf_count <= 0) return;
    s->selected_leaf--;
    if (s->selected_leaf < 0) s->selected_leaf = top->leaf_count - 1;
}

void menu_interaction_move_down(InteractiveMenuState *s) {
    InteractiveMenuTop *top;
    if (!s || s->top_count <= 0) return;
    top = &s->tops[s->selected_top];
    if (top->leaf_count <= 0) return;
    s->selected_leaf++;
    if (s->selected_leaf >= top->leaf_count) s->selected_leaf = 0;
}

const char *menu_interaction_activate(const InteractiveMenuState *s) {
    const InteractiveMenuTop *top;
    if (!s || s->top_count <= 0) return NULL;
    top = &s->tops[s->selected_top];
    if (top->leaf_count <= 0 || s->selected_leaf < 0 || s->selected_leaf >= top->leaf_count) return NULL;
    return top->leaves[s->selected_leaf].command_id;
}

void menu_interaction_append_overlay(const InteractiveMenuState *s, char *out, int out_len) {
    int pos = 0;
    int i;
    const InteractiveMenuTop *top;
    if (!s || !out || out_len <= 0 || !s->active || s->top_count <= 0) return;
    top = &s->tops[s->selected_top];

    pos = appendf(out, out_len, pos, "\n\n[Interactive Menu Mode]\n");
    pos = appendf(out, out_len, pos, "TAB close, Arrows navigate, ENTER execute, ESC cancel\n");
    pos = appendf(out, out_len, pos, "Top: ");
    for (i = 0; i < s->top_count; i++) {
        pos = appendf(out, out_len, pos, (i == s->selected_top) ? " [%s] " : " %s ", s->tops[i].name);
    }
    pos = appendf(out, out_len, pos, "\nSubmenu (%s):\n", top->name);
    for (i = 0; i < top->leaf_count && i < 12; i++) {
        pos = appendf(out, out_len, pos, "  %c %s\n", (i == s->selected_leaf) ? '>' : ' ', top->leaves[i].label);
    }
}
