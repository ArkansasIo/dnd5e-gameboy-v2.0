#ifndef UI_API_H
#define UI_API_H

#include <stdbool.h>
#include "editor_theme.h"

typedef enum {
    UI_DOCK_LEFT,
    UI_DOCK_RIGHT,
    UI_DOCK_TOP,
    UI_DOCK_BOTTOM,
    UI_DOCK_CENTER
} UIDockSlot;

typedef struct {
    char id[48];
    char title[96];
    UIDockSlot dock;
    bool visible;
} UIPanel;

#define UI_MAX_PANELS 48

typedef struct {
    UIPanel panels[UI_MAX_PANELS];
    int panel_count;
    EditorTheme theme;
} UIContext;

void ui_api_init(UIContext *ctx, EditorThemeId theme_id);
bool ui_api_add_panel(UIContext *ctx, const char *id, const char *title, UIDockSlot dock);
bool ui_api_set_visible(UIContext *ctx, const char *id, bool visible);
UIPanel *ui_api_find_panel(UIContext *ctx, const char *id);
void ui_api_apply_theme(UIContext *ctx, EditorThemeId theme_id);

#endif
