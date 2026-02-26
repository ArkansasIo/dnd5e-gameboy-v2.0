#include "multidim_examples.h"

void fill_tilemap(TileMap *tm, int value) {
    for (uint16_t y = 0; y < tm->grid.height; ++y) {
        for (uint16_t x = 0; x < tm->grid.width; ++x) {
            tilemap_set(tm, x, y, value);
        }
    }
}

void set_voxel(ChunkedWorld *world, int cx, int cy, int cz, int x, int y, int z, int value) {
    if (cx < WORLD_CHUNKS_X && cy < WORLD_CHUNKS_Y && cz < WORLD_CHUNKS_Z) {
        Chunk *chunk = &world->chunks[cx][cy][cz];
        grid3d_set(&chunk->grid, x, y, z, value);
    }
}

void fill_uigrid(UIGrid *ui, int value) {
    for (uint16_t row = 0; row < ui->grid.height; ++row) {
        for (uint16_t col = 0; col < ui->grid.width; ++col) {
            uigrid_set(ui, row, col, value);
        }
    }
}

void fill_heatmap_gradient(HeatMap *hm) {
    for (uint16_t y = 0; y < hm->grid.height; ++y) {
        for (uint16_t x = 0; x < hm->grid.width; ++x) {
            heatmap_set(hm, x, y, x + y);
        }
    }
}
