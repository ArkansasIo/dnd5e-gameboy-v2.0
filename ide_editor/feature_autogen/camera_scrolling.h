#ifndef CAMERA_SCROLLING_H
#define CAMERA_SCROLLING_H

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
} camera_scrolling_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} camera_scrolling_state;

void camera_scrolling_config_defaults(camera_scrolling_config *cfg);
void camera_scrolling_state_init(camera_scrolling_state *state);
bool camera_scrolling_validate(const camera_scrolling_config *cfg, char *reason, int reason_len);
bool camera_scrolling_set_option_int(camera_scrolling_config *cfg, int index, int value);
bool camera_scrolling_set_option_float(camera_scrolling_config *cfg, int index, float value);
bool camera_scrolling_set_option_bool(camera_scrolling_config *cfg, int index, bool value);
bool camera_scrolling_handle_command(camera_scrolling_config *cfg, camera_scrolling_state *state, const char *command_id);
void camera_scrolling_tick(camera_scrolling_config *cfg, camera_scrolling_state *state);
bool camera_scrolling_serialize_config(const camera_scrolling_config *cfg, const char *path);
bool camera_scrolling_deserialize_config(camera_scrolling_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif