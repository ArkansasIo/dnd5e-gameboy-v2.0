#ifndef SCENE_MIXER_H
#define SCENE_MIXER_H

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
} scene_mixer_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} scene_mixer_state;

void scene_mixer_config_defaults(scene_mixer_config *cfg);
void scene_mixer_state_init(scene_mixer_state *state);
bool scene_mixer_validate(const scene_mixer_config *cfg, char *reason, int reason_len);
bool scene_mixer_set_option_int(scene_mixer_config *cfg, int index, int value);
bool scene_mixer_set_option_float(scene_mixer_config *cfg, int index, float value);
bool scene_mixer_set_option_bool(scene_mixer_config *cfg, int index, bool value);
bool scene_mixer_handle_command(scene_mixer_config *cfg, scene_mixer_state *state, const char *command_id);
void scene_mixer_tick(scene_mixer_config *cfg, scene_mixer_state *state);
bool scene_mixer_serialize_config(const scene_mixer_config *cfg, const char *path);
bool scene_mixer_deserialize_config(scene_mixer_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif