#ifndef FONT_PNG_JSON_H
#define FONT_PNG_JSON_H

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
} font_png_json_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} font_png_json_state;

void font_png_json_config_defaults(font_png_json_config *cfg);
void font_png_json_state_init(font_png_json_state *state);
bool font_png_json_validate(const font_png_json_config *cfg, char *reason, int reason_len);
bool font_png_json_set_option_int(font_png_json_config *cfg, int index, int value);
bool font_png_json_set_option_float(font_png_json_config *cfg, int index, float value);
bool font_png_json_set_option_bool(font_png_json_config *cfg, int index, bool value);
bool font_png_json_handle_command(font_png_json_config *cfg, font_png_json_state *state, const char *command_id);
void font_png_json_tick(font_png_json_config *cfg, font_png_json_state *state);
bool font_png_json_serialize_config(const font_png_json_config *cfg, const char *path);
bool font_png_json_deserialize_config(font_png_json_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif