#ifndef HUGE_TRACKER_BRIDGE_H
#define HUGE_TRACKER_BRIDGE_H

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
} huge_tracker_bridge_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} huge_tracker_bridge_state;

void huge_tracker_bridge_config_defaults(huge_tracker_bridge_config *cfg);
void huge_tracker_bridge_state_init(huge_tracker_bridge_state *state);
bool huge_tracker_bridge_validate(const huge_tracker_bridge_config *cfg, char *reason, int reason_len);
bool huge_tracker_bridge_set_option_int(huge_tracker_bridge_config *cfg, int index, int value);
bool huge_tracker_bridge_set_option_float(huge_tracker_bridge_config *cfg, int index, float value);
bool huge_tracker_bridge_set_option_bool(huge_tracker_bridge_config *cfg, int index, bool value);
bool huge_tracker_bridge_handle_command(huge_tracker_bridge_config *cfg, huge_tracker_bridge_state *state, const char *command_id);
void huge_tracker_bridge_tick(huge_tracker_bridge_config *cfg, huge_tracker_bridge_state *state);
bool huge_tracker_bridge_serialize_config(const huge_tracker_bridge_config *cfg, const char *path);
bool huge_tracker_bridge_deserialize_config(huge_tracker_bridge_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif