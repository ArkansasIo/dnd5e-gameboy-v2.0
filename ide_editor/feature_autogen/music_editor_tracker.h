#ifndef MUSIC_EDITOR_TRACKER_H
#define MUSIC_EDITOR_TRACKER_H

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
} music_editor_tracker_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} music_editor_tracker_state;

void music_editor_tracker_config_defaults(music_editor_tracker_config *cfg);
void music_editor_tracker_state_init(music_editor_tracker_state *state);
bool music_editor_tracker_validate(const music_editor_tracker_config *cfg, char *reason, int reason_len);
bool music_editor_tracker_set_option_int(music_editor_tracker_config *cfg, int index, int value);
bool music_editor_tracker_set_option_float(music_editor_tracker_config *cfg, int index, float value);
bool music_editor_tracker_set_option_bool(music_editor_tracker_config *cfg, int index, bool value);
bool music_editor_tracker_handle_command(music_editor_tracker_config *cfg, music_editor_tracker_state *state, const char *command_id);
void music_editor_tracker_tick(music_editor_tracker_config *cfg, music_editor_tracker_state *state);
bool music_editor_tracker_serialize_config(const music_editor_tracker_config *cfg, const char *path);
bool music_editor_tracker_deserialize_config(music_editor_tracker_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif