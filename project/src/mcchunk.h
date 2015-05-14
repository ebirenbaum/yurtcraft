#ifndef MCCHUNK_H
#define MCCHUNK_H

#include "chunk.h"
#include "blocktype.h"

class McChunk : public Chunk {
public:
    McChunk(ChunkFactory *factory, const Vector3 &pos);
    virtual ~McChunk();

protected:
    bool shouldDrawTransparent(Direction dir, const Block &block, Vector3 *modify);
};

#endif // MCCHUNK_H
