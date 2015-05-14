#include "mcchunk.h"

McChunk::McChunk(ChunkFactory *factory, const Vector3 &pos) : Chunk(factory, pos) {
}

McChunk::~McChunk() {
}

bool McChunk::shouldDrawTransparent(Direction dir, const Block &block, Vector3 *modify) {
    if ((BlockType) block.type == WATER) {
        Block up = getBlock(*modify + Vector3(0,1,0));
        return dir == TOP && (!inChunk(*modify) || up.type == 0);
    } else if ((BlockType) block.type == LEAVES) {
        return true;
    } else if ((BlockType) block.type == GLASS) {
        return true;
    }
    return false;
}
