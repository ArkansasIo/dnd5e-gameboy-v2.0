#include "editor_backend.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int unused;
} SDL2BackendState;

typedef struct {
    bool (*init)(void *state, const char *title, int width, int height);
    void (*poll)(void *state);
    void (*render)(void *state);
    void (*set_overlay)(void *state, const char *text);
    void (*set_ui_actions)(void *state, const EditorUIAction *actions, int count);
    bool (*pop_ui_action)(void *state, char *out_command, int out_len);
    bool (*is_running)(void *state);
    void (*shutdown)(void *state);
    const char *name;
    size_t state_size;
} EditorBackendOps;

static bool sdl2_init(void *state, const char *title, int width, int height) {
    (void)state; (void)title; (void)width; (void)height;
    printf("[IDE] SDL2 backend requested but SDL2 is not linked in this build. Falling back to Console.\n");
    return false;
}

static void sdl2_poll(void *state) { (void)state; }
static void sdl2_render(void *state) { (void)state; }
static void sdl2_set_overlay(void *state, const char *text) { (void)state; (void)text; }
static void sdl2_set_ui_actions(void *state, const EditorUIAction *actions, int count) { (void)state; (void)actions; (void)count; }
static bool sdl2_pop_ui_action(void *state, char *out_command, int out_len) { (void)state; (void)out_command; (void)out_len; return false; }
static bool sdl2_running(void *state) { (void)state; return false; }
static void sdl2_shutdown(void *state) { (void)state; }

EditorBackendOps editor_backend_sdl2_ops(void) {
    EditorBackendOps ops;
    ops.init = sdl2_init;
    ops.poll = sdl2_poll;
    ops.render = sdl2_render;
    ops.set_overlay = sdl2_set_overlay;
    ops.set_ui_actions = sdl2_set_ui_actions;
    ops.pop_ui_action = sdl2_pop_ui_action;
    ops.is_running = sdl2_running;
    ops.shutdown = sdl2_shutdown;
    ops.name = "SDL2";
    ops.state_size = sizeof(SDL2BackendState);
    return ops;
}
