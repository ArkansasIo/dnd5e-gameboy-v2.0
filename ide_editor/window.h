#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

typedef struct EditorWindow EditorWindow;

EditorWindow *window_create(const char *title, int width, int height);
void window_show(EditorWindow *win);
bool window_is_open(EditorWindow *win);
void window_poll_events(EditorWindow *win);
void window_render(EditorWindow *win);
void window_destroy(EditorWindow *win);

#endif // WINDOW_H
