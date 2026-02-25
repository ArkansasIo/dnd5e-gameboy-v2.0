#include "editor_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EditorFileBuffer *editor_file_load(const char *filename) {
    EditorFileBuffer *buf = (EditorFileBuffer *)malloc(sizeof(EditorFileBuffer));
    buf->line_count = 0;
    buf->cursor_line = 0;
    buf->cursor_col = 0;
    buf->modified = false;
    FILE *f = fopen(filename, "r");
    if (f) {
        char line[EDITOR_FILE_MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), f) && buf->line_count < EDITOR_FILE_MAX_LINES) {
            strncpy(buf->lines[buf->line_count], line, EDITOR_FILE_MAX_LINE_LENGTH-1);
            buf->lines[buf->line_count][EDITOR_FILE_MAX_LINE_LENGTH-1] = '\0';
            buf->line_count++;
        }
        fclose(f);
    }
    return buf;
}

void editor_file_save(EditorFileBuffer *buf, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (f) {
        for (int i = 0; i < buf->line_count; ++i) {
            fputs(buf->lines[i], f);
        }
        fclose(f);
        buf->modified = false;
    }
}

void editor_file_free(EditorFileBuffer *buf) {
    free(buf);
}

void editor_file_insert(EditorFileBuffer *buf, int line, const char *text) {
    if (line < 0 || line > buf->line_count || buf->line_count >= EDITOR_FILE_MAX_LINES) return;
    for (int i = buf->line_count; i > line; --i) {
        strncpy(buf->lines[i], buf->lines[i-1], EDITOR_FILE_MAX_LINE_LENGTH);
    }
    strncpy(buf->lines[line], text, EDITOR_FILE_MAX_LINE_LENGTH-1);
    buf->lines[line][EDITOR_FILE_MAX_LINE_LENGTH-1] = '\0';
    buf->line_count++;
    buf->modified = true;
}

void editor_file_delete(EditorFileBuffer *buf, int line) {
    if (line < 0 || line >= buf->line_count) return;
    for (int i = line; i < buf->line_count-1; ++i) {
        strncpy(buf->lines[i], buf->lines[i+1], EDITOR_FILE_MAX_LINE_LENGTH);
    }
    buf->line_count--;
    buf->modified = true;
}
