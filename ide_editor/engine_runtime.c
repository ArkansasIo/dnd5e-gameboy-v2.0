#include "engine_runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void cpy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

static EREntity *find_entity_by_id(EngineRuntime *rt, int id) {
    int i;
    if (!rt) return NULL;
    for (i = 0; i < rt->entity_count; i++) {
        if (rt->entities[i].id == id) return &rt->entities[i];
    }
    return NULL;
}

static EREntity *first_hostile_alive(EngineRuntime *rt) {
    int i;
    if (!rt) return NULL;
    for (i = 0; i < rt->entity_count; i++) {
        EREntity *e = &rt->entities[i];
        if (e->alive && e->faction == ER_FACTION_HOSTILE) return e;
    }
    return NULL;
}

void engine_runtime_init(EngineRuntime *rt) {
    if (!rt) return;
    memset(rt, 0, sizeof(*rt));
    rt->active_quest = -1;
    rt->player_entity_id = -1;
    cpy(rt->last_event, (int)sizeof(rt->last_event), "runtime initialized");
}

bool engine_runtime_spawn_enemy(EngineRuntime *rt, const char *name, int hp, int x, int y) {
    EREntity *e;
    if (!rt || rt->entity_count >= ER_MAX_ENTITIES) return false;
    e = &rt->entities[rt->entity_count++];
    e->id = rt->entity_count;
    cpy(e->name, (int)sizeof(e->name), name ? name : "Enemy");
    e->hp_max = hp > 0 ? hp : 1;
    e->hp = e->hp_max;
    e->x = x;
    e->y = y;
    e->faction = ER_FACTION_HOSTILE;
    e->alive = true;
    snprintf(rt->last_event, sizeof(rt->last_event), "spawned enemy: %s (%d hp)", e->name, e->hp);
    return true;
}

void engine_runtime_seed_defaults(EngineRuntime *rt) {
    EREntity *p;
    ERQuest *q;
    if (!rt) return;

    if (rt->entity_count < ER_MAX_ENTITIES) {
        p = &rt->entities[rt->entity_count++];
        p->id = rt->entity_count;
        cpy(p->name, (int)sizeof(p->name), "Hero");
        p->hp_max = 30;
        p->hp = 30;
        p->x = 4;
        p->y = 4;
        p->faction = ER_FACTION_PLAYER;
        p->alive = true;
        rt->player_entity_id = p->id;
    }

    if (rt->quest_count < ER_MAX_QUESTS) {
        q = &rt->quests[rt->quest_count++];
        q->id = rt->quest_count;
        cpy(q->title, (int)sizeof(q->title), "Clear the Entry Dungeon");
        cpy(q->objective, (int)sizeof(q->objective), "Defeat 3 hostile creatures near the entrance.");
        q->target_count = 3;
        q->current_count = 0;
        q->reward_gold = 30;
        q->completed = false;
        rt->active_quest = 0;
    }
    if (rt->quest_count < ER_MAX_QUESTS) {
        q = &rt->quests[rt->quest_count++];
        q->id = rt->quest_count;
        cpy(q->title, (int)sizeof(q->title), "Secure Village Route");
        cpy(q->objective, (int)sizeof(q->objective), "Defeat 5 hostiles on the road.");
        q->target_count = 5;
        q->current_count = 0;
        q->reward_gold = 55;
        q->completed = false;
    }

    engine_runtime_spawn_enemy(rt, "Slime", 8, 9, 6);
    engine_runtime_spawn_enemy(rt, "Goblin", 12, 12, 8);
    cpy(rt->last_event, (int)sizeof(rt->last_event), "seeded runtime state");
}

bool engine_runtime_quest_next(EngineRuntime *rt) {
    if (!rt || rt->quest_count <= 0) return false;
    rt->active_quest = (rt->active_quest + 1) % rt->quest_count;
    snprintf(rt->last_event, sizeof(rt->last_event), "active quest: %s", rt->quests[rt->active_quest].title);
    return true;
}

bool engine_runtime_quest_progress(EngineRuntime *rt, int delta) {
    ERQuest *q;
    if (!rt || rt->active_quest < 0 || rt->active_quest >= rt->quest_count) return false;
    q = &rt->quests[rt->active_quest];
    if (q->completed) return false;
    q->current_count += delta;
    if (q->current_count < 0) q->current_count = 0;
    if (q->current_count > q->target_count) q->current_count = q->target_count;
    snprintf(rt->last_event, sizeof(rt->last_event), "quest progress: %s (%d/%d)", q->title, q->current_count, q->target_count);
    return true;
}

bool engine_runtime_quest_complete_active(EngineRuntime *rt) {
    ERQuest *q;
    if (!rt || rt->active_quest < 0 || rt->active_quest >= rt->quest_count) return false;
    q = &rt->quests[rt->active_quest];
    if (q->completed || q->current_count < q->target_count) return false;
    q->completed = true;
    rt->total_gold += q->reward_gold;
    snprintf(rt->last_event, sizeof(rt->last_event), "quest complete: %s (+%d gold)", q->title, q->reward_gold);
    return true;
}

bool engine_runtime_player_attack(EngineRuntime *rt) {
    EREntity *player;
    EREntity *enemy;
    int dmg;
    if (!rt) return false;
    player = find_entity_by_id(rt, rt->player_entity_id);
    enemy = first_hostile_alive(rt);
    if (!player || !player->alive || !enemy) {
        cpy(rt->last_event, (int)sizeof(rt->last_event), "attack skipped: no target");
        return false;
    }
    dmg = 3 + (rt->turn % 6);
    enemy->hp -= dmg;
    if (enemy->hp <= 0) {
        enemy->hp = 0;
        enemy->alive = false;
        engine_runtime_quest_progress(rt, 1);
        snprintf(rt->last_event, sizeof(rt->last_event), "player defeated %s", enemy->name);
    } else {
        snprintf(rt->last_event, sizeof(rt->last_event), "player hit %s for %d", enemy->name, dmg);
    }
    return true;
}

bool engine_runtime_tick(EngineRuntime *rt) {
    EREntity *player;
    int i;
    if (!rt) return false;
    rt->turn++;
    player = find_entity_by_id(rt, rt->player_entity_id);
    if (!player || !player->alive) {
        cpy(rt->last_event, (int)sizeof(rt->last_event), "tick: player unavailable");
        return false;
    }

    for (i = 0; i < rt->entity_count; i++) {
        EREntity *e = &rt->entities[i];
        if (!e->alive || e->faction != ER_FACTION_HOSTILE) continue;
        if (e->x < player->x) e->x++;
        else if (e->x > player->x) e->x--;
        if (e->y < player->y) e->y++;
        else if (e->y > player->y) e->y--;
        if (abs(e->x - player->x) <= 1 && abs(e->y - player->y) <= 1) {
            int dmg = 1 + (rt->turn % 3);
            player->hp -= dmg;
            if (player->hp < 0) player->hp = 0;
            if (player->hp == 0) {
                player->alive = false;
                cpy(rt->last_event, (int)sizeof(rt->last_event), "player was defeated");
                return true;
            }
        }
    }

    if ((rt->turn % 20) == 0 && rt->entity_count < ER_MAX_ENTITIES) {
        char name[32];
        snprintf(name, sizeof(name), "Wanderer%d", rt->turn / 20);
        engine_runtime_spawn_enemy(rt, name, 10 + (rt->turn % 6), 18, 12);
    } else {
        cpy(rt->last_event, (int)sizeof(rt->last_event), "tick advanced");
    }
    return true;
}

void engine_runtime_summary(const EngineRuntime *rt, char *out, int out_len) {
    int i;
    int hostiles_alive = 0;
    int quests_done = 0;
    const ERQuest *q = NULL;
    if (!out || out_len <= 0) return;
    out[0] = '\0';
    if (!rt) return;

    for (i = 0; i < rt->entity_count; i++) {
        if (rt->entities[i].alive && rt->entities[i].faction == ER_FACTION_HOSTILE) hostiles_alive++;
    }
    for (i = 0; i < rt->quest_count; i++) {
        if (rt->quests[i].completed) quests_done++;
    }
    if (rt->active_quest >= 0 && rt->active_quest < rt->quest_count) q = &rt->quests[rt->active_quest];

    snprintf(out, (size_t)out_len,
        "Turn:%d Gold:%d Hostiles:%d Quests:%d/%d Active:%s",
        rt->turn,
        rt->total_gold,
        hostiles_alive,
        quests_done,
        rt->quest_count,
        q ? q->title : "(none)");
}
