#ifndef MCCHUNKFACTORY_H
#define MCCHUNKFACTORY_H

#include <set>

#include "includes.h"
#include "mcchunk.h"
#include "noise.h"
#include "blocktype.h"

class BiomeManager;
class BiomeTile;
class BiomeGenInfo;

class McChunkFactory : public ChunkFactory {
public:
    McChunkFactory(int seed = 0);
    virtual ~McChunkFactory();

    Chunk *createChunk(const Vector3 &pos);

    BiomeGenInfo* getBiomeInfo(const Vector3 &pos);

private:
    void initSurface(const Vector3 &chunkPos, Chunk *chunk);
    void initUnderground(const Vector3 &chunkPos, Chunk *chunk);

    void initTunnels(const Vector3 &pos, Chunk *chunk, const Vector4 &hub);
    void initTunnel(const Vector3 &start, const Vector3 &end, float thickness, Chunk *chunk);

    void initOres(const Vector3 &chunkPos, Chunk *chunk);
    void initOreVeins(BlockType type, float veinDelta, float oreDelta, Chunk *chunk);
    void initOreVein(BlockType block, float delta, Chunk *chunk);

    Noise *m_noise;
    int m_seed;

    bool m_terrain, m_trees, m_caves;
    set<BlockType> m_excludeCaves, m_replaceWithOres;

    BiomeManager *bm;
    map<pair<float, float>, BiomeTile*> biomeTiles;
};

#endif // MCCHUNKFACTORY_H
