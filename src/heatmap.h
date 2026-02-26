#ifndef HEATMAP_H
#define HEATMAP_H

#include "multidim_grid.h"

#define HEATMAP_MAX_WIDTH 64
#define HEATMAP_MAX_HEIGHT 64

typedef struct {
    Grid2D grid;
    int buffer[HEATMAP_MAX_WIDTH * HEATMAP_MAX_HEIGHT];
} HeatMap;

void heatmap_init(HeatMap *hm, uint16_t width, uint16_t height);
int heatmap_get(const HeatMap *hm, uint16_t x, uint16_t y);
void heatmap_set(HeatMap *hm, uint16_t x, uint16_t y, int value);

#endif // HEATMAP_H
