#ifndef LOGIC_TRIGGERS_CONDITIONS_H
#define LOGIC_TRIGGERS_CONDITIONS_H

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
} logic_triggers_conditions_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} logic_triggers_conditions_state;

void logic_triggers_conditions_config_defaults(logic_triggers_conditions_config *cfg);
void logic_triggers_conditions_state_init(logic_triggers_conditions_state *state);
bool logic_triggers_conditions_validate(const logic_triggers_conditions_config *cfg, char *reason, int reason_len);
bool logic_triggers_conditions_set_option_int(logic_triggers_conditions_config *cfg, int index, int value);
bool logic_triggers_conditions_set_option_float(logic_triggers_conditions_config *cfg, int index, float value);
bool logic_triggers_conditions_set_option_bool(logic_triggers_conditions_config *cfg, int index, bool value);
bool logic_triggers_conditions_handle_command(logic_triggers_conditions_config *cfg, logic_triggers_conditions_state *state, const char *command_id);
void logic_triggers_conditions_tick(logic_triggers_conditions_config *cfg, logic_triggers_conditions_state *state);
bool logic_triggers_conditions_serialize_config(const logic_triggers_conditions_config *cfg, const char *path);
bool logic_triggers_conditions_deserialize_config(logic_triggers_conditions_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif