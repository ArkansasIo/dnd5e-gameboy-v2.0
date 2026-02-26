#include "tilemap.h"

void tilemap_init(TileMap *tm, uint16_t width, uint16_t height) {
    grid2d_init(&tm->grid, width, height, tm->buffer);
}

int tilemap_get(const TileMap *tm, uint16_t x, uint16_t y) {
    return grid2d_get(&tm->grid, x, y);
}

void tilemap_set(TileMap *tm, uint16_t x, uint16_t y, int value) {
    grid2d_set(&tm->grid, x, y, value);
}
