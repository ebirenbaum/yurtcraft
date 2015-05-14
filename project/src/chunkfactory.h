#ifndef CHUNKFACTORY_H
#define CHUNKFACTORY_H

#include "includes.h"
#include <map>
#include "blockdef.h"

class Chunk;

class ChunkFactory {
public:
    ChunkFactory();
    virtual ~ChunkFactory();

    Chunk *getChunk(const Vector3 &pos);
    virtual Chunk *createChunk(const Vector3 &pos) = 0;

    void registerBlockDefinition(const BlockDef &blockDef);
    BlockDef getBlockDef(char key);

protected:
    map<char, BlockDef> m_blockDefs;
};

#endif // CHUNKFACTORY_H
