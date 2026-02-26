#ifndef SGB_PALETTE_BORDER_H
#define SGB_PALETTE_BORDER_H

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
} sgb_palette_border_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} sgb_palette_border_state;

void sgb_palette_border_config_defaults(sgb_palette_border_config *cfg);
void sgb_palette_border_state_init(sgb_palette_border_state *state);
bool sgb_palette_border_validate(const sgb_palette_border_config *cfg, char *reason, int reason_len);
bool sgb_palette_border_set_option_int(sgb_palette_border_config *cfg, int index, int value);
bool sgb_palette_border_set_option_float(sgb_palette_border_config *cfg, int index, float value);
bool sgb_palette_border_set_option_bool(sgb_palette_border_config *cfg, int index, bool value);
bool sgb_palette_border_handle_command(sgb_palette_border_config *cfg, sgb_palette_border_state *state, const char *command_id);
void sgb_palette_border_tick(sgb_palette_border_config *cfg, sgb_palette_border_state *state);
bool sgb_palette_border_serialize_config(const sgb_palette_border_config *cfg, const char *path);
bool sgb_palette_border_deserialize_config(sgb_palette_border_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif