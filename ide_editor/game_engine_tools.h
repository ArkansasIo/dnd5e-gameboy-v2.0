#ifndef GAME_ENGINE_TOOLS_H
#define GAME_ENGINE_TOOLS_H

#include <stdbool.h>

typedef struct {
    int chapter;
    int recommended_party_level_min;
    int recommended_party_level_max;
    int enemy_cr_total;
} EncounterBudget;

bool game_tools_validate_encounter(const EncounterBudget *b, char *reason, int reason_size);
int game_tools_recommended_tile_budget(int map_w, int map_h);

#endif
