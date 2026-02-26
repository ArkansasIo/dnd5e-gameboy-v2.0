#ifndef LOGIC_VARIABLES_FLAGS_H
#define LOGIC_VARIABLES_FLAGS_H

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
} logic_variables_flags_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} logic_variables_flags_state;

void logic_variables_flags_config_defaults(logic_variables_flags_config *cfg);
void logic_variables_flags_state_init(logic_variables_flags_state *state);
bool logic_variables_flags_validate(const logic_variables_flags_config *cfg, char *reason, int reason_len);
bool logic_variables_flags_set_option_int(logic_variables_flags_config *cfg, int index, int value);
bool logic_variables_flags_set_option_float(logic_variables_flags_config *cfg, int index, float value);
bool logic_variables_flags_set_option_bool(logic_variables_flags_config *cfg, int index, bool value);
bool logic_variables_flags_handle_command(logic_variables_flags_config *cfg, logic_variables_flags_state *state, const char *command_id);
void logic_variables_flags_tick(logic_variables_flags_config *cfg, logic_variables_flags_state *state);
bool logic_variables_flags_serialize_config(const logic_variables_flags_config *cfg, const char *path);
bool logic_variables_flags_deserialize_config(logic_variables_flags_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif