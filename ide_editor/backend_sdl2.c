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
static bool sdl2_running(void *state) { (void)state; return false; }
static void sdl2_shutdown(void *state) { (void)state; }

EditorBackendOps editor_backend_sdl2_ops(void) {
    EditorBackendOps ops;
    ops.init = sdl2_init;
    ops.poll = sdl2_poll;
    ops.render = sdl2_render;
    ops.is_running = sdl2_running;
    ops.shutdown = sdl2_shutdown;
    ops.name = "SDL2";
    ops.state_size = sizeof(SDL2BackendState);
    return ops;
}
