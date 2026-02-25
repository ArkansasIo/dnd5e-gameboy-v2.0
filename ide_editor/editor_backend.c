#include "editor_backend.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    bool (*init)(void *state, const char *title, int width, int height);
    void (*poll)(void *state);
    void (*render)(void *state);
    bool (*is_running)(void *state);
    void (*shutdown)(void *state);
    const char *name;
    size_t state_size;
} EditorBackendOps;

struct EditorBackend {
    EditorBackendMode mode;
    EditorBackendOps ops;
    void *state;
};

/* Backend providers */
EditorBackendOps editor_backend_console_ops(void);
EditorBackendOps editor_backend_sdl2_ops(void);
EditorBackendOps editor_backend_imgui_ops(void);

static EditorBackendOps ops_for_mode(EditorBackendMode mode) {
    switch (mode) {
        case EDITOR_BACKEND_SDL2: return editor_backend_sdl2_ops();
        case EDITOR_BACKEND_IMGUI: return editor_backend_imgui_ops();
        case EDITOR_BACKEND_CONSOLE:
        default: return editor_backend_console_ops();
    }
}

EditorBackend *editor_backend_create(EditorBackendMode mode, const char *title, int width, int height) {
    EditorBackend *b = (EditorBackend *)malloc(sizeof(EditorBackend));
    if (!b) return NULL;

    b->mode = mode;
    b->ops = ops_for_mode(mode);
    b->state = calloc(1, b->ops.state_size);
    if (!b->state) {
        free(b);
        return NULL;
    }

    if (!b->ops.init(b->state, title, width, height)) {
        if (mode != EDITOR_BACKEND_CONSOLE) {
            free(b->state);
            b->mode = EDITOR_BACKEND_CONSOLE;
            b->ops = editor_backend_console_ops();
            b->state = calloc(1, b->ops.state_size);
            if (!b->state || !b->ops.init(b->state, title, width, height)) {
                if (b->state) free(b->state);
                free(b);
                return NULL;
            }
        } else {
            free(b->state);
            free(b);
            return NULL;
        }
    }
    return b;
}

void editor_backend_destroy(EditorBackend *backend) {
    if (!backend) return;
    if (backend->ops.shutdown) backend->ops.shutdown(backend->state);
    free(backend->state);
    free(backend);
}

bool editor_backend_is_running(EditorBackend *backend) {
    if (!backend || !backend->ops.is_running) return false;
    return backend->ops.is_running(backend->state);
}

void editor_backend_poll(EditorBackend *backend) {
    if (!backend || !backend->ops.poll) return;
    backend->ops.poll(backend->state);
}

void editor_backend_render(EditorBackend *backend) {
    if (!backend || !backend->ops.render) return;
    backend->ops.render(backend->state);
}

void editor_backend_shutdown(EditorBackend *backend) {
    if (!backend || !backend->ops.shutdown) return;
    backend->ops.shutdown(backend->state);
}

const char *editor_backend_name(EditorBackend *backend) {
    if (!backend) return "Unknown";
    return backend->ops.name;
}

EditorBackendMode editor_backend_mode_from_string(const char *s) {
    if (!s) return EDITOR_BACKEND_CONSOLE;
    if (_stricmp(s, "sdl2") == 0) return EDITOR_BACKEND_SDL2;
    if (_stricmp(s, "imgui") == 0 || _stricmp(s, "imgui-sdl2") == 0) return EDITOR_BACKEND_IMGUI;
    return EDITOR_BACKEND_CONSOLE;
}

const char *editor_backend_mode_name(EditorBackendMode mode) {
    switch (mode) {
        case EDITOR_BACKEND_SDL2: return "SDL2";
        case EDITOR_BACKEND_IMGUI: return "ImGui+SDL2";
        case EDITOR_BACKEND_CONSOLE:
        default: return "Console";
    }
}
