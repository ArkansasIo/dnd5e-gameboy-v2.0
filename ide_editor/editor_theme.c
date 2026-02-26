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
        case THEME_VSCODE_DARK_PLUS:
            fill_theme(out, "VS Code Dark+", 0x1E1E1E, 0x252526, 0x569CD6, 0xD4D4D4, 0xF48771, 0x89D185, 14, 4, 8);
            break;
        case THEME_VSCODE_LIGHT_PLUS:
            fill_theme(out, "VS Code Light+", 0xFFFFFF, 0xF3F3F3, 0x006AB1, 0x333333, 0xC72E0F, 0x2E8B57, 14, 4, 8);
            break;
        case THEME_VSCODE_MONOKAI:
            fill_theme(out, "VS Code Monokai", 0x272822, 0x2D2E27, 0xFD971F, 0xF8F8F2, 0xF92672, 0xA6E22E, 14, 4, 8);
            break;
        case THEME_VSCODE_SOLARIZED_DARK:
            fill_theme(out, "VS Code Solarized Dark", 0x002B36, 0x073642, 0x268BD2, 0x93A1A1, 0xDC322F, 0x859900, 14, 4, 8);
            break;
        case THEME_VSCODE_SOLARIZED_LIGHT:
            fill_theme(out, "VS Code Solarized Light", 0xFDF6E3, 0xEEE8D5, 0x268BD2, 0x657B83, 0xD33682, 0x859900, 14, 4, 8);
            break;
        case THEME_VSCODE_ABYSS:
            fill_theme(out, "VS Code Abyss", 0x000C18, 0x00162A, 0x007ACC, 0xE6F3FF, 0xFF6B6B, 0x7EDB89, 14, 4, 8);
            break;
        case THEME_VSCODE_KIMBIE_DARK:
            fill_theme(out, "VS Code Kimbie Dark", 0x221A0F, 0x2A2117, 0xF79A32, 0xD3AF86, 0xDC3958, 0x889B4A, 14, 4, 8);
            break;
        case THEME_VSCODE_RED:
            fill_theme(out, "VS Code Red", 0x1E1517, 0x2A1B1F, 0xF14C4C, 0xF2E7E9, 0xFF7B72, 0x73C991, 14, 4, 8);
            break;
        case THEME_VSCODE_QUIET_LIGHT:
            fill_theme(out, "VS Code Quiet Light", 0xF5F5F5, 0xECECEC, 0x0451A5, 0x333333, 0xA31515, 0x2B8A3E, 14, 4, 8);
            break;
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
        case THEME_VSCODE_DARK_PLUS: return "VS Code Dark+";
        case THEME_VSCODE_LIGHT_PLUS: return "VS Code Light+";
        case THEME_VSCODE_MONOKAI: return "VS Code Monokai";
        case THEME_VSCODE_SOLARIZED_DARK: return "VS Code Solarized Dark";
        case THEME_VSCODE_SOLARIZED_LIGHT: return "VS Code Solarized Light";
        case THEME_VSCODE_ABYSS: return "VS Code Abyss";
        case THEME_VSCODE_KIMBIE_DARK: return "VS Code Kimbie Dark";
        case THEME_VSCODE_RED: return "VS Code Red";
        case THEME_VSCODE_QUIET_LIGHT: return "VS Code Quiet Light";
        case THEME_BLUEPRINT_NOIR: return "Blueprint Noir";
        case THEME_RETRO_AMBER: return "Retro Amber";
        case THEME_HIGH_CONTRAST: return "High Contrast";
        case THEME_CLASSIC_DARK:
        default: return "Classic Dark";
    }
}
