#include "window.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct EditorWindow {
    char title[64];
    int width, height;
    bool open;
};

EditorWindow *window_create(const char *title, int width, int height) {
    EditorWindow *win = (EditorWindow *)malloc(sizeof(EditorWindow));
    snprintf(win->title, 64, "%s", title);
    win->width = width;
    win->height = height;
    win->open = true;
    return win;
}

void window_show(EditorWindow *win) {
    printf("Showing window: %s (%dx%d)\n", win->title, win->width, win->height);
}

bool window_is_open(EditorWindow *win) {
    return win->open;
}

void window_poll_events(EditorWindow *win) {
    (void)win;
    // Placeholder for event polling
}

void window_render(EditorWindow *win) {
    (void)win;
    // Placeholder for rendering
}

void window_destroy(EditorWindow *win) {
    free(win);
}
