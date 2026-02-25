
#ifndef EDITOR_FILE_H
#define EDITOR_FILE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

// File buffer for editing
#define EDITOR_FILE_MAX_LINES 2048
#define EDITOR_FILE_MAX_LINE_LENGTH 256

typedef struct {
    char lines[EDITOR_FILE_MAX_LINES][EDITOR_FILE_MAX_LINE_LENGTH];
    int line_count;
    int cursor_line;
    int cursor_col;
    bool modified;
} EditorFileBuffer;

EditorFileBuffer *editor_file_load(const char *filename);
void editor_file_save(EditorFileBuffer *buf, const char *filename);
void editor_file_free(EditorFileBuffer *buf);
void editor_file_insert(EditorFileBuffer *buf, int line, const char *text);
void editor_file_delete(EditorFileBuffer *buf, int line);

#endif // EDITOR_FILE_H
