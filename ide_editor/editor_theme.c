#include "editor_theme.h"
#include <string.h>

static void fill_theme(EditorTheme *t, const char *name, unsigned int bg, unsigned int panel, unsigned int accent, unsigned int text, unsigned int warn, unsigned int ok, int font, int radius, int spacing) {
    strncpy(t->name, name, THEME_NAME_MAX - 1);
    t->name[THEME_NAME_MAX - 1] = '\0';
    t->colors.bg = bg;
    t->colors.panel = panel;
    t->colors.accent = accent;
    t->colors.text = text;
    t->colors.warning = warn;
    t->colors.success = ok;
    t->font_size = font;
    t->corner_radius = radius;
    t->spacing = spacing;
}

void editor_theme_make(EditorThemeId id, EditorTheme *out) {
    if (!out) return;
    switch (id) {
        case THEME_BLUEPRINT_NOIR:
            fill_theme(out, "Blueprint Noir", 0x101215, 0x181C22, 0x4CA3FF, 0xE8EEF7, 0xFF8F5A, 0x63D297, 15, 5, 8);
            break;
        case THEME_RETRO_AMBER:
            fill_theme(out, "Retro Amber", 0x1B150F, 0x2B241A, 0xF2B84B, 0xFCEFD2, 0xE67E22, 0x77C36A, 14, 3, 7);
            break;
        case THEME_HIGH_CONTRAST:
            fill_theme(out, "High Contrast", 0x000000, 0x141414, 0x00FFFF, 0xFFFFFF, 0xFF0044, 0x00FF66, 16, 2, 9);
            break;
        case THEME_CLASSIC_DARK:
        default:
            fill_theme(out, "Classic Dark", 0x1E1E1E, 0x2A2A2A, 0x569CD6, 0xD4D4D4, 0xD16969, 0x6A9955, 14, 4, 8);
            break;
    }
}

const char *editor_theme_name(EditorThemeId id) {
    switch (id) {
        case THEME_BLUEPRINT_NOIR: return "Blueprint Noir";
        case THEME_RETRO_AMBER: return "Retro Amber";
        case THEME_HIGH_CONTRAST: return "High Contrast";
        case THEME_CLASSIC_DARK:
        default: return "Classic Dark";
    }
}
