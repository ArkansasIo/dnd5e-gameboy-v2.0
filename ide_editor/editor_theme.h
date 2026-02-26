#ifndef EDITOR_THEME_H
#define EDITOR_THEME_H

#define THEME_NAME_MAX 64

typedef struct {
    unsigned int bg;
    unsigned int panel;
    unsigned int accent;
    unsigned int text;
    unsigned int warning;
    unsigned int success;
} EditorThemeColors;

typedef struct {
    char name[THEME_NAME_MAX];
    EditorThemeColors colors;
    int font_size;
    int corner_radius;
    int spacing;
} EditorTheme;

typedef enum {
    THEME_CLASSIC_DARK,
    THEME_BLUEPRINT_NOIR,
    THEME_RETRO_AMBER,
    THEME_HIGH_CONTRAST,
    THEME_VSCODE_DARK_PLUS,
    THEME_VSCODE_LIGHT_PLUS,
    THEME_VSCODE_MONOKAI,
    THEME_VSCODE_SOLARIZED_DARK,
    THEME_VSCODE_SOLARIZED_LIGHT,
    THEME_VSCODE_ABYSS,
    THEME_VSCODE_KIMBIE_DARK,
    THEME_VSCODE_RED,
    THEME_VSCODE_QUIET_LIGHT,
    THEME_COUNT
} EditorThemeId;

void editor_theme_make(EditorThemeId id, EditorTheme *out);
const char *editor_theme_name(EditorThemeId id);

#endif
