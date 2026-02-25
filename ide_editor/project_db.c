#include "project_db.h"
#include <stdlib.h>
#include <string.h>

static void copy_str(char *dst, int dst_size, const char *src) {
    if (!dst || dst_size <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(dst_size - 1));
    dst[dst_size - 1] = '\0';
}

ProjectDB *project_db_create(void) {
    ProjectDB *db = (ProjectDB *)malloc(sizeof(ProjectDB));
    if (!db) return NULL;
    db->asset_count = 0;
    db->next_id = 1;
    return db;
}

void project_db_destroy(ProjectDB *db) {
    free(db);
}

int project_db_add_asset(ProjectDB *db, DBAssetType type, const char *name, const char *path, const char *tag) {
    DBAssetRecord *r;
    if (!db || db->asset_count >= DB_MAX_ASSETS) return -1;
    r = &db->assets[db->asset_count++];
    r->id = db->next_id++;
    r->type = type;
    copy_str(r->name, DB_MAX_RECORD_NAME, name);
    copy_str(r->path, 260, path);
    copy_str(r->tag, DB_MAX_TAG, tag);
    r->dirty = true;
    return r->id;
}

DBAssetRecord *project_db_find_asset(ProjectDB *db, int id) {
    int i;
    if (!db) return NULL;
    for (i = 0; i < db->asset_count; i++) {
        if (db->assets[i].id == id) return &db->assets[i];
    }
    return NULL;
}

bool project_db_update_path(ProjectDB *db, int id, const char *new_path) {
    DBAssetRecord *r = project_db_find_asset(db, id);
    if (!r) return false;
    copy_str(r->path, 260, new_path);
    r->dirty = true;
    return true;
}

bool project_db_mark_clean(ProjectDB *db, int id) {
    DBAssetRecord *r = project_db_find_asset(db, id);
    if (!r) return false;
    r->dirty = false;
    return true;
}

bool project_db_remove_asset(ProjectDB *db, int id) {
    int i;
    if (!db) return false;
    for (i = 0; i < db->asset_count; i++) {
        if (db->assets[i].id == id) {
            for (; i < db->asset_count - 1; i++) {
                db->assets[i] = db->assets[i + 1];
            }
            db->asset_count--;
            return true;
        }
    }
    return false;
}

int project_db_count_dirty(const ProjectDB *db) {
    int i;
    int n = 0;
    if (!db) return 0;
    for (i = 0; i < db->asset_count; i++) {
        if (db->assets[i].dirty) n++;
    }
    return n;
}
