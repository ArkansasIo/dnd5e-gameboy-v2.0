#include "project_io.h"
#include <stdio.h>
#include <string.h>

bool project_io_save(const char *filename, const ProjectDB *db, const TileMap *map) {
    int i;
    FILE *f;
    if (!filename || !db) return false;
    f = fopen(filename, "w");
    if (!f) return false;

    fprintf(f, "[project]\n");
    fprintf(f, "asset_count=%d\n", db->asset_count);
    fprintf(f, "map_size=%dx%d\n", map ? map->width : 0, map ? map->height : 0);
    fprintf(f, "\n[assets]\n");
    for (i = 0; i < db->asset_count; i++) {
        const DBAssetRecord *a = &db->assets[i];
        fprintf(f, "%d|%d|%s|%s|%s|%d\n", a->id, (int)a->type, a->name, a->path, a->tag, a->dirty ? 1 : 0);
    }

    fclose(f);
    return true;
}

bool project_io_load(const char *filename, ProjectDB *db) {
    char line[512];
    FILE *f;
    if (!filename || !db) return false;
    f = fopen(filename, "r");
    if (!f) return false;

    db->asset_count = 0;
    while (fgets(line, sizeof(line), f)) {
        int id, type, dirty;
        char name[DB_MAX_RECORD_NAME];
        char path[260];
        char tag[DB_MAX_TAG];
        if (line[0] == '[' || line[0] == '\n' || line[0] == '\r') continue;
        if (sscanf(line, "%d|%d|%95[^|]|%259[^|]|%31[^|]|%d", &id, &type, name, path, tag, &dirty) == 6) {
            if (db->asset_count < DB_MAX_ASSETS) {
                DBAssetRecord *a = &db->assets[db->asset_count++];
                a->id = id;
                a->type = (DBAssetType)type;
                strncpy(a->name, name, DB_MAX_RECORD_NAME - 1);
                a->name[DB_MAX_RECORD_NAME - 1] = '\0';
                strncpy(a->path, path, 259);
                a->path[259] = '\0';
                strncpy(a->tag, tag, DB_MAX_TAG - 1);
                a->tag[DB_MAX_TAG - 1] = '\0';
                a->dirty = dirty ? true : false;
                if (id >= db->next_id) db->next_id = id + 1;
            }
        }
    }
    fclose(f);
    return true;
}
