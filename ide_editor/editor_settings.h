#ifndef EDITOR_SETTINGS_H
#define EDITOR_SETTINGS_H

#include <stdbool.h>

#define SETTINGS_MAX_RECENT_PROJECTS 10

typedef struct {
    int grid_size;
    bool snap_to_grid;
    bool show_collision;
    bool show_nav_overlay;
    bool enable_realtime_preview;
} EditorViewportSettings;

typedef struct {
    int autosave_seconds;
    bool autosave_enabled;
    bool backup_on_save;
    bool confirm_on_close;
} EditorProjectSettings;

typedef struct {
    bool enable_sound_preview;
    bool compact_mode;
    bool high_contrast_ui;
    char recent_projects[SETTINGS_MAX_RECENT_PROJECTS][260];
    int recent_count;
} EditorUserSettings;

typedef struct {
    EditorViewportSettings viewport;
    EditorProjectSettings project;
    EditorUserSettings user;
} EditorSettings;

void editor_settings_init(EditorSettings *s);
bool editor_settings_save(const EditorSettings *s, const char *filename);
bool editor_settings_load(EditorSettings *s, const char *filename);
void editor_settings_add_recent(EditorSettings *s, const char *project_path);

#endif
