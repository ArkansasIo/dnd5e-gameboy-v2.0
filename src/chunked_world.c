#include "chunked_world.h"

void chunk_init(Chunk *chunk) {
    grid3d_init(&chunk->grid, CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE, chunk->buffer);
}

void chunked_world_init(ChunkedWorld *world) {
    for (int x = 0; x < WORLD_CHUNKS_X; ++x) {
        for (int y = 0; y < WORLD_CHUNKS_Y; ++y) {
            for (int z = 0; z < WORLD_CHUNKS_Z; ++z) {
                chunk_init(&world->chunks[x][y][z]);
            }
        }
    }
}
