#include "ui_api.h"
#include <string.h>
#include <stdio.h>

static void cpy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

void ui_api_init(UIContext *ctx, EditorThemeId theme_id) {
    if (!ctx) return;
    ctx->panel_count = 0;
    ctx->widget_count = 0;
    ctx->menu_item_count = 0;
    ctx->notification_count = 0;
    ctx->dialog.open = false;
    ctx->dialog.id[0] = '\0';
    ctx->dialog.title[0] = '\0';
    ctx->dialog.message[0] = '\0';
    editor_theme_make(theme_id, &ctx->theme);
}

bool ui_api_add_panel(UIContext *ctx, const char *id, const char *title, UIDockSlot dock) {
    UIPanel *p;
    if (!ctx || !id || !title || ctx->panel_count >= UI_MAX_PANELS) return false;
    p = &ctx->panels[ctx->panel_count++];
    cpy(p->id, (int)sizeof(p->id), id);
    cpy(p->title, (int)sizeof(p->title), title);
    p->dock = dock;
    p->visible = true;
    return true;
}

bool ui_api_set_visible(UIContext *ctx, const char *id, bool visible) {
    UIPanel *p = ui_api_find_panel(ctx, id);
    if (!p) return false;
    p->visible = visible;
    return true;
}

UIPanel *ui_api_find_panel(UIContext *ctx, const char *id) {
    int i;
    if (!ctx || !id) return NULL;
    for (i = 0; i < ctx->panel_count; i++) {
        if (strcmp(ctx->panels[i].id, id) == 0) return &ctx->panels[i];
    }
    return NULL;
}

void ui_api_apply_theme(UIContext *ctx, EditorThemeId theme_id) {
    if (!ctx) return;
    editor_theme_make(theme_id, &ctx->theme);
}

bool ui_api_add_widget(UIContext *ctx, const char *id, const char *panel_id, const char *label, UIWidgetKind kind) {
    UIWidget *w;
    if (!ctx || !id || !panel_id || !label || ctx->widget_count >= UI_MAX_WIDGETS) return false;
    w = &ctx->widgets[ctx->widget_count++];
    cpy(w->id, (int)sizeof(w->id), id);
    cpy(w->panel_id, (int)sizeof(w->panel_id), panel_id);
    cpy(w->label, (int)sizeof(w->label), label);
    w->kind = kind;
    w->visible = true;
    w->enabled = true;
    w->int_value = 0;
    w->float_value = 0.0f;
    w->text_value[0] = '\0';
    return true;
}

UIWidget *ui_api_find_widget(UIContext *ctx, const char *id) {
    int i;
    if (!ctx || !id) return NULL;
    for (i = 0; i < ctx->widget_count; i++) {
        if (strcmp(ctx->widgets[i].id, id) == 0) return &ctx->widgets[i];
    }
    return NULL;
}

bool ui_api_set_widget_visible(UIContext *ctx, const char *id, bool visible) {
    UIWidget *w = ui_api_find_widget(ctx, id);
    if (!w) return false;
    w->visible = visible;
    return true;
}

bool ui_api_set_widget_enabled(UIContext *ctx, const char *id, bool enabled) {
    UIWidget *w = ui_api_find_widget(ctx, id);
    if (!w) return false;
    w->enabled = enabled;
    return true;
}

bool ui_api_set_widget_text(UIContext *ctx, const char *id, const char *text) {
    UIWidget *w = ui_api_find_widget(ctx, id);
    if (!w) return false;
    cpy(w->text_value, (int)sizeof(w->text_value), text);
    return true;
}

bool ui_api_add_menu_item(UIContext *ctx, const char *path, const char *command_id) {
    UIMenuItem *m;
    if (!ctx || !path || !command_id || ctx->menu_item_count >= UI_MAX_MENU_ITEMS) return false;
    m = &ctx->menu_items[ctx->menu_item_count++];
    cpy(m->path, (int)sizeof(m->path), path);
    cpy(m->command_id, (int)sizeof(m->command_id), command_id);
    m->enabled = true;
    m->checked = false;
    return true;
}

const UIMenuItem *ui_api_find_menu_item(const UIContext *ctx, const char *path) {
    int i;
    if (!ctx || !path) return NULL;
    for (i = 0; i < ctx->menu_item_count; i++) {
        if (strcmp(ctx->menu_items[i].path, path) == 0) return &ctx->menu_items[i];
    }
    return NULL;
}

const char *ui_api_find_menu_command(const UIContext *ctx, const char *path) {
    const UIMenuItem *m = ui_api_find_menu_item(ctx, path);
    if (!m) return NULL;
    return m->command_id;
}

bool ui_api_push_notification(UIContext *ctx, UINotificationSeverity severity, const char *text, int ttl_frames) {
    UINotification *n;
    int i;
    if (!ctx || !text || ttl_frames <= 0) return false;
    if (ctx->notification_count >= UI_MAX_NOTIFICATIONS) {
        for (i = 1; i < ctx->notification_count; i++) {
            ctx->notifications[i - 1] = ctx->notifications[i];
        }
        ctx->notification_count--;
    }
    n = &ctx->notifications[ctx->notification_count++];
    cpy(n->text, (int)sizeof(n->text), text);
    n->severity = severity;
    n->ttl_frames = ttl_frames;
    return true;
}

void ui_api_tick(UIContext *ctx) {
    int i;
    int write_idx = 0;
    if (!ctx) return;
    for (i = 0; i < ctx->notification_count; i++) {
        UINotification n = ctx->notifications[i];
        n.ttl_frames--;
        if (n.ttl_frames > 0) {
            ctx->notifications[write_idx++] = n;
        }
    }
    ctx->notification_count = write_idx;
}

void ui_api_open_dialog(UIContext *ctx, const char *id, const char *title, const char *message) {
    if (!ctx) return;
    ctx->dialog.open = true;
    cpy(ctx->dialog.id, (int)sizeof(ctx->dialog.id), id ? id : "dialog");
    cpy(ctx->dialog.title, (int)sizeof(ctx->dialog.title), title ? title : "Dialog");
    cpy(ctx->dialog.message, (int)sizeof(ctx->dialog.message), message ? message : "");
}

void ui_api_close_dialog(UIContext *ctx) {
    if (!ctx) return;
    ctx->dialog.open = false;
}

bool ui_api_save_layout(const UIContext *ctx, const char *filename) {
    int i;
    FILE *f;
    if (!ctx || !filename) return false;
    f = fopen(filename, "w");
    if (!f) return false;

    fprintf(f, "[panels]\n");
    for (i = 0; i < ctx->panel_count; i++) {
        const UIPanel *p = &ctx->panels[i];
        fprintf(f, "%s|%s|%d|%d\n", p->id, p->title, (int)p->dock, p->visible ? 1 : 0);
    }

    fprintf(f, "\n[widgets]\n");
    for (i = 0; i < ctx->widget_count; i++) {
        const UIWidget *w = &ctx->widgets[i];
        fprintf(f, "%s|%s|%s|%d|%d|%d|%.3f|%s\n",
            w->id, w->panel_id, w->label, (int)w->kind, w->visible ? 1 : 0, w->enabled ? 1 : 0, w->float_value, w->text_value);
    }

    fclose(f);
    return true;
}

bool ui_api_load_layout(UIContext *ctx, const char *filename) {
    enum { SEC_NONE, SEC_PANELS, SEC_WIDGETS } sec = SEC_NONE;
    char line[512];
    FILE *f;
    if (!ctx || !filename) return false;
    f = fopen(filename, "r");
    if (!f) return false;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n' || line[0] == '\r') continue;
        if (line[0] == '[') {
            if (strncmp(line, "[panels]", 8) == 0) sec = SEC_PANELS;
            else if (strncmp(line, "[widgets]", 9) == 0) sec = SEC_WIDGETS;
            else sec = SEC_NONE;
            continue;
        }

        if (sec == SEC_PANELS) {
            char id[48];
            char title[96];
            int dock;
            int visible;
            UIPanel *p;
            if (sscanf(line, "%47[^|]|%95[^|]|%d|%d", id, title, &dock, &visible) == 4) {
                p = ui_api_find_panel(ctx, id);
                if (!p) {
                    if (!ui_api_add_panel(ctx, id, title, (UIDockSlot)dock)) continue;
                    p = ui_api_find_panel(ctx, id);
                }
                if (p) {
                    p->dock = (UIDockSlot)dock;
                    p->visible = visible ? true : false;
                }
            }
        } else if (sec == SEC_WIDGETS) {
            char id[48];
            char panel_id[48];
            char label[64];
            int kind;
            int visible;
            int enabled;
            float fval;
            char text[96];
            UIWidget *w;
            if (sscanf(line, "%47[^|]|%47[^|]|%63[^|]|%d|%d|%d|%f|%95[^\n]",
                id, panel_id, label, &kind, &visible, &enabled, &fval, text) == 8) {
                w = ui_api_find_widget(ctx, id);
                if (!w) {
                    if (!ui_api_add_widget(ctx, id, panel_id, label, (UIWidgetKind)kind)) continue;
                    w = ui_api_find_widget(ctx, id);
                }
                if (w) {
                    w->kind = (UIWidgetKind)kind;
                    w->visible = visible ? true : false;
                    w->enabled = enabled ? true : false;
                    w->float_value = fval;
                    cpy(w->text_value, (int)sizeof(w->text_value), text);
                }
            }
        }
    }

    fclose(f);
    return true;
}


void ui_api_render_console(const UIContext *ctx) {
    int i;
    if (!ctx) return;
    printf("[UI] Panels=%d Widgets=%d MenuItems=%d Notifications=%d Theme=%s\n",
        ctx->panel_count, ctx->widget_count, ctx->menu_item_count, ctx->notification_count, ctx->theme.name);
    if (ctx->dialog.open) {
        printf("[UI][Dialog] %s: %s\n", ctx->dialog.title, ctx->dialog.message);
    }
    for (i = 0; i < ctx->notification_count; i++) {
        const UINotification *n = &ctx->notifications[i];
        const char *sev = (n->severity == UI_NOTIFY_ERROR) ? "ERR" : (n->severity == UI_NOTIFY_WARN ? "WRN" : "INF");
        printf("[UI][%s] %s (%d)\n", sev, n->text, n->ttl_frames);
    }
}

// Unreal 5-style layout: menu bar, toolbar, docked panels, tool shelf
void ui_api_render_unreal_layout(const UIContext *ctx) {
    int i;
    int top_count = 0;
    char tops[32][32];
    if (!ctx) return;
    printf("\n==== Unreal 5-Style IDE Layout ====\n");
    // Menu Bar
    printf("[Menu Bar]\n");
    for (i = 0; i < ctx->menu_item_count; i++) {
        const UIMenuItem *m = &ctx->menu_items[i];
        const char *slash = strchr(m->path, '/');
        char top[32];
        int j;
        int exists = 0;
        if (!slash) continue;
        {
            size_t n = (size_t)(slash - m->path);
            if (n >= sizeof(top)) n = sizeof(top) - 1;
            memcpy(top, m->path, n);
            top[n] = '\0';
        }
        for (j = 0; j < top_count; j++) {
            if (strcmp(tops[j], top) == 0) {
                exists = 1;
                break;
            }
        }
        if (!exists && top_count < 32) {
            strcpy(tops[top_count++], top);
            printf("  %s  ", top);
        }
    }
    printf("\n-----------------------------------\n");
    // Toolbar
    printf("[Toolbar]\n");
    for (i = 0; i < ctx->widget_count; i++) {
        const UIWidget *w = &ctx->widgets[i];
        if (w->kind == UI_WIDGET_BUTTON && strstr(w->id, "toolbar")) {
            printf("[%s] ", w->label);
        }
    }
    printf("\n-----------------------------------\n");
    // Tool Shelf
    printf("[Tool Shelf]\n");
    for (i = 0; i < ctx->widget_count; i++) {
        const UIWidget *w = &ctx->widgets[i];
        if (w->kind == UI_WIDGET_BUTTON && strstr(w->id, "tools")) {
            printf("[%s] ", w->label);
        }
    }
    printf("\n-----------------------------------\n");
    // Docked Panels
    printf("[Panels]\n");
    for (i = 0; i < ctx->panel_count; i++) {
        const UIPanel *p = &ctx->panels[i];
        printf("- %s (%s)\n", p->title,
            p->dock == UI_DOCK_LEFT ? "Left" :
            p->dock == UI_DOCK_RIGHT ? "Right" :
            p->dock == UI_DOCK_TOP ? "Top" :
            p->dock == UI_DOCK_BOTTOM ? "Bottom" : "Center");
    }
    printf("-----------------------------------\n");
    // Notifications
    for (i = 0; i < ctx->notification_count; i++) {
        const UINotification *n = &ctx->notifications[i];
        const char *sev = (n->severity == UI_NOTIFY_ERROR) ? "ERR" : (n->severity == UI_NOTIFY_WARN ? "WRN" : "INF");
        printf("[UI][%s] %s (%d)\n", sev, n->text, n->ttl_frames);
    }
    if (ctx->dialog.open) {
        printf("[Dialog] %s: %s\n", ctx->dialog.title, ctx->dialog.message);
    }
    printf("===================================\n\n");
}
