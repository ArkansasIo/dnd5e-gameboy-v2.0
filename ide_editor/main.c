#include "ide_editor.h"
#include "splash.h"
#include "editor_backend.h"
#include <stdio.h>
#include <string.h>
#include "math_bits.h"

static void print_usage(const char *exe_name) {
    printf("Usage: %s [--backend <console|sdl2|imgui>] [--no-splash]\n", exe_name);
}

int main(int argc, char **argv) {
    EditorBackendMode mode = EDITOR_BACKEND_CONSOLE;
    int show_splash = 1;
    int i;

    for (i = 1; i < argc; ++i) {
        if ((strcmp(argv[i], "--backend") == 0 || strcmp(argv[i], "-b") == 0) && (i + 1) < argc) {
            mode = editor_backend_mode_from_string(argv[++i]);
        } else if (strcmp(argv[i], "--no-splash") == 0) {
            show_splash = 0;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }

    if (show_splash) {
        show_splash_screen();
        print_math_limits();
    }
    IDEEditorApp *app = ide_editor_create_with_backend(mode);
    if (!app) {
        fprintf(stderr, "Failed to create IDE editor app for backend: %s\n", editor_backend_mode_name(mode));
        return 1;
    }
    ide_editor_run(app);
    ide_editor_destroy(app);
    return 0;
}
