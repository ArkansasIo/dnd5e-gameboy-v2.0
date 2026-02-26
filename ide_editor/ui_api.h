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

typedef enum {
    UI_WIDGET_LABEL = 0,
    UI_WIDGET_BUTTON = 1,
    UI_WIDGET_TOGGLE = 2,
    UI_WIDGET_SLIDER = 3,
    UI_WIDGET_INPUT = 4
} UIWidgetKind;

typedef enum {
    UI_NOTIFY_INFO = 0,
    UI_NOTIFY_WARN = 1,
    UI_NOTIFY_ERROR = 2
} UINotificationSeverity;

typedef struct {
    char id[48];
    char title[96];
    UIDockSlot dock;
    bool visible;
    int order; // For Unreal-style ordering
} UIPanel;

typedef struct {
    char id[48];
    char panel_id[48];
    char label[64];
    UIWidgetKind kind;
    bool visible;
    bool enabled;
    int int_value;
    float float_value;
    char text_value[96];
    int order; // For Unreal-style ordering
    int icon_id; // For toolbar/tool shelf icons
} UIWidget;

typedef struct {
    char path[96];
    char command_id[64];
    bool enabled;
    bool checked;
} UIMenuItem;

typedef struct {
    char text[128];
    UINotificationSeverity severity;
    int ttl_frames;
} UINotification;

typedef struct {
    bool open;
    char id[48];
    char title[96];
    char message[192];
} UIDialog;

#define UI_MAX_PANELS 48
#define UI_MAX_WIDGETS 256
#define UI_MAX_MENU_ITEMS 256
#define UI_MAX_NOTIFICATIONS 16

typedef struct {
    UIPanel panels[UI_MAX_PANELS];
    int panel_count;
    UIWidget widgets[UI_MAX_WIDGETS];
    int widget_count;
    UIMenuItem menu_items[UI_MAX_MENU_ITEMS];
    int menu_item_count;
    UINotification notifications[UI_MAX_NOTIFICATIONS];
    int notification_count;
    UIDialog dialog;
    EditorTheme theme;
} UIContext;

void ui_api_init(UIContext *ctx, EditorThemeId theme_id);
bool ui_api_add_panel(UIContext *ctx, const char *id, const char *title, UIDockSlot dock);
bool ui_api_set_visible(UIContext *ctx, const char *id, bool visible);
UIPanel *ui_api_find_panel(UIContext *ctx, const char *id);
void ui_api_apply_theme(UIContext *ctx, EditorThemeId theme_id);
bool ui_api_add_widget(UIContext *ctx, const char *id, const char *panel_id, const char *label, UIWidgetKind kind);
UIWidget *ui_api_find_widget(UIContext *ctx, const char *id);
bool ui_api_set_widget_visible(UIContext *ctx, const char *id, bool visible);
bool ui_api_set_widget_enabled(UIContext *ctx, const char *id, bool enabled);
bool ui_api_set_widget_text(UIContext *ctx, const char *id, const char *text);
bool ui_api_add_menu_item(UIContext *ctx, const char *path, const char *command_id);
const UIMenuItem *ui_api_find_menu_item(const UIContext *ctx, const char *path);
const char *ui_api_find_menu_command(const UIContext *ctx, const char *path);
bool ui_api_push_notification(UIContext *ctx, UINotificationSeverity severity, const char *text, int ttl_frames);
void ui_api_tick(UIContext *ctx);
void ui_api_open_dialog(UIContext *ctx, const char *id, const char *title, const char *message);
void ui_api_close_dialog(UIContext *ctx);
bool ui_api_save_layout(const UIContext *ctx, const char *filename);
bool ui_api_load_layout(UIContext *ctx, const char *filename);
void ui_api_render_console(const UIContext *ctx);
void ui_api_render_unreal_layout(const UIContext *ctx);

#endif
