#include "editor_settings.h"
#include <stdio.h>
#include <string.h>

void editor_settings_init(EditorSettings *s) {
    int i;
    if (!s) return;
    s->viewport.grid_size = 8;
    s->viewport.snap_to_grid = true;
    s->viewport.show_collision = true;
    s->viewport.show_nav_overlay = false;
    s->viewport.enable_realtime_preview = true;

    s->project.autosave_seconds = 30;
    s->project.autosave_enabled = true;
    s->project.backup_on_save = true;
    s->project.confirm_on_close = true;

    s->user.enable_sound_preview = true;
    s->user.compact_mode = false;
    s->user.high_contrast_ui = false;
    s->user.recent_count = 0;
    for (i = 0; i < SETTINGS_MAX_RECENT_PROJECTS; i++) {
        s->user.recent_projects[i][0] = '\0';
    }
}

static void trim_newline(char *line) {
    int n;
    if (!line) return;
    n = (int)strlen(line);
    while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == '\r')) {
        line[n - 1] = '\0';
        n--;
    }
}

bool editor_settings_save(const EditorSettings *s, const char *filename) {
    FILE *f;
    int i;
    if (!s || !filename) return false;
    f = fopen(filename, "w");
    if (!f) return false;

    fprintf(f, "grid_size=%d\n", s->viewport.grid_size);
    fprintf(f, "snap_to_grid=%d\n", s->viewport.snap_to_grid ? 1 : 0);
    fprintf(f, "show_collision=%d\n", s->viewport.show_collision ? 1 : 0);
    fprintf(f, "show_nav_overlay=%d\n", s->viewport.show_nav_overlay ? 1 : 0);
    fprintf(f, "enable_realtime_preview=%d\n", s->viewport.enable_realtime_preview ? 1 : 0);

    fprintf(f, "autosave_seconds=%d\n", s->project.autosave_seconds);
    fprintf(f, "autosave_enabled=%d\n", s->project.autosave_enabled ? 1 : 0);
    fprintf(f, "backup_on_save=%d\n", s->project.backup_on_save ? 1 : 0);
    fprintf(f, "confirm_on_close=%d\n", s->project.confirm_on_close ? 1 : 0);

    fprintf(f, "enable_sound_preview=%d\n", s->user.enable_sound_preview ? 1 : 0);
    fprintf(f, "compact_mode=%d\n", s->user.compact_mode ? 1 : 0);
    fprintf(f, "high_contrast_ui=%d\n", s->user.high_contrast_ui ? 1 : 0);
    fprintf(f, "recent_count=%d\n", s->user.recent_count);

    for (i = 0; i < s->user.recent_count && i < SETTINGS_MAX_RECENT_PROJECTS; i++) {
        fprintf(f, "recent_%d=%s\n", i, s->user.recent_projects[i]);
    }

    fclose(f);
    return true;
}

bool editor_settings_load(EditorSettings *s, const char *filename) {
    FILE *f;
    char line[512];
    if (!s || !filename) return false;
    f = fopen(filename, "r");
    if (!f) return false;

    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        sscanf(line, "grid_size=%d", &s->viewport.grid_size);
        sscanf(line, "snap_to_grid=%d", (int *)&s->viewport.snap_to_grid);
        sscanf(line, "show_collision=%d", (int *)&s->viewport.show_collision);
        sscanf(line, "show_nav_overlay=%d", (int *)&s->viewport.show_nav_overlay);
        sscanf(line, "enable_realtime_preview=%d", (int *)&s->viewport.enable_realtime_preview);

        sscanf(line, "autosave_seconds=%d", &s->project.autosave_seconds);
        sscanf(line, "autosave_enabled=%d", (int *)&s->project.autosave_enabled);
        sscanf(line, "backup_on_save=%d", (int *)&s->project.backup_on_save);
        sscanf(line, "confirm_on_close=%d", (int *)&s->project.confirm_on_close);

        sscanf(line, "enable_sound_preview=%d", (int *)&s->user.enable_sound_preview);
        sscanf(line, "compact_mode=%d", (int *)&s->user.compact_mode);
        sscanf(line, "high_contrast_ui=%d", (int *)&s->user.high_contrast_ui);
        sscanf(line, "recent_count=%d", &s->user.recent_count);

        if (strncmp(line, "recent_", 7) == 0) {
            int idx;
            char path[260];
            if (sscanf(line, "recent_%d=%259[^\n]", &idx, path) == 2) {
                if (idx >= 0 && idx < SETTINGS_MAX_RECENT_PROJECTS) {
                    strncpy(s->user.recent_projects[idx], path, 259);
                    s->user.recent_projects[idx][259] = '\0';
                }
            }
        }
    }

    if (s->user.recent_count < 0) s->user.recent_count = 0;
    if (s->user.recent_count > SETTINGS_MAX_RECENT_PROJECTS) s->user.recent_count = SETTINGS_MAX_RECENT_PROJECTS;

    fclose(f);
    return true;
}

void editor_settings_add_recent(EditorSettings *s, const char *project_path) {
    int i;
    if (!s || !project_path || project_path[0] == '\0') return;

    for (i = 0; i < s->user.recent_count; i++) {
        if (strcmp(s->user.recent_projects[i], project_path) == 0) return;
    }

    if (s->user.recent_count < SETTINGS_MAX_RECENT_PROJECTS) {
        strncpy(s->user.recent_projects[s->user.recent_count], project_path, 259);
        s->user.recent_projects[s->user.recent_count][259] = '\0';
        s->user.recent_count++;
    } else {
        for (i = 1; i < SETTINGS_MAX_RECENT_PROJECTS; i++) {
            strncpy(s->user.recent_projects[i - 1], s->user.recent_projects[i], 260);
        }
        strncpy(s->user.recent_projects[SETTINGS_MAX_RECENT_PROJECTS - 1], project_path, 259);
        s->user.recent_projects[SETTINGS_MAX_RECENT_PROJECTS - 1][259] = '\0';
    }
}
