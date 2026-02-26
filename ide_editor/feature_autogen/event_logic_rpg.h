#ifndef EVENT_LOGIC_RPG_H
#define EVENT_LOGIC_RPG_H

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
} event_logic_rpg_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} event_logic_rpg_state;

void event_logic_rpg_config_defaults(event_logic_rpg_config *cfg);
void event_logic_rpg_state_init(event_logic_rpg_state *state);
bool event_logic_rpg_validate(const event_logic_rpg_config *cfg, char *reason, int reason_len);
bool event_logic_rpg_set_option_int(event_logic_rpg_config *cfg, int index, int value);
bool event_logic_rpg_set_option_float(event_logic_rpg_config *cfg, int index, float value);
bool event_logic_rpg_set_option_bool(event_logic_rpg_config *cfg, int index, bool value);
bool event_logic_rpg_handle_command(event_logic_rpg_config *cfg, event_logic_rpg_state *state, const char *command_id);
void event_logic_rpg_tick(event_logic_rpg_config *cfg, event_logic_rpg_state *state);
bool event_logic_rpg_serialize_config(const event_logic_rpg_config *cfg, const char *path);
bool event_logic_rpg_deserialize_config(event_logic_rpg_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif