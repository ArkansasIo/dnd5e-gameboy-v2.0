#ifndef GRAVITY_TUNING_H
#define GRAVITY_TUNING_H

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
} gravity_tuning_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} gravity_tuning_state;

void gravity_tuning_config_defaults(gravity_tuning_config *cfg);
void gravity_tuning_state_init(gravity_tuning_state *state);
bool gravity_tuning_validate(const gravity_tuning_config *cfg, char *reason, int reason_len);
bool gravity_tuning_set_option_int(gravity_tuning_config *cfg, int index, int value);
bool gravity_tuning_set_option_float(gravity_tuning_config *cfg, int index, float value);
bool gravity_tuning_set_option_bool(gravity_tuning_config *cfg, int index, bool value);
bool gravity_tuning_handle_command(gravity_tuning_config *cfg, gravity_tuning_state *state, const char *command_id);
void gravity_tuning_tick(gravity_tuning_config *cfg, gravity_tuning_state *state);
bool gravity_tuning_serialize_config(const gravity_tuning_config *cfg, const char *path);
bool gravity_tuning_deserialize_config(gravity_tuning_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif