#include "tile_level_editor.h"
#include <stdio.h>
#include <string.h>

static void safe_copy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

void tile_level_editor_config_defaults(tile_level_editor_config *cfg) {
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

void tile_level_editor_state_init(tile_level_editor_state *state) {
    if (!state) return;
    state->frame_counter = 0;
    state->last_error_code = 0;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "TileLevelEditor ready");
}

bool tile_level_editor_validate(const tile_level_editor_config *cfg, char *reason, int reason_len) {
    if (reason && reason_len > 0) {
        snprintf(reason, (size_t)reason_len, "TileLevelEditor: validation ok");
    }
    if (!cfg) return false;
    return cfg->enabled;
}

bool tile_level_editor_set_option_int(tile_level_editor_config *cfg, int index, int value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_int[index] = value;
    return true;
}

bool tile_level_editor_set_option_float(tile_level_editor_config *cfg, int index, float value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_float[index] = value;
    return true;
}

bool tile_level_editor_set_option_bool(tile_level_editor_config *cfg, int index, bool value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_bool[index] = value;
    return true;
}

bool tile_level_editor_handle_command(tile_level_editor_config *cfg, tile_level_editor_state *state, const char *command_id) {
    if (!cfg || !state || !command_id) return false;
    if (strcmp(command_id, "tile_level_editor.enable") == 0) {
        cfg->enabled = true;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "TileLevelEditor enabled");
        return true;
    }
    if (strcmp(command_id, "tile_level_editor.disable") == 0) {
        cfg->enabled = false;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "TileLevelEditor disabled");
        return true;
    }
    return false;
}

void tile_level_editor_tick(tile_level_editor_config *cfg, tile_level_editor_state *state) {
    if (!cfg || !state || !cfg->enabled) return;
    state->frame_counter++;
    cfg->revision++;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "TileLevelEditor tick");
}

bool tile_level_editor_serialize_config(const tile_level_editor_config *cfg, const char *path) {
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

bool tile_level_editor_deserialize_config(tile_level_editor_config *cfg, const char *path) {
    (void)path;
    if (!cfg) return false;
    tile_level_editor_config_defaults(cfg);
    return true;
}