#ifndef PROJECT_IO_H
#define PROJECT_IO_H

#include <stdbool.h>
#include "project_db.h"
#include "map_editor.h"

bool project_io_save(const char *filename, const ProjectDB *db, const TileMap *map);
bool project_io_load(const char *filename, ProjectDB *db);

#endif
