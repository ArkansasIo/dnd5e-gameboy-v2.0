#include "ui_grid.h"

void uigrid_init(UIGrid *ui, uint16_t rows, uint16_t cols) {
    grid2d_init(&ui->grid, cols, rows, ui->buffer);
}

int uigrid_get(const UIGrid *ui, uint16_t row, uint16_t col) {
    return grid2d_get(&ui->grid, col, row);
}

void uigrid_set(UIGrid *ui, uint16_t row, uint16_t col, int value) {
    grid2d_set(&ui->grid, col, row, value);
}
