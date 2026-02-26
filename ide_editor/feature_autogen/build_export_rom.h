#ifndef BUILD_EXPORT_ROM_H
#define BUILD_EXPORT_ROM_H

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
} build_export_rom_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} build_export_rom_state;

void build_export_rom_config_defaults(build_export_rom_config *cfg);
void build_export_rom_state_init(build_export_rom_state *state);
bool build_export_rom_validate(const build_export_rom_config *cfg, char *reason, int reason_len);
bool build_export_rom_set_option_int(build_export_rom_config *cfg, int index, int value);
bool build_export_rom_set_option_float(build_export_rom_config *cfg, int index, float value);
bool build_export_rom_set_option_bool(build_export_rom_config *cfg, int index, bool value);
bool build_export_rom_handle_command(build_export_rom_config *cfg, build_export_rom_state *state, const char *command_id);
void build_export_rom_tick(build_export_rom_config *cfg, build_export_rom_state *state);
bool build_export_rom_serialize_config(const build_export_rom_config *cfg, const char *path);
bool build_export_rom_deserialize_config(build_export_rom_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif