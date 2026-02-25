#ifndef ENGINE_RUNTIME_H
#define ENGINE_RUNTIME_H

#include <stdbool.h>

#define ER_MAX_ENTITIES 64
#define ER_MAX_QUESTS 32

typedef enum {
    ER_FACTION_PLAYER = 0,
    ER_FACTION_FRIENDLY = 1,
    ER_FACTION_HOSTILE = 2
} ERFaction;

typedef struct {
    int id;
    char name[40];
    int hp;
    int hp_max;
    int x;
    int y;
    ERFaction faction;
    bool alive;
} EREntity;

typedef struct {
    int id;
    char title[64];
    char objective[96];
    int target_count;
    int current_count;
    int reward_gold;
    bool completed;
} ERQuest;

typedef struct {
    EREntity entities[ER_MAX_ENTITIES];
    ERQuest quests[ER_MAX_QUESTS];
    int entity_count;
    int quest_count;
    int active_quest;
    int turn;
    int player_entity_id;
    int total_gold;
    char last_event[128];
} EngineRuntime;

void engine_runtime_init(EngineRuntime *rt);
void engine_runtime_seed_defaults(EngineRuntime *rt);
bool engine_runtime_tick(EngineRuntime *rt);
bool engine_runtime_spawn_enemy(EngineRuntime *rt, const char *name, int hp, int x, int y);
bool engine_runtime_player_attack(EngineRuntime *rt);
bool engine_runtime_quest_next(EngineRuntime *rt);
bool engine_runtime_quest_progress(EngineRuntime *rt, int delta);
bool engine_runtime_quest_complete_active(EngineRuntime *rt);
void engine_runtime_summary(const EngineRuntime *rt, char *out, int out_len);

#endif
