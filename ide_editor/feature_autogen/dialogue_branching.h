#ifndef DIALOGUE_BRANCHING_H
#define DIALOGUE_BRANCHING_H

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
} dialogue_branching_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} dialogue_branching_state;

void dialogue_branching_config_defaults(dialogue_branching_config *cfg);
void dialogue_branching_state_init(dialogue_branching_state *state);
bool dialogue_branching_validate(const dialogue_branching_config *cfg, char *reason, int reason_len);
bool dialogue_branching_set_option_int(dialogue_branching_config *cfg, int index, int value);
bool dialogue_branching_set_option_float(dialogue_branching_config *cfg, int index, float value);
bool dialogue_branching_set_option_bool(dialogue_branching_config *cfg, int index, bool value);
bool dialogue_branching_handle_command(dialogue_branching_config *cfg, dialogue_branching_state *state, const char *command_id);
void dialogue_branching_tick(dialogue_branching_config *cfg, dialogue_branching_state *state);
bool dialogue_branching_serialize_config(const dialogue_branching_config *cfg, const char *path);
bool dialogue_branching_deserialize_config(dialogue_branching_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif