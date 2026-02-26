#ifndef SCENE_SHOOTER_H
#define SCENE_SHOOTER_H

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
} scene_shooter_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} scene_shooter_state;

void scene_shooter_config_defaults(scene_shooter_config *cfg);
void scene_shooter_state_init(scene_shooter_state *state);
bool scene_shooter_validate(const scene_shooter_config *cfg, char *reason, int reason_len);
bool scene_shooter_set_option_int(scene_shooter_config *cfg, int index, int value);
bool scene_shooter_set_option_float(scene_shooter_config *cfg, int index, float value);
bool scene_shooter_set_option_bool(scene_shooter_config *cfg, int index, bool value);
bool scene_shooter_handle_command(scene_shooter_config *cfg, scene_shooter_state *state, const char *command_id);
void scene_shooter_tick(scene_shooter_config *cfg, scene_shooter_state *state);
bool scene_shooter_serialize_config(const scene_shooter_config *cfg, const char *path);
bool scene_shooter_deserialize_config(scene_shooter_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif