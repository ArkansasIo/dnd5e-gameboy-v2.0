#include "editor_backend.h"
#include "window.h"
#include <stdlib.h>

typedef struct {
    EditorWindow *window;
} ConsoleBackendState;

typedef struct {
    bool (*init)(void *state, const char *title, int width, int height);
    void (*poll)(void *state);
    void (*render)(void *state);
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
    ops.is_running = c_running;
    ops.shutdown = c_shutdown;
    ops.name = "Console";
    ops.state_size = sizeof(ConsoleBackendState);
    return ops;
}
