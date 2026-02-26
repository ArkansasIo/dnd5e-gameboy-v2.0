#ifndef SCENE_TOPDOWN_H
#define SCENE_TOPDOWN_H

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
} scene_topdown_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} scene_topdown_state;

void scene_topdown_config_defaults(scene_topdown_config *cfg);
void scene_topdown_state_init(scene_topdown_state *state);
bool scene_topdown_validate(const scene_topdown_config *cfg, char *reason, int reason_len);
bool scene_topdown_set_option_int(scene_topdown_config *cfg, int index, int value);
bool scene_topdown_set_option_float(scene_topdown_config *cfg, int index, float value);
bool scene_topdown_set_option_bool(scene_topdown_config *cfg, int index, bool value);
bool scene_topdown_handle_command(scene_topdown_config *cfg, scene_topdown_state *state, const char *command_id);
void scene_topdown_tick(scene_topdown_config *cfg, scene_topdown_state *state);
bool scene_topdown_serialize_config(const scene_topdown_config *cfg, const char *path);
bool scene_topdown_deserialize_config(scene_topdown_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif