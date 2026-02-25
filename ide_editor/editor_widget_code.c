#include "editor_widget_code.h"
#include <stdlib.h>
#include <stdio.h>

EditorWidgetCode *editor_widget_code_create(const char *filename) {
    EditorWidgetCode *w = (EditorWidgetCode *)malloc(sizeof(EditorWidgetCode));
    w->file = editor_file_load(filename);
    w->scroll_offset = 0;
    w->cursor_line = 0;
    w->cursor_col = 0;
    w->is_active = 1;
    return w;
}

void editor_widget_code_destroy(EditorWidgetCode *w) {
    if (w) {
        editor_file_free(w->file);
        free(w);
    }
}

void editor_widget_code_draw(const EditorWidgetCode *w) {
    if (!w || !w->file) return;
    printf("--- Code Editor ---\n");
    for (int i = 0; i < w->file->line_count; ++i) {
        printf("%4d: %s", i+1, w->file->lines[i]);
    }
}

void editor_widget_code_handle_input(EditorWidgetCode *w, int key) {
    (void)w;
    (void)key;
    // Placeholder for input handling
}
