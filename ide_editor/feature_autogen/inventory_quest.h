#ifndef INVENTORY_QUEST_H
#define INVENTORY_QUEST_H

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
} inventory_quest_config;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} inventory_quest_state;

void inventory_quest_config_defaults(inventory_quest_config *cfg);
void inventory_quest_state_init(inventory_quest_state *state);
bool inventory_quest_validate(const inventory_quest_config *cfg, char *reason, int reason_len);
bool inventory_quest_set_option_int(inventory_quest_config *cfg, int index, int value);
bool inventory_quest_set_option_float(inventory_quest_config *cfg, int index, float value);
bool inventory_quest_set_option_bool(inventory_quest_config *cfg, int index, bool value);
bool inventory_quest_handle_command(inventory_quest_config *cfg, inventory_quest_state *state, const char *command_id);
void inventory_quest_tick(inventory_quest_config *cfg, inventory_quest_state *state);
bool inventory_quest_serialize_config(const inventory_quest_config *cfg, const char *path);
bool inventory_quest_deserialize_config(inventory_quest_config *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif