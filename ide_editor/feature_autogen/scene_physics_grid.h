#ifndef SCENE_PHYSICS_GRID_H
#define SCENE_PHYSICS_GRID_H

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
} scene_physics_grid_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} scene_physics_grid_state;

void scene_physics_grid_config_defaults(scene_physics_grid_config *cfg);
void scene_physics_grid_state_init(scene_physics_grid_state *state);
bool scene_physics_grid_validate(const scene_physics_grid_config *cfg, char *reason, int reason_len);
bool scene_physics_grid_set_option_int(scene_physics_grid_config *cfg, int index, int value);
bool scene_physics_grid_set_option_float(scene_physics_grid_config *cfg, int index, float value);
bool scene_physics_grid_set_option_bool(scene_physics_grid_config *cfg, int index, bool value);
bool scene_physics_grid_handle_command(scene_physics_grid_config *cfg, scene_physics_grid_state *state, const char *command_id);
void scene_physics_grid_tick(scene_physics_grid_config *cfg, scene_physics_grid_state *state);
bool scene_physics_grid_serialize_config(const scene_physics_grid_config *cfg, const char *path);
bool scene_physics_grid_deserialize_config(scene_physics_grid_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif