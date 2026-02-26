#ifndef UI_SKIN_EDITOR_H
#define UI_SKIN_EDITOR_H

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
} ui_skin_editor_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} ui_skin_editor_state;

void ui_skin_editor_config_defaults(ui_skin_editor_config *cfg);
void ui_skin_editor_state_init(ui_skin_editor_state *state);
bool ui_skin_editor_validate(const ui_skin_editor_config *cfg, char *reason, int reason_len);
bool ui_skin_editor_set_option_int(ui_skin_editor_config *cfg, int index, int value);
bool ui_skin_editor_set_option_float(ui_skin_editor_config *cfg, int index, float value);
bool ui_skin_editor_set_option_bool(ui_skin_editor_config *cfg, int index, bool value);
bool ui_skin_editor_handle_command(ui_skin_editor_config *cfg, ui_skin_editor_state *state, const char *command_id);
void ui_skin_editor_tick(ui_skin_editor_config *cfg, ui_skin_editor_state *state);
bool ui_skin_editor_serialize_config(const ui_skin_editor_config *cfg, const char *path);
bool ui_skin_editor_deserialize_config(ui_skin_editor_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif