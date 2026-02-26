#ifndef TILE_LEVEL_EDITOR_H
#define TILE_LEVEL_EDITOR_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool enabled;
    int revision;
    int option_int[16];
    float option_float[16];
    bool option_bool[16];
} tile_level_editor_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} tile_level_editor_state;

void tile_level_editor_config_defaults(tile_level_editor_config *cfg);
void tile_level_editor_state_init(tile_level_editor_state *state);
bool tile_level_editor_validate(const tile_level_editor_config *cfg, char *reason, int reason_len);
bool tile_level_editor_set_option_int(tile_level_editor_config *cfg, int index, int value);
bool tile_level_editor_set_option_float(tile_level_editor_config *cfg, int index, float value);
bool tile_level_editor_set_option_bool(tile_level_editor_config *cfg, int index, bool value);
bool tile_level_editor_handle_command(tile_level_editor_config *cfg, tile_level_editor_state *state, const char *command_id);
void tile_level_editor_tick(tile_level_editor_config *cfg, tile_level_editor_state *state);
bool tile_level_editor_serialize_config(const tile_level_editor_config *cfg, const char *path);
bool tile_level_editor_deserialize_config(tile_level_editor_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif