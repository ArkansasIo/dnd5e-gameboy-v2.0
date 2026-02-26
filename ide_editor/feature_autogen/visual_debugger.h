#ifndef VISUAL_DEBUGGER_H
#define VISUAL_DEBUGGER_H

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
} visual_debugger_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} visual_debugger_state;

void visual_debugger_config_defaults(visual_debugger_config *cfg);
void visual_debugger_state_init(visual_debugger_state *state);
bool visual_debugger_validate(const visual_debugger_config *cfg, char *reason, int reason_len);
bool visual_debugger_set_option_int(visual_debugger_config *cfg, int index, int value);
bool visual_debugger_set_option_float(visual_debugger_config *cfg, int index, float value);
bool visual_debugger_set_option_bool(visual_debugger_config *cfg, int index, bool value);
bool visual_debugger_handle_command(visual_debugger_config *cfg, visual_debugger_state *state, const char *command_id);
void visual_debugger_tick(visual_debugger_config *cfg, visual_debugger_state *state);
bool visual_debugger_serialize_config(const visual_debugger_config *cfg, const char *path);
bool visual_debugger_deserialize_config(visual_debugger_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif