#ifndef SCENE_PLATFORMER_H
#define SCENE_PLATFORMER_H

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
} scene_platformer_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} scene_platformer_state;

void scene_platformer_config_defaults(scene_platformer_config *cfg);
void scene_platformer_state_init(scene_platformer_state *state);
bool scene_platformer_validate(const scene_platformer_config *cfg, char *reason, int reason_len);
bool scene_platformer_set_option_int(scene_platformer_config *cfg, int index, int value);
bool scene_platformer_set_option_float(scene_platformer_config *cfg, int index, float value);
bool scene_platformer_set_option_bool(scene_platformer_config *cfg, int index, bool value);
bool scene_platformer_handle_command(scene_platformer_config *cfg, scene_platformer_state *state, const char *command_id);
void scene_platformer_tick(scene_platformer_config *cfg, scene_platformer_state *state);
bool scene_platformer_serialize_config(const scene_platformer_config *cfg, const char *path);
bool scene_platformer_deserialize_config(scene_platformer_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif