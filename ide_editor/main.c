#include "ide_editor.h"
#include "splash.h"
#include "editor_backend.h"
#include <stdio.h>
#include <string.h>
#include "math_bits.h"
#include <windows.h>

static LONG WINAPI ide_crash_handler(struct _EXCEPTION_POINTERS *ep) {
    FILE *f = fopen("ide_editor_crash.log", "a");
    if (f) {
        SYSTEMTIME st;
        GetLocalTime(&st);
        fprintf(
            f,
            "[%04u-%02u-%02u %02u:%02u:%02u] Unhandled exception: code=0x%08lX address=%p\n",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,
            ep && ep->ExceptionRecord ? ep->ExceptionRecord->ExceptionCode : 0UL,
            ep && ep->ExceptionRecord ? ep->ExceptionRecord->ExceptionAddress : NULL
        );
        fclose(f);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}

static void print_usage(const char *exe_name) {
    printf("Usage: %s [--backend <console|sdl2|imgui>] [--no-splash]\n", exe_name);
}

int main(int argc, char **argv) {
    EditorBackendMode mode = EDITOR_BACKEND_CONSOLE;
    int show_splash = 1;
    int i;

    SetUnhandledExceptionFilter(ide_crash_handler);

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
