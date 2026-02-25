#include "ui_api.h"
#include <string.h>

static void cpy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

void ui_api_init(UIContext *ctx, EditorThemeId theme_id) {
    if (!ctx) return;
    ctx->panel_count = 0;
    editor_theme_make(theme_id, &ctx->theme);
}

bool ui_api_add_panel(UIContext *ctx, const char *id, const char *title, UIDockSlot dock) {
    UIPanel *p;
    if (!ctx || !id || !title || ctx->panel_count >= UI_MAX_PANELS) return false;
    p = &ctx->panels[ctx->panel_count++];
    cpy(p->id, sizeof(p->id), id);
    cpy(p->title, sizeof(p->title), title);
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
