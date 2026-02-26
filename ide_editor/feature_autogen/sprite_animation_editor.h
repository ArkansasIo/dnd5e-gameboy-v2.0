#ifndef SPRITE_ANIMATION_EDITOR_H
#define SPRITE_ANIMATION_EDITOR_H

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
} sprite_animation_editor_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} sprite_animation_editor_state;

void sprite_animation_editor_config_defaults(sprite_animation_editor_config *cfg);
void sprite_animation_editor_state_init(sprite_animation_editor_state *state);
bool sprite_animation_editor_validate(const sprite_animation_editor_config *cfg, char *reason, int reason_len);
bool sprite_animation_editor_set_option_int(sprite_animation_editor_config *cfg, int index, int value);
bool sprite_animation_editor_set_option_float(sprite_animation_editor_config *cfg, int index, float value);
bool sprite_animation_editor_set_option_bool(sprite_animation_editor_config *cfg, int index, bool value);
bool sprite_animation_editor_handle_command(sprite_animation_editor_config *cfg, sprite_animation_editor_state *state, const char *command_id);
void sprite_animation_editor_tick(sprite_animation_editor_config *cfg, sprite_animation_editor_state *state);
bool sprite_animation_editor_serialize_config(const sprite_animation_editor_config *cfg, const char *path);
bool sprite_animation_editor_deserialize_config(sprite_animation_editor_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif