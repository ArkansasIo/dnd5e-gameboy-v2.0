#include "game_engine_tools.h"
#include <stdio.h>

bool game_tools_validate_encounter(const EncounterBudget *b, char *reason, int reason_size) {
    int party_mid;
    int safe_max;
    if (!b) return false;
    party_mid = (b->recommended_party_level_min + b->recommended_party_level_max) / 2;
    safe_max = party_mid * 4;
    if (b->enemy_cr_total > safe_max) {
        if (reason && reason_size > 0) {
            snprintf(reason, (size_t)reason_size, "CR budget too high (%d > %d)", b->enemy_cr_total, safe_max);
        }
        return false;
    }
    if (reason && reason_size > 0) {
        snprintf(reason, (size_t)reason_size, "OK");
    }
    return true;
}

int game_tools_recommended_tile_budget(int map_w, int map_h) {
    if (map_w <= 0 || map_h <= 0) return 0;
    return (map_w * map_h) / 2;
}
