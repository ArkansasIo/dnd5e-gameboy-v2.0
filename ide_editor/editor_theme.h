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
    THEME_COUNT
} EditorThemeId;

void editor_theme_make(EditorThemeId id, EditorTheme *out);
const char *editor_theme_name(EditorThemeId id);

#endif
