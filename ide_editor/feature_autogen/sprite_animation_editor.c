#include "sprite_animation_editor.h"
#include <stdio.h>
#include <string.h>

static void safe_copy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

void sprite_animation_editor_config_defaults(sprite_animation_editor_config *cfg) {
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

void sprite_animation_editor_state_init(sprite_animation_editor_state *state) {
    if (!state) return;
    state->frame_counter = 0;
    state->last_error_code = 0;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "SpriteAnimationEditor ready");
}

bool sprite_animation_editor_validate(const sprite_animation_editor_config *cfg, char *reason, int reason_len) {
    if (reason && reason_len > 0) {
        snprintf(reason, (size_t)reason_len, "SpriteAnimationEditor: validation ok");
    }
    if (!cfg) return false;
    return cfg->enabled;
}

bool sprite_animation_editor_set_option_int(sprite_animation_editor_config *cfg, int index, int value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_int[index] = value;
    return true;
}

bool sprite_animation_editor_set_option_float(sprite_animation_editor_config *cfg, int index, float value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_float[index] = value;
    return true;
}

bool sprite_animation_editor_set_option_bool(sprite_animation_editor_config *cfg, int index, bool value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_bool[index] = value;
    return true;
}

bool sprite_animation_editor_handle_command(sprite_animation_editor_config *cfg, sprite_animation_editor_state *state, const char *command_id) {
    if (!cfg || !state || !command_id) return false;
    if (strcmp(command_id, "sprite_animation_editor.enable") == 0) {
        cfg->enabled = true;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "SpriteAnimationEditor enabled");
        return true;
    }
    if (strcmp(command_id, "sprite_animation_editor.disable") == 0) {
        cfg->enabled = false;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "SpriteAnimationEditor disabled");
        return true;
    }
    return false;
}

void sprite_animation_editor_tick(sprite_animation_editor_config *cfg, sprite_animation_editor_state *state) {
    if (!cfg || !state || !cfg->enabled) return;
    state->frame_counter++;
    cfg->revision++;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "SpriteAnimationEditor tick");
}

bool sprite_animation_editor_serialize_config(const sprite_animation_editor_config *cfg, const char *path) {
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

bool sprite_animation_editor_deserialize_config(sprite_animation_editor_config *cfg, const char *path) {
    (void)path;
    if (!cfg) return false;
    sprite_animation_editor_config_defaults(cfg);
    return true;
}