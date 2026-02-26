#ifndef PLUGIN_MANAGER_EXT_H
#define PLUGIN_MANAGER_EXT_H

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
} plugin_manager_ext_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} plugin_manager_ext_state;

void plugin_manager_ext_config_defaults(plugin_manager_ext_config *cfg);
void plugin_manager_ext_state_init(plugin_manager_ext_state *state);
bool plugin_manager_ext_validate(const plugin_manager_ext_config *cfg, char *reason, int reason_len);
bool plugin_manager_ext_set_option_int(plugin_manager_ext_config *cfg, int index, int value);
bool plugin_manager_ext_set_option_float(plugin_manager_ext_config *cfg, int index, float value);
bool plugin_manager_ext_set_option_bool(plugin_manager_ext_config *cfg, int index, bool value);
bool plugin_manager_ext_handle_command(plugin_manager_ext_config *cfg, plugin_manager_ext_state *state, const char *command_id);
void plugin_manager_ext_tick(plugin_manager_ext_config *cfg, plugin_manager_ext_state *state);
bool plugin_manager_ext_serialize_config(const plugin_manager_ext_config *cfg, const char *path);
bool plugin_manager_ext_deserialize_config(plugin_manager_ext_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif