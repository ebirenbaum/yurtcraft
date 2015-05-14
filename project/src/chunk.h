#ifndef CHUNK_H
#define CHUNK_H

#define CHUNKX 32
#define CHUNKY 32
#define CHUNKZ 32

#include "graphics.h"
#include "blockdef.h"
#include "chunkfactory.h"
#include "aab.h"
#include "entity.h"

class Chunk {

public:
    Chunk(ChunkFactory *factory, const Vector3 &pos);
    virtual ~Chunk();

    void draw(Graphics *g);
    void transparentDraw(Graphics *g);

    void resetVbo();

    void collide(float seconds, Entity *e);

    void setBlock(const Vector3 &pos, const BlockDef &blockDef);
    Block getBlock(const Vector3 &pos) const;

    void setTranslatedBlock(const Vector3 &pos, const BlockDef &blockDef);
    Block getTranslatedBlock(const Vector3 &pos) const;

    bool inChunk(const Vector3 &pos) const;
    int getBlockIndex(const Vector3 &pos) const;

    AAB getBoundingBox() const;
    inline Vector3 getPos() const { return m_pos; }

protected:
    Block *_blocks;

    virtual bool shouldDrawTransparent(Direction dir, const Block &block, Vector3 *modify);

private:
    void drawBlock(const Block &block, const Vector3 &pos);
    void drawTransparentBlock(const Block &block, const Vector3 &pos);

    void updateBlockFaces();
    void updateBlockFaces(const Vector3 &pos, int index, bool val);

    void collideX(float seconds, Entity *e);
    void collideY(float seconds, Entity *e);
    void collideZ(float seconds, Entity *e);

    Vector3 m_pos, m_trans;
    int m_xbyz, m_size;

    GLuint m_vboId, m_numVerts;
    GLuint m_tVboId, m_tNumVerts;

    ChunkFactory *m_factory;
};

#endif // CHUNK_H
