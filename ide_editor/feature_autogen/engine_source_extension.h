#ifndef ENGINE_SOURCE_EXTENSION_H
#define ENGINE_SOURCE_EXTENSION_H

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
} engine_source_extension_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} engine_source_extension_state;

void engine_source_extension_config_defaults(engine_source_extension_config *cfg);
void engine_source_extension_state_init(engine_source_extension_state *state);
bool engine_source_extension_validate(const engine_source_extension_config *cfg, char *reason, int reason_len);
bool engine_source_extension_set_option_int(engine_source_extension_config *cfg, int index, int value);
bool engine_source_extension_set_option_float(engine_source_extension_config *cfg, int index, float value);
bool engine_source_extension_set_option_bool(engine_source_extension_config *cfg, int index, bool value);
bool engine_source_extension_handle_command(engine_source_extension_config *cfg, engine_source_extension_state *state, const char *command_id);
void engine_source_extension_tick(engine_source_extension_config *cfg, engine_source_extension_state *state);
bool engine_source_extension_serialize_config(const engine_source_extension_config *cfg, const char *path);
bool engine_source_extension_deserialize_config(engine_source_extension_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif