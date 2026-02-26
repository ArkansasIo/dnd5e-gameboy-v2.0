#ifndef GB_STUDIO_TOOLS_H
#define GB_STUDIO_TOOLS_H

#include <stdbool.h>

#define GB_MAX_SCENES 96
#define GB_MAX_ACTORS 512
#define GB_MAX_TRIGGERS 512
#define GB_MAX_SCRIPT_EVENTS 2048

typedef enum {
    GB_SCRIPT_NOP = 0,
    GB_SCRIPT_SET_FLAG = 1,
    GB_SCRIPT_ADD_GOLD = 2,
    GB_SCRIPT_DAMAGE_PLAYER = 3,
    GB_SCRIPT_HEAL_PLAYER = 4,
    GB_SCRIPT_TELEPORT = 5
} GBScriptOpcode;

typedef struct {
    int id;
    char name[48];
    int width;
    int height;
    int bg_tileset_id;
} GBScene;

typedef struct {
    int id;
    int scene_id;
    char name[48];
    char sprite_symbol[32];
    int x;
    int y;
    int move_speed;
    bool hostile;
} GBActor;

typedef struct {
    int id;
    int scene_id;
    char name[48];
    int x;
    int y;
    int width;
    int height;
    int script_event_id;
} GBTrigger;

typedef struct {
    int id;
    GBScriptOpcode opcode;
    int arg0;
    int arg1;
    int arg2;
} GBScriptEvent;

typedef struct {
    GBScene scenes[GB_MAX_SCENES];
    GBActor actors[GB_MAX_ACTORS];
    GBTrigger triggers[GB_MAX_TRIGGERS];
    GBScriptEvent script_events[GB_MAX_SCRIPT_EVENTS];

    int scene_count;
    int actor_count;
    int trigger_count;
    int script_event_count;

    int next_scene_id;
    int next_actor_id;
    int next_trigger_id;
    int next_script_event_id;

    int active_scene_id;

    int player_hp;
    int player_hp_max;
    int player_gold;
    int world_tick;
    unsigned char flags[256];
} GBStudioProject;

void gbstudio_project_init(GBStudioProject *p);
int gbstudio_add_scene(GBStudioProject *p, const char *name, int width, int height, int bg_tileset_id);
int gbstudio_add_actor(GBStudioProject *p, int scene_id, const char *name, const char *sprite_symbol, int x, int y, int move_speed, bool hostile);
int gbstudio_add_trigger(GBStudioProject *p, int scene_id, const char *name, int x, int y, int width, int height, int script_event_id);
int gbstudio_add_script_event(GBStudioProject *p, GBScriptOpcode opcode, int arg0, int arg1, int arg2);

bool gbstudio_validate_scene(const GBStudioProject *p, int scene_id, char *reason, int reason_len);
bool gbstudio_validate_all_scenes(const GBStudioProject *p, char *reason, int reason_len);
bool gbstudio_run_script_event(GBStudioProject *p, int event_id, char *result, int result_len);
void gbstudio_tick_game_logic(GBStudioProject *p);
bool gbstudio_save_project(const GBStudioProject *p, const char *filename);
bool gbstudio_load_project(GBStudioProject *p, const char *filename);

#endif
