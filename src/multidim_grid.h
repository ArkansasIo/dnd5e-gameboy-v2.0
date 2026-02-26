#ifndef MULTIDIM_GRID_H
#define MULTIDIM_GRID_H

#include <stdint.h>
#include <stdbool.h>

// --- 2D Grid Template ---
typedef struct {
    uint16_t width;
    uint16_t height;
    int *data; // pointer to width*height elements
} Grid2D;

void grid2d_init(Grid2D *grid, uint16_t width, uint16_t height, int *buffer);
int grid2d_get(const Grid2D *grid, uint16_t x, uint16_t y);
void grid2d_set(Grid2D *grid, uint16_t x, uint16_t y, int value);

// --- 3D Grid Template ---
typedef struct {
    uint16_t width, height, depth;
    int *data; // pointer to width*height*depth elements
} Grid3D;

void grid3d_init(Grid3D *grid, uint16_t width, uint16_t height, uint16_t depth, int *buffer);
int grid3d_get(const Grid3D *grid, uint16_t x, uint16_t y, uint16_t z);
void grid3d_set(Grid3D *grid, uint16_t x, uint16_t y, uint16_t z, int value);

// --- 2D+Time (3D) Grid Template ---
typedef struct {
    uint16_t width, height, frames;
    int *data; // pointer to width*height*frames elements
} Grid2DT;

void grid2dt_init(Grid2DT *grid, uint16_t width, uint16_t height, uint16_t frames, int *buffer);
int grid2dt_get(const Grid2DT *grid, uint16_t x, uint16_t y, uint16_t t);
void grid2dt_set(Grid2DT *grid, uint16_t x, uint16_t y, uint16_t t, int value);

#endif // MULTIDIM_GRID_H
