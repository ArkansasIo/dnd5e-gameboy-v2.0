#include "widget.h"
#include <stdlib.h>
#include <string.h>

struct EditorWidget {
    char type[32];
};

EditorWidget *widget_create(const char *type) {
    EditorWidget *widget = (EditorWidget *)malloc(sizeof(EditorWidget));
    strncpy(widget->type, type, 31);
    widget->type[31] = '\0';
    return widget;
}

void widget_destroy(EditorWidget *widget) {
    free(widget);
}
