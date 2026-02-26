#ifndef RUNTIME_TARGETS_H
#define RUNTIME_TARGETS_H

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
} runtime_targets_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} runtime_targets_state;

void runtime_targets_config_defaults(runtime_targets_config *cfg);
void runtime_targets_state_init(runtime_targets_state *state);
bool runtime_targets_validate(const runtime_targets_config *cfg, char *reason, int reason_len);
bool runtime_targets_set_option_int(runtime_targets_config *cfg, int index, int value);
bool runtime_targets_set_option_float(runtime_targets_config *cfg, int index, float value);
bool runtime_targets_set_option_bool(runtime_targets_config *cfg, int index, bool value);
bool runtime_targets_handle_command(runtime_targets_config *cfg, runtime_targets_state *state, const char *command_id);
void runtime_targets_tick(runtime_targets_config *cfg, runtime_targets_state *state);
bool runtime_targets_serialize_config(const runtime_targets_config *cfg, const char *path);
bool runtime_targets_deserialize_config(runtime_targets_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif