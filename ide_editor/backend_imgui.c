#include "editor_backend.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int unused;
} ImGuiBackendState;

typedef struct {
    bool (*init)(void *state, const char *title, int width, int height);
    void (*poll)(void *state);
    void (*render)(void *state);
    bool (*is_running)(void *state);
    void (*shutdown)(void *state);
    const char *name;
    size_t state_size;
} EditorBackendOps;

static bool imgui_init(void *state, const char *title, int width, int height) {
    (void)state; (void)title; (void)width; (void)height;
    printf("[IDE] ImGui+SDL2 backend requested but dependencies are not linked in this build. Falling back to Console.\n");
    return false;
}

static void imgui_poll(void *state) { (void)state; }
static void imgui_render(void *state) { (void)state; }
static bool imgui_running(void *state) { (void)state; return false; }
static void imgui_shutdown(void *state) { (void)state; }

EditorBackendOps editor_backend_imgui_ops(void) {
    EditorBackendOps ops;
    ops.init = imgui_init;
    ops.poll = imgui_poll;
    ops.render = imgui_render;
    ops.is_running = imgui_running;
    ops.shutdown = imgui_shutdown;
    ops.name = "ImGui+SDL2";
    ops.state_size = sizeof(ImGuiBackendState);
    return ops;
}
