#ifndef EDITOR_WIDGET_CODE_H
#define EDITOR_WIDGET_CODE_H

#include "editor_file.h"

// Code/text editor widget
typedef struct {
    EditorFileBuffer *file;
    int scroll_offset;
    int cursor_line, cursor_col;
    int is_active;
} EditorWidgetCode;

EditorWidgetCode *editor_widget_code_create(const char *filename);
void editor_widget_code_destroy(EditorWidgetCode *w);
void editor_widget_code_draw(const EditorWidgetCode *w);
void editor_widget_code_handle_input(EditorWidgetCode *w, int key);

#endif // EDITOR_WIDGET_CODE_H
