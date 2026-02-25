#ifndef PROJECT_DB_H
#define PROJECT_DB_H

#include <stdbool.h>

#define DB_MAX_ASSETS 4096
#define DB_MAX_RECORD_NAME 96
#define DB_MAX_TAG 32

typedef enum {
    DB_ASSET_TILESET,
    DB_ASSET_TILEMAP,
    DB_ASSET_SPRITE,
    DB_ASSET_SOUND,
    DB_ASSET_SCRIPT,
    DB_ASSET_DATA,
    DB_ASSET_OTHER
} DBAssetType;

typedef struct {
    int id;
    DBAssetType type;
    char name[DB_MAX_RECORD_NAME];
    char path[260];
    char tag[DB_MAX_TAG];
    bool dirty;
} DBAssetRecord;

typedef struct {
    DBAssetRecord assets[DB_MAX_ASSETS];
    int asset_count;
    int next_id;
} ProjectDB;

ProjectDB *project_db_create(void);
void project_db_destroy(ProjectDB *db);
int project_db_add_asset(ProjectDB *db, DBAssetType type, const char *name, const char *path, const char *tag);
DBAssetRecord *project_db_find_asset(ProjectDB *db, int id);
bool project_db_update_path(ProjectDB *db, int id, const char *new_path);
bool project_db_mark_clean(ProjectDB *db, int id);
bool project_db_remove_asset(ProjectDB *db, int id);
int project_db_count_dirty(const ProjectDB *db);

#endif
