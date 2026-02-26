#include "music_editor_tracker.h"
#include <stdio.h>
#include <string.h>

static void safe_copy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

void music_editor_tracker_config_defaults(music_editor_tracker_config *cfg) {
    int i;
    if (!cfg) return;
    cfg->enabled = true;
    cfg->revision = 1;
    for (i = 0; i < 16; ++i) {
        cfg->option_int[i] = 0;
        cfg->option_float[i] = 0.0f;
        cfg->option_bool[i] = false;
    }
}

void music_editor_tracker_state_init(music_editor_tracker_state *state) {
    if (!state) return;
    state->frame_counter = 0;
    state->last_error_code = 0;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "MusicEditorTracker ready");
}

bool music_editor_tracker_validate(const music_editor_tracker_config *cfg, char *reason, int reason_len) {
    if (reason && reason_len > 0) {
        snprintf(reason, (size_t)reason_len, "MusicEditorTracker: validation ok");
    }
    if (!cfg) return false;
    return cfg->enabled;
}

bool music_editor_tracker_set_option_int(music_editor_tracker_config *cfg, int index, int value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_int[index] = value;
    return true;
}

bool music_editor_tracker_set_option_float(music_editor_tracker_config *cfg, int index, float value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_float[index] = value;
    return true;
}

bool music_editor_tracker_set_option_bool(music_editor_tracker_config *cfg, int index, bool value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_bool[index] = value;
    return true;
}

bool music_editor_tracker_handle_command(music_editor_tracker_config *cfg, music_editor_tracker_state *state, const char *command_id) {
    if (!cfg || !state || !command_id) return false;
    if (strcmp(command_id, "music_editor_tracker.enable") == 0) {
        cfg->enabled = true;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "MusicEditorTracker enabled");
        return true;
    }
    if (strcmp(command_id, "music_editor_tracker.disable") == 0) {
        cfg->enabled = false;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "MusicEditorTracker disabled");
        return true;
    }
    return false;
}

void music_editor_tracker_tick(music_editor_tracker_config *cfg, music_editor_tracker_state *state) {
    if (!cfg || !state || !cfg->enabled) return;
    state->frame_counter++;
    cfg->revision++;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "MusicEditorTracker tick");
}

bool music_editor_tracker_serialize_config(const music_editor_tracker_config *cfg, const char *path) {
    FILE *f;
    int i;
    if (!cfg || !path) return false;
    f = fopen(path, "w");
    if (!f) return false;
    fprintf(f, "enabled=%d\n", cfg->enabled ? 1 : 0);
    fprintf(f, "revision=%d\n", cfg->revision);
    for (i = 0; i < 16; ++i) fprintf(f, "int_%d=%d\n", i, cfg->option_int[i]);
    for (i = 0; i < 16; ++i) fprintf(f, "float_%d=%.5f\n", i, cfg->option_float[i]);
    for (i = 0; i < 16; ++i) fprintf(f, "bool_%d=%d\n", i, cfg->option_bool[i] ? 1 : 0);
    fclose(f);
    return true;
}

bool music_editor_tracker_deserialize_config(music_editor_tracker_config *cfg, const char *path) {
    (void)path;
    if (!cfg) return false;
    music_editor_tracker_config_defaults(cfg);
    return true;
}