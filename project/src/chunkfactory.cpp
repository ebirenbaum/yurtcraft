#include "chunkfactory.h"
#include "chunk.h"

ChunkFactory::ChunkFactory() {
    m_blockDefs[0] = BlockDef();
}

ChunkFactory::~ChunkFactory() {
}

Chunk *ChunkFactory::getChunk(const Vector3 &pos) {
    Chunk *output = createChunk(pos);
    output->resetVbo();
    return output;
}

void ChunkFactory::registerBlockDefinition(const BlockDef &blockDef) {
    if (blockDef.block.type == 0) return;

    if (m_blockDefs.find(blockDef.block.type) == m_blockDefs.end()) {
        m_blockDefs[blockDef.block.type] = blockDef;
    }
}

BlockDef ChunkFactory::getBlockDef(char key) {
    if (m_blockDefs.find(key) != m_blockDefs.end()) {
        return m_blockDefs[key];
    }
    return BlockDef();
}
