#include "chunk.h"

Chunk::Chunk(ChunkFactory *factory, const Vector3 &pos) :
    m_factory(factory), m_pos(pos), m_trans(m_pos * Vector3(CHUNKX, CHUNKY, CHUNKZ)),
    m_xbyz(CHUNKZ * CHUNKX), m_size(m_xbyz * CHUNKY),
    m_vboId(-1), m_numVerts(0),
    m_tVboId(-1), m_tNumVerts(0) {

    _blocks = new Block[m_size];

    updateBlockFaces();
}

Chunk::~Chunk() {
    delete[] _blocks;
    glDeleteBuffers(1, &m_vboId);
    glDeleteBuffers(1, &m_tVboId);
}

void Chunk::draw(Graphics *g) {
    glPushMatrix();
    g->translate(m_trans);

    unsigned int stride = sizeof(float) * (3 + 2 + 3);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

    glVertexPointer(3, GL_FLOAT, stride, (char *) 0);
    glTexCoordPointer(2, GL_FLOAT, stride, (char *) (3 * sizeof(float)));
    glNormalPointer(GL_FLOAT, stride, (char *) (5 * sizeof(float)));
    glDrawArrays(GL_QUADS, 0, m_numVerts);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPopMatrix();
}

void Chunk::transparentDraw(Graphics *g) {
    //glColor4f(1,1,1,1);
    glPushMatrix();
    g->translate(m_trans);

    unsigned int stride = sizeof(float) * (3 + 2);

    glBindBuffer(GL_ARRAY_BUFFER, m_tVboId);

    glVertexPointer(3, GL_FLOAT, stride, (char *) 0);
    glTexCoordPointer(2, GL_FLOAT, stride, (char *) (3 * sizeof(float)));
    glDrawArrays(GL_QUADS, 0, m_tNumVerts);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPopMatrix();
}

void Chunk::drawBlock(const Block &block, const Vector3 &pos) {
    BlockDef def = m_factory->getBlockDef(block.type);
    Vector3 pos2 = pos + Vector3(1,1,1);

    if (block.shouldDrawTop()) {
        Vector4 coords = def.textureCoords[TOP];
        glNormal3f(0,1,0);
        glTexCoord2f(coords.x, coords.y); glVertex3f(pos.x,pos2.y,pos.z);
        glTexCoord2f(coords.x, coords.w); glVertex3f(pos.x,pos2.y,pos2.z);
        glTexCoord2f(coords.z, coords.w); glVertex3f(pos2.x,pos2.y,pos2.z);
        glTexCoord2f(coords.z, coords.y); glVertex3f(pos2.x,pos2.y,pos.z);
    }

    if (block.shouldDrawBottom()) {
        Vector4 coords = def.textureCoords[BOTTOM];
        glNormal3f(0,-1,0);
        glTexCoord2f(coords.x, coords.y); glVertex3f(pos.x,pos.y,pos.z);
        glTexCoord2f(coords.z, coords.y); glVertex3f(pos2.x,pos.y,pos.z);
        glTexCoord2f(coords.z, coords.w); glVertex3f(pos2.x,pos.y,pos2.z);
        glTexCoord2f(coords.x, coords.w); glVertex3f(pos.x,pos.y,pos2.z);
    }

    if (block.shouldDrawFront()) {
        glNormal3f(-1,0,0);
        Vector4 coords = def.textureCoords[LEFT];
        glTexCoord2f(coords.x, coords.y); glVertex3f(pos.x,pos.y,pos.z);
        glTexCoord2f(coords.z, coords.y); glVertex3f(pos.x,pos.y,pos2.z);
        glTexCoord2f(coords.z, coords.w); glVertex3f(pos.x,pos2.y,pos2.z);
        glTexCoord2f(coords.x, coords.w); glVertex3f(pos.x,pos2.y,pos.z);
    }

    if (block.shouldDrawBack()) {
        Vector4 coords = def.textureCoords[RIGHT];
        glNormal3f(1,0,0);
        glTexCoord2f(coords.x, coords.y); glVertex3f(pos2.x,pos.y,pos2.z);
        glTexCoord2f(coords.z, coords.y); glVertex3f(pos2.x,pos.y,pos.z);
        glTexCoord2f(coords.z, coords.w); glVertex3f(pos2.x,pos2.y,pos.z);
        glTexCoord2f(coords.x, coords.w); glVertex3f(pos2.x,pos2.y,pos2.z);
    }

    if (block.shouldDrawLeft()) {
        Vector4 coords = def.textureCoords[FRONT];
        glNormal3f(0,0,-1);
        glTexCoord2f(coords.x, coords.y); glVertex3f(pos2.x,pos.y,pos.z);
        glTexCoord2f(coords.z, coords.y); glVertex3f(pos.x,pos.y,pos.z);
        glTexCoord2f(coords.z, coords.w); glVertex3f(pos.x,pos2.y,pos.z);
        glTexCoord2f(coords.x, coords.w); glVertex3f(pos2.x,pos2.y,pos.z);
    }

    if (block.shouldDrawRight()) {
        Vector4 coords = def.textureCoords[BACK];
        glNormal3f(0,0,1);
        glTexCoord2f(coords.x, coords.y); glVertex3f(pos.x,pos.y,pos2.z);
        glTexCoord2f(coords.z, coords.y); glVertex3f(pos2.x,pos.y,pos2.z);
        glTexCoord2f(coords.z, coords.w); glVertex3f(pos2.x,pos2.y,pos2.z);
        glTexCoord2f(coords.x, coords.w); glVertex3f(pos.x,pos2.y,pos2.z);
    }
}

void Chunk::drawTransparentBlock(const Block &block, const Vector3 &pos) {
    BlockDef def = m_factory->getBlockDef(block.type);
    Vector3 pos2 = pos + Vector3(1,1,1);

    bool shouldRender = inChunk(pos + Vector3(0,1,0));
    Block up = getBlock(pos + Vector3(0,1,0));
    if (shouldRender && up.type == 0) {
        Vector4 coords = def.textureCoords[TOP];
        glNormal3f(0,1,0);
        glTexCoord2f(coords.x, coords.y); glVertex3f(pos.x,pos2.y,pos.z);
        glTexCoord2f(coords.x, coords.w); glVertex3f(pos.x,pos2.y,pos2.z);
        glTexCoord2f(coords.z, coords.w); glVertex3f(pos2.x,pos2.y,pos2.z);
        glTexCoord2f(coords.z, coords.y); glVertex3f(pos2.x,pos2.y,pos.z);
    }
}

void Chunk::updateBlockFaces() {
    for (int y = 0; y < CHUNKY; y++) {
        for (int x = 0; x < CHUNKX; x++) {
            for (int z = 0; z < CHUNKZ; z++) {
                Vector3 pos(x,y,z);
                int index = getBlockIndex(pos);
                updateBlockFaces(pos, index, _blocks[index].isTransparent());
            }
        }
    }
}

void Chunk::updateBlockFaces(const Vector3 &pos, int index, bool val) {
    if (pos.y < CHUNKY - 1) { _blocks[index + m_xbyz].setDrawBottom(val); }
    if (pos.y > 0) { _blocks[index - m_xbyz].setDrawTop(val); }
    if (pos.x < CHUNKX - 1) { _blocks[index + CHUNKZ].setDrawFront(val); }
    if (pos.x > 0) { _blocks[index - CHUNKZ].setDrawBack(val); }
    if (pos.z < CHUNKZ - 1) { _blocks[index + 1].setDrawLeft(val); }
    if (pos.z > 0) { _blocks[index - 1].setDrawRight(val); }
}

void Chunk::resetVbo() {
    GLuint buff[2];
    if (m_vboId == -1) {
        glGenBuffers(1, buff);
        m_vboId = buff[0];
    }
    float *data = new float[m_size * 8 * 6 * 4];
    float *ptr = data;
    unsigned int faces = 0;

    if (m_tVboId == -1) glGenBuffers(1, &m_tVboId);
    float *tData = new float[m_size * 5 /* * 6 */ * 4];
    float *tPtr = tData;
    unsigned int tFaces = 0;

    unsigned int count = 0;
    for (int y = 0; y < CHUNKY; y++) {
        for (int x = 0; x < CHUNKX; x++) {
            for (int z = 0; z < CHUNKZ; z++) {
                Block block = _blocks[count];

                BlockDef def = m_factory->getBlockDef(block.type);
                int x1 = x+1, y1 = y+1, z1 = z+1;
                bool trans = block.isTransparent();

                if (!trans) {
                    if (block.shouldDrawTop()) {
                        Vector4 coords = def.textureCoords[TOP];
                        ptr[0] = x; ptr[1] = y1; ptr[2] = z; ptr[3] = coords.x; ptr[4] = coords.y;
                        ptr[5] = 0; ptr[6] = 1; ptr[7] = 0; ptr += 8;

                        ptr[0] = x; ptr[1] = y1; ptr[2] = z1; ptr[3] = coords.x; ptr[4] = coords.w;
                        ptr[5] = 0; ptr[6] = 1; ptr[7] = 0; ptr += 8;

                        ptr[0] = x1; ptr[1] = y1; ptr[2] = z1; ptr[3] = coords.z; ptr[4] = coords.w;
                        ptr[5] = 0; ptr[6] = 1; ptr[7] = 0; ptr += 8;

                        ptr[0] = x1; ptr[1] = y1; ptr[2] = z; ptr[3] = coords.z; ptr[4] = coords.y;
                        ptr[5] = 0; ptr[6] = 1; ptr[7] = 0; ptr += 8;
                        faces += 1;
                    }

                    if (block.shouldDrawBottom()) {
                        Vector4 coords = def.textureCoords[BOTTOM];
                        ptr[0] = x; ptr[1] = y; ptr[2] = z; ptr[3] = coords.x; ptr[4] = coords.y;
                        ptr[5] = 0; ptr[6] = -1; ptr[7] = 0; ptr += 8;

                        ptr[0] = x1; ptr[1] = y; ptr[2] = z; ptr[3] = coords.z; ptr[4] = coords.y;
                        ptr[5] = 0; ptr[6] = -1; ptr[7] = 0; ptr += 8;

                        ptr[0] = x1; ptr[1] = y; ptr[2] = z1; ptr[3] = coords.z; ptr[4] = coords.w;
                        ptr[5] = 0; ptr[6] = -1; ptr[7] = 0; ptr += 8;

                        ptr[0] = x; ptr[1] = y; ptr[2] = z1; ptr[3] = coords.x; ptr[4] = coords.w;
                        ptr[5] = 0; ptr[6] = -1; ptr[7] = 0; ptr += 8;
                        faces += 1;
                    }

                    if (block.shouldDrawLeft()) {
                        Vector4 coords = def.textureCoords[LEFT];
                        ptr[0] = x1; ptr[1] = y; ptr[2] = z; ptr[3] = coords.x; ptr[4] = coords.y;
                        ptr[5] = 0; ptr[6] = 0; ptr[7] = -1; ptr += 8;

                        ptr[0] = x; ptr[1] = y; ptr[2] = z; ptr[3] = coords.z; ptr[4] = coords.y;
                        ptr[5] = 0; ptr[6] = 0; ptr[7] = -1; ptr += 8;

                        ptr[0] = x; ptr[1] = y1; ptr[2] = z; ptr[3] = coords.z; ptr[4] = coords.w;
                        ptr[5] = 0; ptr[6] = 0; ptr[7] = -1; ptr += 8;

                        ptr[0] = x1; ptr[1] = y1; ptr[2] = z; ptr[3] = coords.x; ptr[4] = coords.w;
                        ptr[5] = 0; ptr[6] = 0; ptr[7] = -1; ptr += 8;
                        faces += 1;
                    }

                    if (block.shouldDrawRight()) {
                        Vector4 coords = def.textureCoords[RIGHT];
                        ptr[0] = x; ptr[1] = y; ptr[2] = z1; ptr[3] = coords.x; ptr[4] = coords.y;
                        ptr[5] = 0; ptr[6] = 0; ptr[7] = 1; ptr += 8;

                        ptr[0] = x1; ptr[1] = y; ptr[2] = z1; ptr[3] = coords.z; ptr[4] = coords.y;
                        ptr[5] = 0; ptr[6] = 0; ptr[7] = 1; ptr += 8;

                        ptr[0] = x1; ptr[1] = y1; ptr[2] = z1; ptr[3] = coords.z; ptr[4] = coords.w;
                        ptr[5] = 0; ptr[6] = 0; ptr[7] = 1; ptr += 8;

                        ptr[0] = x; ptr[1] = y1; ptr[2] = z1; ptr[3] = coords.x; ptr[4] = coords.w;
                        ptr[5] = 0; ptr[6] = 0; ptr[7] = 1; ptr += 8;
                        faces += 1;
                    }

                    if (block.shouldDrawFront()) {
                        Vector4 coords = def.textureCoords[FRONT];
                        ptr[0] = x; ptr[1] = y; ptr[2] = z; ptr[3] = coords.x; ptr[4] = coords.y;
                        ptr[5] = -1; ptr[6] = 0; ptr[7] = 0; ptr += 8;

                        ptr[0] = x; ptr[1] = y; ptr[2] = z1; ptr[3] = coords.z; ptr[4] = coords.y;
                        ptr[5] = -1; ptr[6] = 0; ptr[7] = 0; ptr += 8;

                        ptr[0] = x; ptr[1] = y1; ptr[2] = z1; ptr[3] = coords.z; ptr[4] = coords.w;
                        ptr[5] = -1; ptr[6] = 0; ptr[7] = 0; ptr += 8;

                        ptr[0] = x; ptr[1] = y1; ptr[2] = z; ptr[3] = coords.x; ptr[4] = coords.w;
                        ptr[5] = -1; ptr[6] = 0; ptr[7] = 0; ptr += 8;
                        faces += 1;
                    }

                    if (block.shouldDrawBack()) {
                        Vector4 coords = def.textureCoords[BACK];
                        ptr[0] = x1; ptr[1] = y; ptr[2] = z1; ptr[3] = coords.x; ptr[4] = coords.y;
                        ptr[5] = 1; ptr[6] = 0; ptr[7] = 0; ptr += 8;

                        ptr[0] = x1; ptr[1] = y; ptr[2] = z; ptr[3] = coords.z; ptr[4] = coords.y;
                        ptr[5] = 1; ptr[6] = 0; ptr[7] = 0; ptr += 8;

                        ptr[0] = x1; ptr[1] = y1; ptr[2] = z; ptr[3] = coords.z; ptr[4] = coords.w;
                        ptr[5] = 1; ptr[6] = 0; ptr[7] = 0; ptr += 8;

                        ptr[0] = x1; ptr[1] = y1; ptr[2] = z1; ptr[3] = coords.x; ptr[4] = coords.w;
                        ptr[5] = 1; ptr[6] = 0; ptr[7] = 0; ptr += 8;
                        faces += 1;
                    }
                } else if (block.type != 0) {
                    for (int i = 0; i < 6; i++) {
                        Vector3 extra = Vector3();
                        Vector3 derp = Vector3(x,y,z);

                        switch ((Direction) i) {
                        case TOP:
                            extra.y = 1;
                            break;
                        case BOTTOM:
                            extra.y = -1;
                            break;
                        case LEFT:
                            extra.z = -1;
                            break;
                        case RIGHT:
                            extra.z = 1;
                            break;
                        case FRONT:
                            extra.x = -1;
                            break;
                        case BACK:
                            extra.x = 1;
                            break;
                        default:
                            break;
                        }

                        Vector3 toCheck = derp + extra;

                        bool shouldRender = shouldDrawTransparent((Direction) i, block, &toCheck);

                        if (shouldRender) {
                            if ((Direction) i == TOP) {
                                Vector4 coords = def.textureCoords[TOP];
                                tPtr[0] = x; tPtr[1] = y1; tPtr[2] = z; tPtr[3] = coords.x; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x; tPtr[1] = y1; tPtr[2] = z1; tPtr[3] = coords.x; tPtr[4] = coords.w;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y1; tPtr[2] = z1; tPtr[3] = coords.z; tPtr[4] = coords.w;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y1; tPtr[2] = z; tPtr[3] = coords.z; tPtr[4] = coords.y;
                                tPtr += 5;
                                tFaces += 1;
                            }

                            else if ((Direction) i == BOTTOM) {
                                Vector4 coords = def.textureCoords[BOTTOM];
                                tPtr[0] = x; tPtr[1] = y; tPtr[2] = z; tPtr[3] = coords.x; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y; tPtr[2] = z; tPtr[3] = coords.z; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y; tPtr[2] = z1; tPtr[3] = coords.z; tPtr[4] = coords.w;
                                tPtr += 5;

                                tPtr[0] = x; tPtr[1] = y; tPtr[2] = z1; tPtr[3] = coords.x; tPtr[4] = coords.w;
                                tPtr += 5;
                                tFaces += 1;
                            }

                            else if ((Direction) i == LEFT) {
                                Vector4 coords = def.textureCoords[LEFT];
                                tPtr[0] = x1; tPtr[1] = y; tPtr[2] = z; tPtr[3] = coords.x; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x; tPtr[1] = y; tPtr[2] = z; tPtr[3] = coords.z; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x; tPtr[1] = y1; tPtr[2] = z; tPtr[3] = coords.z; tPtr[4] = coords.w;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y1; tPtr[2] = z; tPtr[3] = coords.x; tPtr[4] = coords.w;
                                tPtr += 5;
                                tFaces += 1;
                            }

                            else if ((Direction) i == RIGHT) {
                                Vector4 coords = def.textureCoords[RIGHT];
                                tPtr[0] = x; tPtr[1] = y; tPtr[2] = z1; tPtr[3] = coords.x; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y; tPtr[2] = z1; tPtr[3] = coords.z; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y1; tPtr[2] = z1; tPtr[3] = coords.z; tPtr[4] = coords.w;
                                tPtr += 5;

                                tPtr[0] = x; tPtr[1] = y1; tPtr[2] = z1; tPtr[3] = coords.x; tPtr[4] = coords.w;
                                tPtr += 5;
                                tFaces += 1;
                            }

                            else if ((Direction) i == FRONT) {
                                Vector4 coords = def.textureCoords[FRONT];
                                tPtr[0] = x; tPtr[1] = y; tPtr[2] = z; tPtr[3] = coords.x; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x; tPtr[1] = y; tPtr[2] = z1; tPtr[3] = coords.z; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x; tPtr[1] = y1; tPtr[2] = z1; tPtr[3] = coords.z; tPtr[4] = coords.w;
                                tPtr += 5;

                                tPtr[0] = x; tPtr[1] = y1; tPtr[2] = z; tPtr[3] = coords.x; tPtr[4] = coords.w;
                                tPtr += 5;
                                tFaces += 1;
                            }

                            else if ((Direction) i == BACK) {
                                Vector4 coords = def.textureCoords[BACK];
                                tPtr[0] = x1; tPtr[1] = y; tPtr[2] = z1; tPtr[3] = coords.x; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y; tPtr[2] = z; tPtr[3] = coords.z; tPtr[4] = coords.y;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y1; tPtr[2] = z; tPtr[3] = coords.z; tPtr[4] = coords.w;
                                tPtr += 5;

                                tPtr[0] = x1; tPtr[1] = y1; tPtr[2] = z1; tPtr[3] = coords.x; tPtr[4] = coords.w;
                                tPtr += 5;
                                tFaces += 1;
                            }
                        }
                    }
                }
                count++;
            }
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * faces * 8 * 4, (GLvoid *)data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_numVerts = faces * 4;

    delete[] data;

    glBindBuffer(GL_ARRAY_BUFFER, m_tVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tFaces * 5 * 4, (GLvoid *)tData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_tNumVerts = tFaces * 4;

    delete[] tData;
}

bool Chunk::shouldDrawTransparent(Direction dir, const Block &block, Vector3 *modify) {
    return false;
}

void Chunk::collide(float seconds, Entity *e) {
    collideY(seconds, e);
    collideX(seconds, e);
    collideZ(seconds, e);
}

void Chunk::collideX(float seconds, Entity *e) {
    if (e->m_vel.x == 0) return;

    bool positive = e->m_vel.x > 0; int delta = positive ? 1 : -1;
    AAB entityAAB = e->getBoundingBox(); entityAAB.translate(-m_trans);

    int x1 = positive ? ceil(entityAAB.m_posi.x) : floor(entityAAB.m_negi.x);
    Vector3 translation = Vector3(e->nextTranslation(seconds).x, 0, 0);
    entityAAB.translate(translation);

    Vector3 c000 = entityAAB.m_negi.floor(), c111 = entityAAB.m_posi.floor();
    for (int x = x1; (positive ? x <= c111.x : x >= c000.x); x += delta) {
        for (int y = c000.y; y <= c111.y; y++) {
            for (int z = c000.z; z <= c111.z; z++) {
                Vector3 cell(x, y, z);
                if (inChunk(cell)) {
                    Block bl = getBlock(cell);
                    if (!bl.isPassable()) {
                        e->m_vel.x = 0;
                        e->m_pos.x = cell.x + m_trans.x +
                                (positive ? -e->m_dim.x/2.f - EPS : 1.f + e->m_dim.x/2.f + EPS);
                        e->collideVoxel(VoxelCollision(bl, Vector3(-delta, 0, 0)));
                        return;
                    }
                }
            }
        }
    }
}

void Chunk::collideY(float seconds, Entity *e) {
    if (e->m_vel.y == 0) return;

    bool positive = e->m_vel.y > 0; int delta = positive ? 1 : -1;
    AAB entityAAB = e->getBoundingBox(); entityAAB.translate(-m_trans);

    int y1 = positive ? ceil(entityAAB.m_posi.y) : floor(entityAAB.m_negi.y);
    Vector3 translation = Vector3(0, e->nextTranslation(seconds).y, 0);
    entityAAB.translate(translation);

    Vector3 c000 = entityAAB.m_negi.floor(), c111 = entityAAB.m_posi.floor();
    for (int y = y1; (positive ? y <= c111.y : y >= c000.y); y += delta) {
        for (int x = c000.x; x <= c111.x; x++) {
            for (int z = c000.z; z <= c111.z; z++) {
                Vector3 cell(x, y, z);
                if (inChunk(cell)) {
                    Block bl = getBlock(cell);
                    if (!bl.isPassable()) {
                        e->m_vel.y = 0;
                        e->m_pos.y = cell.y + m_trans.y +
                                (positive ? -e->m_dim.y - EPS : 1 + EPS);
                        e->collideVoxel(VoxelCollision(bl, Vector3(0, -delta, 0)));
                        return;
                    }
                }
            }
        }
    }
}

void Chunk::collideZ(float seconds, Entity *e) {
    if (e->m_vel.z == 0) return;

    bool positive = e->m_vel.z > 0; int delta = positive ? 1 : -1;
    AAB entityAAB = e->getBoundingBox(); entityAAB.translate(-m_trans);

    int z1 = positive ? ceil(entityAAB.m_posi.z) : floor(entityAAB.m_negi.z);
    Vector3 translation = Vector3(0, 0, e->nextTranslation(seconds).z);
    entityAAB.translate(translation);

    Vector3 c000 = entityAAB.m_negi.floor(), c111 = entityAAB.m_posi.floor();
    for (int z = z1; (positive ? z <= c111.z : z >= c000.z); z += delta) {
        for (int y = c000.y; y <= c111.y; y++) {
            for (int x = c000.x; x <= c111.x; x++) {
                Vector3 cell(x, y, z);
                if (inChunk(cell)) {
                    Block bl = getBlock(cell);
                    if (!bl.isPassable()) {
                        e->m_vel.z = 0;
                        e->m_pos.z = cell.z + m_trans.z +
                                (positive ? -e->m_dim.z/2.f - EPS : 1.f + e->m_dim.z/2.f + EPS);
                        e->collideVoxel(VoxelCollision(bl, Vector3(0, 0, -delta)));
                        return;
                    }
                }
            }
        }
    }
}

void Chunk::setBlock(const Vector3 &pos, const BlockDef &blockDef) {
    if (!inChunk(pos)) return;

    int index = getBlockIndex(pos);
    _blocks[index].type = blockDef.block.type;
    _blocks[index].setTransparent(blockDef.block.isTransparent());
    _blocks[index].setPassable(blockDef.block.isPassable());

    updateBlockFaces(pos, index, blockDef.block.isTransparent());
}

Block Chunk::getBlock(const Vector3 &pos) const {
    return _blocks[getBlockIndex(pos)];
}

void Chunk::setTranslatedBlock(const Vector3 &pos, const BlockDef &blockDef) {
    setBlock(pos - m_trans, blockDef);
}

Block Chunk::getTranslatedBlock(const Vector3 &pos) const {
    return getBlock(pos - m_trans);
}

bool Chunk::inChunk(const Vector3 &pos) const {
    return INRANGE(pos.x, 0, CHUNKX-1) && INRANGE(pos.y, 0, CHUNKY-1) && INRANGE(pos.z, 0, CHUNKZ-1);
}

int Chunk::getBlockIndex(const Vector3 &pos) const {
    int output = pos.z + CHUNKZ * pos.x + m_xbyz * pos.y;
    return output >= m_size ? 0 : output;
}

AAB Chunk::getBoundingBox() const {
    return AAB(m_trans, m_trans + Vector3(CHUNKX, CHUNKY, CHUNKZ));
}
