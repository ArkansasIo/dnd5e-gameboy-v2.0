#include "multidim_grid.h"

// --- 2D Grid Implementation ---
void grid2d_init(Grid2D *grid, uint16_t width, uint16_t height, int *buffer) {
    grid->width = width;
    grid->height = height;
    grid->data = buffer;
}

int grid2d_get(const Grid2D *grid, uint16_t x, uint16_t y) {
    return grid->data[y * grid->width + x];
}

void grid2d_set(Grid2D *grid, uint16_t x, uint16_t y, int value) {
    grid->data[y * grid->width + x] = value;
}

// --- 3D Grid Implementation ---
void grid3d_init(Grid3D *grid, uint16_t width, uint16_t height, uint16_t depth, int *buffer) {
    grid->width = width;
    grid->height = height;
    grid->depth = depth;
    grid->data = buffer;
}

int grid3d_get(const Grid3D *grid, uint16_t x, uint16_t y, uint16_t z) {
    return grid->data[(z * grid->height * grid->width) + (y * grid->width) + x];
}

void grid3d_set(Grid3D *grid, uint16_t x, uint16_t y, uint16_t z, int value) {
    grid->data[(z * grid->height * grid->width) + (y * grid->width) + x] = value;
}

// --- 2D+Time (3D) Grid Implementation ---
void grid2dt_init(Grid2DT *grid, uint16_t width, uint16_t height, uint16_t frames, int *buffer) {
    grid->width = width;
    grid->height = height;
    grid->frames = frames;
    grid->data = buffer;
}

int grid2dt_get(const Grid2DT *grid, uint16_t x, uint16_t y, uint16_t t) {
    return grid->data[(t * grid->height * grid->width) + (y * grid->width) + x];
}

void grid2dt_set(Grid2DT *grid, uint16_t x, uint16_t y, uint16_t t, int value) {
    grid->data[(t * grid->height * grid->width) + (y * grid->width) + x] = value;
}
