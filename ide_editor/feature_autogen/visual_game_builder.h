#ifndef VISUAL_GAME_BUILDER_H
#define VISUAL_GAME_BUILDER_H

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
} visual_game_builder_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} visual_game_builder_state;

void visual_game_builder_config_defaults(visual_game_builder_config *cfg);
void visual_game_builder_state_init(visual_game_builder_state *state);
bool visual_game_builder_validate(const visual_game_builder_config *cfg, char *reason, int reason_len);
bool visual_game_builder_set_option_int(visual_game_builder_config *cfg, int index, int value);
bool visual_game_builder_set_option_float(visual_game_builder_config *cfg, int index, float value);
bool visual_game_builder_set_option_bool(visual_game_builder_config *cfg, int index, bool value);
bool visual_game_builder_handle_command(visual_game_builder_config *cfg, visual_game_builder_state *state, const char *command_id);
void visual_game_builder_tick(visual_game_builder_config *cfg, visual_game_builder_state *state);
bool visual_game_builder_serialize_config(const visual_game_builder_config *cfg, const char *path);
bool visual_game_builder_deserialize_config(visual_game_builder_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif