#ifndef MULTIDIM_EXAMPLES_H
#define MULTIDIM_EXAMPLES_H

#include "tilemap.h"
#include "chunked_world.h"
#include "ui_grid.h"
#include "heatmap.h"

// Example: Fill a tilemap with a value
void fill_tilemap(TileMap *tm, int value);

// Example: Set a voxel in a chunked world
void set_voxel(ChunkedWorld *world, int cx, int cy, int cz, int x, int y, int z, int value);

// Example: Fill a UI grid with a value
void fill_uigrid(UIGrid *ui, int value);

// Example: Fill a heatmap with a gradient
void fill_heatmap_gradient(HeatMap *hm);

#endif // MULTIDIM_EXAMPLES_H
