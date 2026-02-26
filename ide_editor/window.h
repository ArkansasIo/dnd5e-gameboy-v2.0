#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

typedef struct EditorWindow EditorWindow;

#define WINDOW_UI_MAX_ACTIONS 256

typedef struct {
    int x;
    int y;
    int w;
    int h;
    char label[64];
    char command_id[128];
} WindowUIAction;

EditorWindow *window_create(const char *title, int width, int height);
void window_show(EditorWindow *win);
bool window_is_open(EditorWindow *win);
void window_poll_events(EditorWindow *win);
void window_render(EditorWindow *win);
void window_set_overlay_text(EditorWindow *win, const char *text);
void window_set_loading_screen(EditorWindow *win, bool active, const char *title, const char *subtitle, int progress_percent);
void window_set_ui_actions(EditorWindow *win, const WindowUIAction *actions, int count);
bool window_pop_ui_action(EditorWindow *win, char *out_command, int out_len);
void window_destroy(EditorWindow *win);

#endif // WINDOW_H
