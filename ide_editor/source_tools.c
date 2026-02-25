#include "source_tools.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void s_copy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

static const char *base_name(const char *path) {
    const char *p = strrchr(path, '\\');
    const char *q = strrchr(path, '/');
    const char *m = p > q ? p : q;
    return m ? (m + 1) : path;
}

static const char *file_ext(const char *name) {
    const char *dot = strrchr(name, '.');
    return dot ? dot : "";
}

static bool is_supported_source(const char *ext) {
    if (!ext) return false;
    return (_stricmp(ext, ".c") == 0 || _stricmp(ext, ".h") == 0 ||
            _stricmp(ext, ".s") == 0 || _stricmp(ext, ".asm") == 0);
}

static int command_exists(const char *cmd) {
    char probe[128];
    snprintf(probe, sizeof(probe), "where %s >nul 2>nul", cmd);
    return system(probe) == 0;
}

static void add_file(SourceWorkspace *ws, const char *fullpath) {
    SourceFileEntry *e;
    const char *bn;
    if (!ws || ws->file_count >= SOURCE_MAX_FILES) return;
    bn = base_name(fullpath);
    e = &ws->files[ws->file_count++];
    s_copy(e->path, (int)sizeof(e->path), fullpath);
    s_copy(e->name, (int)sizeof(e->name), bn);
    s_copy(e->ext, (int)sizeof(e->ext), file_ext(bn));
}

static void scan_dir(SourceWorkspace *ws, const char *dir) {
    WIN32_FIND_DATAA fd;
    HANDLE h;
    char pattern[320];
    char full[320];
    if (!ws || !dir) return;

    snprintf(pattern, sizeof(pattern), "%s\\*", dir);
    h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return;

    do {
        if (strcmp(fd.cFileName, ".") == 0 || strcmp(fd.cFileName, "..") == 0) continue;
        snprintf(full, sizeof(full), "%s\\%s", dir, fd.cFileName);
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            scan_dir(ws, full);
        } else {
            const char *ext = file_ext(fd.cFileName);
            if (is_supported_source(ext)) add_file(ws, full);
        }
    } while (FindNextFileA(h, &fd));

    FindClose(h);
}

void source_workspace_init(SourceWorkspace *ws, const char *root) {
    if (!ws) return;
    memset(ws, 0, sizeof(*ws));
    s_copy(ws->root, (int)sizeof(ws->root), root ? root : "..\\src");
    ws->active_index = -1;
    s_copy(ws->last_status, (int)sizeof(ws->last_status), "source workspace initialized");
}

void source_workspace_free(SourceWorkspace *ws) {
    if (!ws) return;
    if (ws->active_buffer) {
        editor_file_free(ws->active_buffer);
        ws->active_buffer = NULL;
    }
}

bool source_workspace_scan(SourceWorkspace *ws) {
    if (!ws) return false;
    ws->file_count = 0;
    scan_dir(ws, ws->root);
    if (ws->file_count > 0) {
        ws->active_index = 0;
        s_copy(ws->active_path, (int)sizeof(ws->active_path), ws->files[0].path);
        snprintf(ws->last_status, sizeof(ws->last_status), "scan complete: %d files", ws->file_count);
        return true;
    }
    ws->active_index = -1;
    s_copy(ws->active_path, (int)sizeof(ws->active_path), "");
    s_copy(ws->last_status, (int)sizeof(ws->last_status), "scan complete: no source files");
    return false;
}

bool source_workspace_next(SourceWorkspace *ws) {
    if (!ws || ws->file_count <= 0) return false;
    ws->active_index = (ws->active_index + 1) % ws->file_count;
    s_copy(ws->active_path, (int)sizeof(ws->active_path), ws->files[ws->active_index].path);
    return true;
}

bool source_workspace_prev(SourceWorkspace *ws) {
    if (!ws || ws->file_count <= 0) return false;
    ws->active_index--;
    if (ws->active_index < 0) ws->active_index = ws->file_count - 1;
    s_copy(ws->active_path, (int)sizeof(ws->active_path), ws->files[ws->active_index].path);
    return true;
}

const SourceFileEntry *source_workspace_active(const SourceWorkspace *ws) {
    if (!ws || ws->active_index < 0 || ws->active_index >= ws->file_count) return NULL;
    return &ws->files[ws->active_index];
}

bool source_workspace_open_active(SourceWorkspace *ws) {
    const SourceFileEntry *e = source_workspace_active(ws);
    if (!ws || !e) return false;
    if (ws->active_buffer) {
        editor_file_free(ws->active_buffer);
        ws->active_buffer = NULL;
    }
    ws->active_buffer = editor_file_load(e->path);
    s_copy(ws->active_path, (int)sizeof(ws->active_path), e->path);
    snprintf(ws->last_status, sizeof(ws->last_status), "opened: %s", e->name);
    return ws->active_buffer ? true : false;
}

bool source_workspace_save_active(SourceWorkspace *ws) {
    if (!ws || !ws->active_buffer || !ws->active_path[0]) return false;
    editor_file_save(ws->active_buffer, ws->active_path);
    snprintf(ws->last_status, sizeof(ws->last_status), "saved: %s", base_name(ws->active_path));
    return true;
}

bool source_workspace_compile_active(SourceWorkspace *ws, const char *mode) {
    const SourceFileEntry *e = source_workspace_active(ws);
    char cmd[700];
    char obj_dir[320];
    char obj_file[420];
    int rc;
    const int want_asm = mode && _stricmp(mode, "asm") == 0;
    const int want_c = mode && _stricmp(mode, "c") == 0;

    if (!ws || !e) return false;
    CreateDirectoryA("build_tmp", NULL);
    CreateDirectoryA("build_tmp\\obj", NULL);
    snprintf(obj_dir, sizeof(obj_dir), "build_tmp\\obj");
    snprintf(obj_file, sizeof(obj_file), "%s\\%s.o", obj_dir, e->name);

    if (want_asm || _stricmp(e->ext, ".s") == 0 || _stricmp(e->ext, ".asm") == 0) {
        if (command_exists("rgbasm")) {
            snprintf(cmd, sizeof(cmd), "rgbasm -o \"%s\" \"%s\"", obj_file, e->path);
        } else if (command_exists("x86_64-w64-mingw32-gcc")) {
            snprintf(cmd, sizeof(cmd), "x86_64-w64-mingw32-gcc -x assembler -c \"%s\" -o \"%s\"", e->path, obj_file);
        } else if (command_exists("gcc")) {
            snprintf(cmd, sizeof(cmd), "gcc -x assembler -c \"%s\" -o \"%s\"", e->path, obj_file);
        } else {
            s_copy(ws->last_status, (int)sizeof(ws->last_status), "asm compile failed: no assembler tool");
            return false;
        }
    } else if (want_c || _stricmp(e->ext, ".c") == 0) {
        if (command_exists("x86_64-w64-mingw32-gcc")) {
            snprintf(cmd, sizeof(cmd), "x86_64-w64-mingw32-gcc -std=c99 -c \"%s\" -o \"%s\"", e->path, obj_file);
        } else if (command_exists("gcc")) {
            snprintf(cmd, sizeof(cmd), "gcc -std=c99 -c \"%s\" -o \"%s\"", e->path, obj_file);
        } else {
            s_copy(ws->last_status, (int)sizeof(ws->last_status), "c compile failed: gcc not found");
            return false;
        }
    } else {
        snprintf(ws->last_status, sizeof(ws->last_status), "compile skipped: unsupported ext %s", e->ext);
        return false;
    }

    rc = system(cmd);
    if (rc == 0) {
        snprintf(ws->last_status, sizeof(ws->last_status), "compiled: %s", e->name);
        return true;
    }
    snprintf(ws->last_status, sizeof(ws->last_status), "compile failed (%d): %s", rc, e->name);
    return false;
}
