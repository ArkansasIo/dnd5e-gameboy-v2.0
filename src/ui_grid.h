#ifndef UI_GRID_H
#define UI_GRID_H

#include "multidim_grid.h"

#define UI_GRID_MAX_ROWS 16
#define UI_GRID_MAX_COLS 32

typedef struct {
    Grid2D grid;
    int buffer[UI_GRID_MAX_ROWS * UI_GRID_MAX_COLS];
} UIGrid;

void uigrid_init(UIGrid *ui, uint16_t rows, uint16_t cols);
int uigrid_get(const UIGrid *ui, uint16_t row, uint16_t col);
void uigrid_set(UIGrid *ui, uint16_t row, uint16_t col, int value);

#endif // UI_GRID_H
