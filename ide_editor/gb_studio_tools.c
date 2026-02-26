#include "gb_studio_tools.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void s_copy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

static GBScene *find_scene_mut(GBStudioProject *p, int scene_id) {
    int i;
    if (!p) return NULL;
    for (i = 0; i < p->scene_count; i++) {
        if (p->scenes[i].id == scene_id) return &p->scenes[i];
    }
    return NULL;
}

static const GBScene *find_scene(const GBStudioProject *p, int scene_id) {
    int i;
    if (!p) return NULL;
    for (i = 0; i < p->scene_count; i++) {
        if (p->scenes[i].id == scene_id) return &p->scenes[i];
    }
    return NULL;
}

static const GBScriptEvent *find_script_event(const GBStudioProject *p, int event_id) {
    int i;
    if (!p) return NULL;
    for (i = 0; i < p->script_event_count; i++) {
        if (p->script_events[i].id == event_id) return &p->script_events[i];
    }
    return NULL;
}

void gbstudio_project_init(GBStudioProject *p) {
    if (!p) return;
    memset(p, 0, sizeof(*p));
    p->next_scene_id = 1;
    p->next_actor_id = 1;
    p->next_trigger_id = 1;
    p->next_script_event_id = 1;
    p->player_hp_max = 20;
    p->player_hp = p->player_hp_max;
}

int gbstudio_add_scene(GBStudioProject *p, const char *name, int width, int height, int bg_tileset_id) {
    GBScene *s;
    if (!p || p->scene_count >= GB_MAX_SCENES || width <= 0 || height <= 0) return -1;
    s = &p->scenes[p->scene_count++];
    s->id = p->next_scene_id++;
    s_copy(s->name, (int)sizeof(s->name), name);
    s->width = width;
    s->height = height;
    s->bg_tileset_id = bg_tileset_id;
    if (p->active_scene_id <= 0) p->active_scene_id = s->id;
    return s->id;
}

int gbstudio_add_actor(GBStudioProject *p, int scene_id, const char *name, const char *sprite_symbol, int x, int y, int move_speed, bool hostile) {
    GBActor *a;
    const GBScene *s = find_scene(p, scene_id);
    if (!p || !s || p->actor_count >= GB_MAX_ACTORS) return -1;
    if (x < 0 || y < 0 || x >= s->width || y >= s->height) return -1;

    a = &p->actors[p->actor_count++];
    a->id = p->next_actor_id++;
    a->scene_id = scene_id;
    s_copy(a->name, (int)sizeof(a->name), name);
    s_copy(a->sprite_symbol, (int)sizeof(a->sprite_symbol), sprite_symbol);
    a->x = x;
    a->y = y;
    a->move_speed = move_speed > 0 ? move_speed : 1;
    a->hostile = hostile;
    return a->id;
}

int gbstudio_add_trigger(GBStudioProject *p, int scene_id, const char *name, int x, int y, int width, int height, int script_event_id) {
    GBTrigger *t;
    const GBScene *s = find_scene(p, scene_id);
    if (!p || !s || p->trigger_count >= GB_MAX_TRIGGERS) return -1;
    if (width <= 0 || height <= 0) return -1;
    if (x < 0 || y < 0 || x + width > s->width || y + height > s->height) return -1;

    t = &p->triggers[p->trigger_count++];
    t->id = p->next_trigger_id++;
    t->scene_id = scene_id;
    s_copy(t->name, (int)sizeof(t->name), name);
    t->x = x;
    t->y = y;
    t->width = width;
    t->height = height;
    t->script_event_id = script_event_id;
    return t->id;
}

int gbstudio_add_script_event(GBStudioProject *p, GBScriptOpcode opcode, int arg0, int arg1, int arg2) {
    GBScriptEvent *e;
    if (!p || p->script_event_count >= GB_MAX_SCRIPT_EVENTS) return -1;
    e = &p->script_events[p->script_event_count++];
    e->id = p->next_script_event_id++;
    e->opcode = opcode;
    e->arg0 = arg0;
    e->arg1 = arg1;
    e->arg2 = arg2;
    return e->id;
}

bool gbstudio_validate_scene(const GBStudioProject *p, int scene_id, char *reason, int reason_len) {
    int i;
    int actor_count = 0;
    int trigger_count = 0;
    const GBScene *s = find_scene(p, scene_id);
    if (!reason || reason_len <= 0) return false;
    reason[0] = '\0';
    if (!p || !s) {
        snprintf(reason, (size_t)reason_len, "Invalid scene id");
        return false;
    }

    for (i = 0; i < p->actor_count; i++) {
        if (p->actors[i].scene_id == scene_id) actor_count++;
    }
    for (i = 0; i < p->trigger_count; i++) {
        if (p->triggers[i].scene_id == scene_id) trigger_count++;
    }

    if (actor_count > 32) {
        snprintf(reason, (size_t)reason_len, "Scene %s has too many actors (%d > 32)", s->name, actor_count);
        return false;
    }
    if (trigger_count > 24) {
        snprintf(reason, (size_t)reason_len, "Scene %s has too many triggers (%d > 24)", s->name, trigger_count);
        return false;
    }

    snprintf(reason, (size_t)reason_len, "Scene %s validated (actors=%d, triggers=%d)", s->name, actor_count, trigger_count);
    return true;
}

bool gbstudio_validate_all_scenes(const GBStudioProject *p, char *reason, int reason_len) {
    int i;
    char tmp[128];
    if (!reason || reason_len <= 0) return false;
    reason[0] = '\0';
    if (!p) {
        snprintf(reason, (size_t)reason_len, "Project is null");
        return false;
    }
    if (p->scene_count <= 0) {
        snprintf(reason, (size_t)reason_len, "No scenes to validate");
        return false;
    }

    for (i = 0; i < p->scene_count; i++) {
        if (!gbstudio_validate_scene(p, p->scenes[i].id, tmp, (int)sizeof(tmp))) {
            snprintf(reason, (size_t)reason_len, "Scene validation failed: %s", tmp);
            return false;
        }
    }
    snprintf(reason, (size_t)reason_len, "All %d scenes validated", p->scene_count);
    return true;
}

bool gbstudio_run_script_event(GBStudioProject *p, int event_id, char *result, int result_len) {
    const GBScriptEvent *e = find_script_event(p, event_id);
    if (!result || result_len <= 0) return false;
    result[0] = '\0';
    if (!p || !e) {
        snprintf(result, (size_t)result_len, "Script event not found");
        return false;
    }

    switch (e->opcode) {
        case GB_SCRIPT_SET_FLAG:
            if (e->arg0 < 0 || e->arg0 >= (int)sizeof(p->flags)) {
                snprintf(result, (size_t)result_len, "SET_FLAG out of range");
                return false;
            }
            p->flags[e->arg0] = (unsigned char)(e->arg1 ? 1 : 0);
            snprintf(result, (size_t)result_len, "Flag %d set to %d", e->arg0, p->flags[e->arg0]);
            return true;
        case GB_SCRIPT_ADD_GOLD:
            p->player_gold += e->arg0;
            if (p->player_gold < 0) p->player_gold = 0;
            snprintf(result, (size_t)result_len, "Gold is now %d", p->player_gold);
            return true;
        case GB_SCRIPT_DAMAGE_PLAYER:
            p->player_hp -= e->arg0;
            if (p->player_hp < 0) p->player_hp = 0;
            snprintf(result, (size_t)result_len, "Player HP %d/%d", p->player_hp, p->player_hp_max);
            return true;
        case GB_SCRIPT_HEAL_PLAYER:
            p->player_hp += e->arg0;
            if (p->player_hp > p->player_hp_max) p->player_hp = p->player_hp_max;
            snprintf(result, (size_t)result_len, "Player HP %d/%d", p->player_hp, p->player_hp_max);
            return true;
        case GB_SCRIPT_TELEPORT:
            if (!find_scene_mut(p, e->arg0)) {
                snprintf(result, (size_t)result_len, "Teleport target scene missing");
                return false;
            }
            p->active_scene_id = e->arg0;
            snprintf(result, (size_t)result_len, "Teleported to scene %d", p->active_scene_id);
            return true;
        case GB_SCRIPT_NOP:
        default:
            snprintf(result, (size_t)result_len, "No-op script event");
            return true;
    }
}

void gbstudio_tick_game_logic(GBStudioProject *p) {
    int i;
    if (!p) return;

    p->world_tick++;
    if ((p->world_tick % 120) == 0 && p->player_hp < p->player_hp_max) {
        p->player_hp++;
    }

    for (i = 0; i < p->actor_count; i++) {
        GBActor *a = &p->actors[i];
        const GBScene *s;
        if (a->scene_id != p->active_scene_id || !a->hostile) continue;
        s = find_scene(p, a->scene_id);
        if (!s) continue;

        if ((p->world_tick % 60) == 0) {
            a->x += (a->move_speed > 1) ? 2 : 1;
            if (a->x >= s->width) a->x = 0;
        }
    }
}

bool gbstudio_save_project(const GBStudioProject *p, const char *filename) {
    int i;
    FILE *f;
    if (!p || !filename) return false;
    f = fopen(filename, "w");
    if (!f) return false;

    fprintf(f, "[meta]\n");
    fprintf(f, "scene_count=%d\n", p->scene_count);
    fprintf(f, "actor_count=%d\n", p->actor_count);
    fprintf(f, "trigger_count=%d\n", p->trigger_count);
    fprintf(f, "script_event_count=%d\n", p->script_event_count);
    fprintf(f, "active_scene_id=%d\n", p->active_scene_id);
    fprintf(f, "player_hp=%d\n", p->player_hp);
    fprintf(f, "player_hp_max=%d\n", p->player_hp_max);
    fprintf(f, "player_gold=%d\n", p->player_gold);
    fprintf(f, "world_tick=%d\n", p->world_tick);
    fprintf(f, "\n[scenes]\n");
    for (i = 0; i < p->scene_count; i++) {
        const GBScene *s = &p->scenes[i];
        fprintf(f, "%d|%s|%d|%d|%d\n", s->id, s->name, s->width, s->height, s->bg_tileset_id);
    }
    fprintf(f, "\n[actors]\n");
    for (i = 0; i < p->actor_count; i++) {
        const GBActor *a = &p->actors[i];
        fprintf(f, "%d|%d|%s|%s|%d|%d|%d|%d\n",
            a->id, a->scene_id, a->name, a->sprite_symbol, a->x, a->y, a->move_speed, a->hostile ? 1 : 0);
    }
    fprintf(f, "\n[triggers]\n");
    for (i = 0; i < p->trigger_count; i++) {
        const GBTrigger *t = &p->triggers[i];
        fprintf(f, "%d|%d|%s|%d|%d|%d|%d|%d\n",
            t->id, t->scene_id, t->name, t->x, t->y, t->width, t->height, t->script_event_id);
    }
    fprintf(f, "\n[scripts]\n");
    for (i = 0; i < p->script_event_count; i++) {
        const GBScriptEvent *e = &p->script_events[i];
        fprintf(f, "%d|%d|%d|%d|%d\n", e->id, (int)e->opcode, e->arg0, e->arg1, e->arg2);
    }

    fclose(f);
    return true;
}

bool gbstudio_load_project(GBStudioProject *p, const char *filename) {
    enum { SEC_NONE, SEC_SCENES, SEC_ACTORS, SEC_TRIGGERS, SEC_SCRIPTS } sec = SEC_NONE;
    char line[512];
    FILE *f;
    if (!p || !filename) return false;
    f = fopen(filename, "r");
    if (!f) return false;

    gbstudio_project_init(p);
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n' || line[0] == '\r') continue;
        if (line[0] == '[') {
            if (strncmp(line, "[scenes]", 8) == 0) sec = SEC_SCENES;
            else if (strncmp(line, "[actors]", 8) == 0) sec = SEC_ACTORS;
            else if (strncmp(line, "[triggers]", 10) == 0) sec = SEC_TRIGGERS;
            else if (strncmp(line, "[scripts]", 9) == 0) sec = SEC_SCRIPTS;
            else sec = SEC_NONE;
            continue;
        }
        if (strncmp(line, "active_scene_id=", 16) == 0) {
            p->active_scene_id = atoi(line + 16);
            continue;
        }
        if (strncmp(line, "player_hp=", 10) == 0) {
            p->player_hp = atoi(line + 10);
            continue;
        }
        if (strncmp(line, "player_hp_max=", 14) == 0) {
            p->player_hp_max = atoi(line + 14);
            continue;
        }
        if (strncmp(line, "player_gold=", 12) == 0) {
            p->player_gold = atoi(line + 12);
            continue;
        }
        if (strncmp(line, "world_tick=", 11) == 0) {
            p->world_tick = atoi(line + 11);
            continue;
        }

        if (sec == SEC_SCENES) {
            int id, w, h, tileset;
            char name[48];
            if (sscanf(line, "%d|%47[^|]|%d|%d|%d", &id, name, &w, &h, &tileset) == 5 && p->scene_count < GB_MAX_SCENES) {
                GBScene *s = &p->scenes[p->scene_count++];
                s->id = id;
                s_copy(s->name, (int)sizeof(s->name), name);
                s->width = w;
                s->height = h;
                s->bg_tileset_id = tileset;
                if (id >= p->next_scene_id) p->next_scene_id = id + 1;
            }
        } else if (sec == SEC_ACTORS) {
            int id, scene_id, x, y, speed, hostile;
            char name[48];
            char sprite[32];
            if (sscanf(line, "%d|%d|%47[^|]|%31[^|]|%d|%d|%d|%d", &id, &scene_id, name, sprite, &x, &y, &speed, &hostile) == 8 &&
                p->actor_count < GB_MAX_ACTORS) {
                GBActor *a = &p->actors[p->actor_count++];
                a->id = id;
                a->scene_id = scene_id;
                s_copy(a->name, (int)sizeof(a->name), name);
                s_copy(a->sprite_symbol, (int)sizeof(a->sprite_symbol), sprite);
                a->x = x;
                a->y = y;
                a->move_speed = speed;
                a->hostile = hostile ? true : false;
                if (id >= p->next_actor_id) p->next_actor_id = id + 1;
            }
        } else if (sec == SEC_TRIGGERS) {
            int id, scene_id, x, y, w, h, script_id;
            char name[48];
            if (sscanf(line, "%d|%d|%47[^|]|%d|%d|%d|%d|%d", &id, &scene_id, name, &x, &y, &w, &h, &script_id) == 8 &&
                p->trigger_count < GB_MAX_TRIGGERS) {
                GBTrigger *t = &p->triggers[p->trigger_count++];
                t->id = id;
                t->scene_id = scene_id;
                s_copy(t->name, (int)sizeof(t->name), name);
                t->x = x;
                t->y = y;
                t->width = w;
                t->height = h;
                t->script_event_id = script_id;
                if (id >= p->next_trigger_id) p->next_trigger_id = id + 1;
            }
        } else if (sec == SEC_SCRIPTS) {
            int id, op, arg0, arg1, arg2;
            if (sscanf(line, "%d|%d|%d|%d|%d", &id, &op, &arg0, &arg1, &arg2) == 5 && p->script_event_count < GB_MAX_SCRIPT_EVENTS) {
                GBScriptEvent *e = &p->script_events[p->script_event_count++];
                e->id = id;
                e->opcode = (GBScriptOpcode)op;
                e->arg0 = arg0;
                e->arg1 = arg1;
                e->arg2 = arg2;
                if (id >= p->next_script_event_id) p->next_script_event_id = id + 1;
            }
        }
    }

    fclose(f);
    return true;
}
