#ifndef EDITOR_BACKEND_H
#define EDITOR_BACKEND_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    EDITOR_BACKEND_CONSOLE = 0,
    EDITOR_BACKEND_SDL2 = 1,
    EDITOR_BACKEND_IMGUI = 2
} EditorBackendMode;

typedef struct EditorBackend EditorBackend;

EditorBackend *editor_backend_create(EditorBackendMode mode, const char *title, int width, int height);
void editor_backend_destroy(EditorBackend *backend);

bool editor_backend_is_running(EditorBackend *backend);
void editor_backend_poll(EditorBackend *backend);
void editor_backend_render(EditorBackend *backend);
void editor_backend_shutdown(EditorBackend *backend);

const char *editor_backend_name(EditorBackend *backend);
EditorBackendMode editor_backend_mode_from_string(const char *s);
const char *editor_backend_mode_name(EditorBackendMode mode);

#ifdef __cplusplus
}
#endif

#endif
