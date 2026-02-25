#ifndef CHUNKED_WORLD_H
#define CHUNKED_WORLD_H

#include "multidim_grid.h"

#define CHUNK_SIZE 16
#define WORLD_CHUNKS_X 8
#define WORLD_CHUNKS_Y 8
#define WORLD_CHUNKS_Z 4

// 3D Voxel World split into chunks

typedef struct {
    Grid3D grid;
    int buffer[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
} Chunk;

typedef struct {
    Chunk chunks[WORLD_CHUNKS_X][WORLD_CHUNKS_Y][WORLD_CHUNKS_Z];
} ChunkedWorld;

void chunk_init(Chunk *chunk);
void chunked_world_init(ChunkedWorld *world);

#endif // CHUNKED_WORLD_H
