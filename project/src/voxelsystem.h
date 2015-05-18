#ifndef VOXELWORLD_H
#define VOXELWORLD_H

#include <map>
#include <queue>

#include "collisions.h"
#include "chunk.h"
#include "blockdef.h"
#include "chunkfactory.h"

struct ChunkGenThreadData {
    bool active;
    bool ready;
    Chunk *chunk;
    Vector3 address;
    ChunkFactory *factory;
};
#define NUM_THREADS 8
#define USE_THREADS false

class VoxelSystem {

public:
    VoxelSystem(/*const string &atlas*/);
    virtual ~VoxelSystem();

    void draw(Graphics *g);
    void collide(float seconds, vector<Entity *> *entities);
    VoxelIntersection raycast(const Ray &ray, vector<Entity *> *entities);

    void setChunkFixation(const Vector3 &pos);
    void setChunkFactory(ChunkFactory *factory);

    Block queryBlock(const Vector3 &pos);
    void setBlock(const Vector3 &pos, char type);

    Chunk *loadChunk(const Vector3 &_address, bool immediate);

    BlockDef getBlockDef(char key) { return m_factory->getBlockDef(key); }

private:
    void collideEntity(float seconds, Entity *e);

//    string m_atlas;
    map<pair<int, pair<int, int> >, Chunk *> m_chunks;
    Vector2 m_xb, m_yb, m_zb;
    ChunkFactory *m_factory;

    queue<Vector3> _incoming, _outgoing;

    pthread_t m_threads[NUM_THREADS];
    struct ChunkGenThreadData m_threadData[NUM_THREADS];
};

#endif // VOXELWORLD_H
