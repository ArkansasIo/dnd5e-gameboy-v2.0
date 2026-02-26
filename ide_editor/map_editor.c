#include "map_editor.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int x;
    int y;
} Point;

static bool in_bounds(const TileMap *map, int x, int y) {
    return map && x >= 0 && y >= 0 && x < map->width && y < map->height;
}

TileMap *tilemap_create(int width, int height, int fill_tile) {
    int x, y;
    TileMap *map;
    if (width <= 0 || height <= 0 || width > MAP_MAX_W || height > MAP_MAX_H) return NULL;
    map = (TileMap *)malloc(sizeof(TileMap));
    if (!map) return NULL;
    map->width = width;
    map->height = height;
    map->dirty = false;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) map->tiles[y][x] = fill_tile;
    }
    return map;
}

void tilemap_destroy(TileMap *map) {
    free(map);
}

bool tilemap_set(TileMap *map, int x, int y, int tile_id) {
    if (!in_bounds(map, x, y)) return false;
    map->tiles[y][x] = tile_id;
    map->dirty = true;
    return true;
}

int tilemap_get(const TileMap *map, int x, int y) {
    if (!in_bounds(map, x, y)) return -1;
    return map->tiles[y][x];
}

bool tilemap_fill_rect(TileMap *map, int x0, int y0, int x1, int y1, int tile_id) {
    int x, y;
    if (!map) return false;
    if (x0 > x1) { int t = x0; x0 = x1; x1 = t; }
    if (y0 > y1) { int t2 = y0; y0 = y1; y1 = t2; }
    if (x1 < 0 || y1 < 0 || x0 >= map->width || y0 >= map->height) return false;
    if (x0 < 0) x0 = 0;
    if (y0 < 0) y0 = 0;
    if (x1 >= map->width) x1 = map->width - 1;
    if (y1 >= map->height) y1 = map->height - 1;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++) map->tiles[y][x] = tile_id;
    }
    map->dirty = true;
    return true;
}

bool tilemap_bucket_fill(TileMap *map, int x, int y, int tile_id) {
    Point queue[MAP_MAX_W * MAP_MAX_H];
    int head = 0, tail = 0;
    int target;
    if (!in_bounds(map, x, y)) return false;
    target = map->tiles[y][x];
    if (target == tile_id) return true;

    queue[tail++] = (Point){x, y};
    while (head < tail) {
        Point p = queue[head++];
        int cx = p.x;
        int cy = p.y;
        if (!in_bounds(map, cx, cy)) continue;
        if (map->tiles[cy][cx] != target) continue;
        map->tiles[cy][cx] = tile_id;

        if (tail < MAP_MAX_W * MAP_MAX_H) {
            queue[tail++] = (Point){cx + 1, cy};
            queue[tail++] = (Point){cx - 1, cy};
            queue[tail++] = (Point){cx, cy + 1};
            queue[tail++] = (Point){cx, cy - 1};
        }
    }
    map->dirty = true;
    return true;
}

bool tilemap_save_csv(const TileMap *map, const char *filename) {
    int x, y;
    FILE *f;
    if (!map || !filename) return false;
    f = fopen(filename, "w");
    if (!f) return false;
    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            fprintf(f, "%d", map->tiles[y][x]);
            if (x < map->width - 1) fputc(',', f);
        }
        fputc('\n', f);
    }
    fclose(f);
    return true;
}
