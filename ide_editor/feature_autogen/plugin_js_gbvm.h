#ifndef PLUGIN_JS_GBVM_H
#define PLUGIN_JS_GBVM_H

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
} plugin_js_gbvm_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} plugin_js_gbvm_state;

void plugin_js_gbvm_config_defaults(plugin_js_gbvm_config *cfg);
void plugin_js_gbvm_state_init(plugin_js_gbvm_state *state);
bool plugin_js_gbvm_validate(const plugin_js_gbvm_config *cfg, char *reason, int reason_len);
bool plugin_js_gbvm_set_option_int(plugin_js_gbvm_config *cfg, int index, int value);
bool plugin_js_gbvm_set_option_float(plugin_js_gbvm_config *cfg, int index, float value);
bool plugin_js_gbvm_set_option_bool(plugin_js_gbvm_config *cfg, int index, bool value);
bool plugin_js_gbvm_handle_command(plugin_js_gbvm_config *cfg, plugin_js_gbvm_state *state, const char *command_id);
void plugin_js_gbvm_tick(plugin_js_gbvm_config *cfg, plugin_js_gbvm_state *state);
bool plugin_js_gbvm_serialize_config(const plugin_js_gbvm_config *cfg, const char *path);
bool plugin_js_gbvm_deserialize_config(plugin_js_gbvm_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif