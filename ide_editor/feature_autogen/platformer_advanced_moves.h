#ifndef PLATFORMER_ADVANCED_MOVES_H
#define PLATFORMER_ADVANCED_MOVES_H

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
} platformer_advanced_moves_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} platformer_advanced_moves_state;

void platformer_advanced_moves_config_defaults(platformer_advanced_moves_config *cfg);
void platformer_advanced_moves_state_init(platformer_advanced_moves_state *state);
bool platformer_advanced_moves_validate(const platformer_advanced_moves_config *cfg, char *reason, int reason_len);
bool platformer_advanced_moves_set_option_int(platformer_advanced_moves_config *cfg, int index, int value);
bool platformer_advanced_moves_set_option_float(platformer_advanced_moves_config *cfg, int index, float value);
bool platformer_advanced_moves_set_option_bool(platformer_advanced_moves_config *cfg, int index, bool value);
bool platformer_advanced_moves_handle_command(platformer_advanced_moves_config *cfg, platformer_advanced_moves_state *state, const char *command_id);
void platformer_advanced_moves_tick(platformer_advanced_moves_config *cfg, platformer_advanced_moves_state *state);
bool platformer_advanced_moves_serialize_config(const platformer_advanced_moves_config *cfg, const char *path);
bool platformer_advanced_moves_deserialize_config(platformer_advanced_moves_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif