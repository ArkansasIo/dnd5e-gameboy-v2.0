#ifndef RUNTIME_OPTIMIZATION_H
#define RUNTIME_OPTIMIZATION_H

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
} runtime_optimization_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} runtime_optimization_state;

void runtime_optimization_config_defaults(runtime_optimization_config *cfg);
void runtime_optimization_state_init(runtime_optimization_state *state);
bool runtime_optimization_validate(const runtime_optimization_config *cfg, char *reason, int reason_len);
bool runtime_optimization_set_option_int(runtime_optimization_config *cfg, int index, int value);
bool runtime_optimization_set_option_float(runtime_optimization_config *cfg, int index, float value);
bool runtime_optimization_set_option_bool(runtime_optimization_config *cfg, int index, bool value);
bool runtime_optimization_handle_command(runtime_optimization_config *cfg, runtime_optimization_state *state, const char *command_id);
void runtime_optimization_tick(runtime_optimization_config *cfg, runtime_optimization_state *state);
bool runtime_optimization_serialize_config(const runtime_optimization_config *cfg, const char *path);
bool runtime_optimization_deserialize_config(runtime_optimization_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif