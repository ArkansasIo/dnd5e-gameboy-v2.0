#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include <stdbool.h>

#define MAP_MAX_W 128
#define MAP_MAX_H 128

typedef struct {
    int width;
    int height;
    int tiles[MAP_MAX_H][MAP_MAX_W];
    bool dirty;
} TileMap;

TileMap *tilemap_create(int width, int height, int fill_tile);
void tilemap_destroy(TileMap *map);
bool tilemap_set(TileMap *map, int x, int y, int tile_id);
int tilemap_get(const TileMap *map, int x, int y);
bool tilemap_fill_rect(TileMap *map, int x0, int y0, int x1, int y1, int tile_id);
bool tilemap_bucket_fill(TileMap *map, int x, int y, int tile_id);
bool tilemap_save_csv(const TileMap *map, const char *filename);

#endif
