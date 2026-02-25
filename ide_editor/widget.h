#ifndef WIDGET_H
#define WIDGET_H

typedef struct EditorWidget EditorWidget;

EditorWidget *widget_create(const char *type);
void widget_destroy(EditorWidget *widget);
// Widget API

#endif // WIDGET_H
