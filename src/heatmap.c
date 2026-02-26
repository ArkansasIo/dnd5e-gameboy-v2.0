#include "heatmap.h"

void heatmap_init(HeatMap *hm, uint16_t width, uint16_t height) {
    grid2d_init(&hm->grid, width, height, hm->buffer);
}

int heatmap_get(const HeatMap *hm, uint16_t x, uint16_t y) {
    return grid2d_get(&hm->grid, x, y);
}

void heatmap_set(HeatMap *hm, uint16_t x, uint16_t y, int value) {
    grid2d_set(&hm->grid, x, y, value);
}
