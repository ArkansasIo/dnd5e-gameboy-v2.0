#ifndef TILEMAP_H
#define TILEMAP_H

#include "multidim_grid.h"

// Example: 2D tilemap for RPGs
#define TILEMAP_MAX_WIDTH 64
#define TILEMAP_MAX_HEIGHT 64

typedef struct {
    Grid2D grid;
    int buffer[TILEMAP_MAX_WIDTH * TILEMAP_MAX_HEIGHT];
} TileMap;

void tilemap_init(TileMap *tm, uint16_t width, uint16_t height);
int tilemap_get(const TileMap *tm, uint16_t x, uint16_t y);
void tilemap_set(TileMap *tm, uint16_t x, uint16_t y, int value);

#endif // TILEMAP_H
