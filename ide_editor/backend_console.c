#include "editor_backend.h"
#include "window.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    EditorWindow *window;
} ConsoleBackendState;

typedef struct {
    bool (*init)(void *state, const char *title, int width, int height);
    void (*poll)(void *state);
    void (*render)(void *state);
    void (*set_overlay)(void *state, const char *text);
    void (*set_loading_screen)(void *state, bool active, const char *title, const char *subtitle, int progress_percent);
    void (*set_ui_actions)(void *state, const EditorUIAction *actions, int count);
    bool (*pop_ui_action)(void *state, char *out_command, int out_len);
    bool (*is_running)(void *state);
    void (*shutdown)(void *state);
    const char *name;
    size_t state_size;
} EditorBackendOps;

static bool c_init(void *state, const char *title, int width, int height) {
    ConsoleBackendState *s = (ConsoleBackendState *)state;
    s->window = window_create(title, width, height);
    if (!s->window) return false;
    window_show(s->window);
    return true;
}

static void c_poll(void *state) {
    ConsoleBackendState *s = (ConsoleBackendState *)state;
    window_poll_events(s->window);
}

static void c_render(void *state) {
    ConsoleBackendState *s = (ConsoleBackendState *)state;
    window_render(s->window);
}

static void c_set_overlay(void *state, const char *text) {
    ConsoleBackendState *s = (ConsoleBackendState *)state;
    window_set_overlay_text(s->window, text);
}

static void c_set_loading_screen(void *state, bool active, const char *title, const char *subtitle, int progress_percent) {
    ConsoleBackendState *s = (ConsoleBackendState *)state;
    if (!s || !s->window) return;
    window_set_loading_screen(s->window, active, title, subtitle, progress_percent);
}

static void c_set_ui_actions(void *state, const EditorUIAction *actions, int count) {
    ConsoleBackendState *s = (ConsoleBackendState *)state;
    WindowUIAction mapped[WINDOW_UI_MAX_ACTIONS];
    int i;
    if (!s || !s->window) return;
    if (!actions || count <= 0) {
        window_set_ui_actions(s->window, NULL, 0);
        return;
    }
    if (count > WINDOW_UI_MAX_ACTIONS) count = WINDOW_UI_MAX_ACTIONS;
    for (i = 0; i < count; i++) {
        mapped[i].x = actions[i].x;
        mapped[i].y = actions[i].y;
        mapped[i].w = actions[i].w;
        mapped[i].h = actions[i].h;
        strncpy(mapped[i].label, actions[i].label, sizeof(mapped[i].label) - 1);
        mapped[i].label[sizeof(mapped[i].label) - 1] = '\0';
        strncpy(mapped[i].command_id, actions[i].command_id, sizeof(mapped[i].command_id) - 1);
        mapped[i].command_id[sizeof(mapped[i].command_id) - 1] = '\0';
    }
    window_set_ui_actions(s->window, mapped, count);
}

static bool c_pop_ui_action(void *state, char *out_command, int out_len) {
    ConsoleBackendState *s = (ConsoleBackendState *)state;
    if (!s || !s->window) return false;
    return window_pop_ui_action(s->window, out_command, out_len);
}

static bool c_running(void *state) {
    ConsoleBackendState *s = (ConsoleBackendState *)state;
    return window_is_open(s->window);
}

static void c_shutdown(void *state) {
    ConsoleBackendState *s = (ConsoleBackendState *)state;
    if (s->window) {
        window_destroy(s->window);
        s->window = NULL;
    }
}

EditorBackendOps editor_backend_console_ops(void) {
    EditorBackendOps ops;
    ops.init = c_init;
    ops.poll = c_poll;
    ops.render = c_render;
    ops.set_overlay = c_set_overlay;
    ops.set_loading_screen = c_set_loading_screen;
    ops.set_ui_actions = c_set_ui_actions;
    ops.pop_ui_action = c_pop_ui_action;
    ops.is_running = c_running;
    ops.shutdown = c_shutdown;
    ops.name = "Console";
    ops.state_size = sizeof(ConsoleBackendState);
    return ops;
}
