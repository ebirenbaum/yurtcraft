#include "mcchunkfactory.h"

McChunkFactory::McChunkFactory(int seed) : ChunkFactory(), m_noise(new Noise(seed)), m_seed(seed) {

    // stone
    registerBlockDefinition(BlockDef(Block(STONE, false, false),
                                     Vector4(.0625, 1 - .9375, .125, 1 - 1)));

    // dirt
    registerBlockDefinition(BlockDef(Block(DIRT, false, false),
                                     Vector4(.125, 1 - .9375, .1875, 1 - 1)));

    // grass
    registerBlockDefinition(BlockDef(Block(GRASS, false, false),
                                     Vector4(0, 1 - .9375, .0625, 1 - 1),
                                     Vector4(.125, 1 - .9375, .1875, 1 - 1),
                                     Vector4(.1875, 1 - .9375, .25, 1 - 1)));

    // snowgrass
    registerBlockDefinition(BlockDef(Block(SNOWGRASS, false, false),
                                     Vector4(.125, 1 - .6875, .1875, 1 - .75),
                                     Vector4(.125, 1 - .9375, .1875, 1 - 1),
                                     Vector4(.25, 1 - .6875, .3125, 1 - .75)));

    // wood
    registerBlockDefinition(BlockDef(Block(WOOD, false, false),
                                     Vector4(.3125, 1 - .875, .375, 1 - .9375),
                                     Vector4(.3125, 1 - .875, .375, 1 - .9375),
                                     Vector4(.25, 1 - .875, .3125, 1 - .9375)));

    // leaves
    registerBlockDefinition(BlockDef(Block(LEAVES, true, false),
                                    Vector4(.25, 1 - .75, .3125, 1 - .8125)));

    registerBlockDefinition(BlockDef(Block(PLANK, false, false),
                                     Vector4(.25, 1 - .9375, .3125, 1 - 1)));

    registerBlockDefinition(BlockDef(Block(BRICK, false, false),
                                     Vector4(.4375, 1 - .9375, .5, 1 - 1)));

//    // cactus
//    registerBlockDefinition(BlockDef(Block(CACTUS, true, false),
//                                     Vector4(.3125, .6875, .375, .75),
//                                     Vector4(.4375, .6875, .5, .75),
//                                     Vector4(.375, .6875, .4375, .75)));

    // sand
    registerBlockDefinition(BlockDef(Block(SAND, false, false),
                                     Vector4(.125, 1 - .875, .1875, 1 - .9375)));

    // glass
    registerBlockDefinition(BlockDef(Block(GLASS, true, false),
                                     Vector4(.0625, 1 - .75, .125, 1 - .8125)));

    // water
    registerBlockDefinition(BlockDef(Block(WATER, true, true),
                                     Vector4(.9375, 1 - .125, 1, 1 - .1875)));

    // coal
    registerBlockDefinition(BlockDef(Block(COAL, false, false),
                                     Vector4(.125, 1 - .8125, .1875, 1 - .875)));

    // iron
    registerBlockDefinition(BlockDef(Block(IRON, false, false),
                                     Vector4(.0625, 1 - .8125, .125, 1 - .875)));

    // gold
    registerBlockDefinition(BlockDef(Block(GOLD, false, false),
                                     Vector4(0, 1 - .8125, .0625, 1 - .875)));

    // redstone
    registerBlockDefinition(BlockDef(Block(REDSTONE, false, false),
                                     Vector4(.1875, 1 - .75, .25, 1 - .8125)));

    // diamond
    registerBlockDefinition(BlockDef(Block(DIAMOND, false, false),
                                     Vector4(.125, 1 - .75, .1875, 1 - .8125)));

    m_terrain = true;
    m_trees = true;
    m_caves = true;

    m_excludeCaves.insert(AIR);
    m_excludeCaves.insert(SAND);
    m_excludeCaves.insert(WATER);

    m_replaceWithOres.insert(DIRT);
    m_replaceWithOres.insert(STONE);
}

McChunkFactory::~McChunkFactory() {
    delete m_noise;
}

Chunk *McChunkFactory::createChunk(const Vector3 &pos) {
    McChunk *chunk = new McChunk(this, pos);

    if (pos.y >= -1) initSurface(pos, chunk);
    else initUnderground(pos, chunk);

    return chunk;
}

void McChunkFactory::initSurface(const Vector3 &chunkPos, Chunk *chunk) {
    srand(chunkPos.hashcode(m_seed));
    Vector4 hub = Vector4(frand() * CHUNKX, frand() * CHUNKY, frand() * CHUNKZ, 0).floor();
    hub.w = frand() - .5;

    if (m_terrain) {
        float noiseValues[CHUNKX][CHUNKZ];

        Vector2 translation = Vector2(chunkPos.x * CHUNKX, chunkPos.z * CHUNKZ);
        float yTranslation = chunkPos.y * CHUNKY;

        for (int x = 0; x < CHUNKX; x++) {
            for (int z = 0; z < CHUNKZ; z++) {
                float value = m_noise->perlin2(Vector2(x, z) + translation, 53.f);
                float scale = cos((x + translation.x) / 128.f) * .15f + .25f;
                noiseValues[x][z] = (value - .2f) / (scale) - .9f;
            }
        }

        for (int y = 0; y < CHUNKY; y++) {
            for (int x = 0; x < CHUNKX; x++) {
                for (int z = 0; z < CHUNKZ; z++) {
                    float originalHeight = noiseValues[x][z] * CHUNKY;
                    float height = originalHeight - yTranslation;
                    BlockType topType = originalHeight > 3 ? GRASS : SAND;
                    BlockType middleType = originalHeight > 3 ? DIRT : SAND;
                    Vector3 pos(x,y,z);

                    if (originalHeight < 0 && y + yTranslation < 0) {
                        chunk->setBlock(pos, m_blockDefs[WATER]);
                    }

                    if (y < height - 5) {
                        chunk->setBlock(pos, m_blockDefs[STONE]);
                    } else if (y < height - 1) {
                        chunk->setBlock(pos, m_blockDefs[middleType]);
                    } else if (y < height) {
                        BlockType type = topType;
                        if (originalHeight > 72) {
                            type = SNOWGRASS;
                        } else if (originalHeight > 66) {
                            float check = frand() * (originalHeight - 66);
                            if (check > 1) type = SNOWGRASS;
                        }
                        chunk->setBlock(pos, m_blockDefs[type]);
                    }
                }
            }
        }

        if (m_trees) {
            int numTrees = 2 + 4 * frand();
            for (int i = 0; i < numTrees; i++) {
                Vector2 tree(floor(frand() * (CHUNKX - 2) + 1), floor(frand() * (CHUNKZ - 2) + 1));
                int base = noiseValues[(int)tree.x][(int)tree.y] * CHUNKY + 1 - chunkPos.y * CHUNKY;
                if (!INRANGE(base, 0, CHUNKY) || chunk->getPos().y < 0) continue;

                int treeHeight = 3 + 2 * frand();
                Block baseBlock = chunk->getBlock(Vector3(tree.x, base - 1, tree.y));
                BlockType treeType = WOOD;

                if (!(baseBlock.type == GRASS || baseBlock.type == SNOWGRASS)) continue;

                for (int h = 0; h < treeHeight; h++) {
                    chunk->setBlock(Vector3(tree.x, base + h, tree.y), m_blockDefs[treeType]);

                    if (treeType == WOOD && h >= 1) {
                        chunk->setBlock(Vector3(tree.x + 1, base + h, tree.y), m_blockDefs[LEAVES]);
                        chunk->setBlock(Vector3(tree.x - 1, base + h, tree.y), m_blockDefs[LEAVES]);
                        chunk->setBlock(Vector3(tree.x, base + h, tree.y + 1), m_blockDefs[LEAVES]);
                        chunk->setBlock(Vector3(tree.x, base + h, tree.y - 1), m_blockDefs[LEAVES]);
                    }
                }
                if (treeType == WOOD)
                    chunk->setBlock(Vector3(tree.x, base + treeHeight, tree.y), m_blockDefs[LEAVES]);
            }
        }
    }

    initOres(chunkPos, chunk);

    if (m_caves) {
        initTunnels(chunkPos, chunk, hub);
    }
}

void McChunkFactory::initUnderground(const Vector3 &chunkPos, Chunk *chunk) {
    for (int y = 0; y < CHUNKY; y++) {
        for (int x = 0; x < CHUNKX; x++) {
            for (int z = 0; z < CHUNKZ; z++) {
                Vector3 pos(x,y,z);
                chunk->setBlock(pos, m_blockDefs[STONE]);
            }
        }
    }

    if (m_caves) {
        srand(chunkPos.hashcode(m_seed));
        Vector4 hub = Vector4(frand() * CHUNKX, frand() * CHUNKY, frand() * CHUNKZ, 0).floor();
        hub.w = frand() - .5;

        initTunnels(chunkPos, chunk, hub);
    }

    initOres(chunkPos, chunk);
}

void McChunkFactory::initTunnels(const Vector3 &chunkPos, Chunk *chunk, const Vector4 &hub) {
    int mult = -1;
    for (int i = 0; i < 6; i++) {
        Vector4 diff = Vector4((i < 2 ? mult : 0), (i >= 2 && i < 4 ? mult : 0), (i >= 4 ? mult : 0), 0),
                neighbor = Vector4(chunkPos, 0) + diff;

        srand(neighbor.toVector3().hashcode(m_seed));
        Vector4 neighborHub = Vector4(frand() * CHUNKX, frand() * CHUNKY, frand() * CHUNKZ, 0).floor()
                + diff * Vector4(CHUNKX, CHUNKY, CHUNKZ, 0);
        neighborHub.w = frand() - .5;

        if (hub.w * neighborHub.w > 0) {
            initTunnel(hub.toVector3(), neighborHub.toVector3(), 2.3f, chunk);
        }
        mult *= -1;
    }
}

void McChunkFactory::initTunnel(const Vector3 &start, const Vector3 &end, float thickness, Chunk *chunk) {
    Vector3 min = Vector3::min(start, end);
    Vector3 max = Vector3::max(start, end);
    float thickness2 = thickness * thickness;

    Vector3 p;
    Ray line(start, end - start);
    for (p.x = min.x; p.x <= max.x; p.x++) {
        if (!INRANGE(p.x, 0, CHUNKX)) continue;
        for (p.y = min.y; p.y <= max.y; p.y++) {
            if (!INRANGE(p.y, 0, CHUNKY)) continue;
            for (p.z = min.z; p.z <= max.z; p.z++) {
                if (!INRANGE(p.z, 0, CHUNKZ)) continue;
                if (line.distanceSquared(p) < thickness2) {
                    Vector3 choice = p;
                    int oldY = choice.y;

//                    float tempY = (choice.y - min.y) / (max.y - min.y);
//                    int newY = floor(tempY * tempY * (max.y - min.y) + min.y);
//                    choice.y = oldY;

                    Block bl = chunk->getBlock(choice);
                    if (m_excludeCaves.find((BlockType) bl.type) == m_excludeCaves.end()) {
                        chunk->setBlock(choice, m_blockDefs[AIR]);
                    }
                }
            }
        }
    }
}

void McChunkFactory::initOres(const Vector3 &chunkPos, Chunk *chunk) {
    initOreVeins(COAL, (chunkPos.y >= 0 ? .2f : .07f), .07f, chunk);
    initOreVeins(IRON, (chunkPos.y >= 0 ? .3f : .15f), .12f, chunk);

    if (chunkPos.y < 0) initOreVeins(REDSTONE, .2f, .16f, chunk);
    if (chunkPos.y < -1) initOreVeins(GOLD, .3f, .25f, chunk);
    if (chunkPos.y < -2) initOreVeins(DIAMOND, .45f, .3f, chunk);
}

void McChunkFactory::initOreVeins(BlockType type, float veinDelta, float oreDelta, Chunk *chunk) {
    float veinProbability = 2.f - veinDelta;
    while (veinProbability > frand()) {
        initOreVein(type, oreDelta, chunk);
        veinProbability -= veinDelta;
    }
}

void McChunkFactory::initOreVein(BlockType block, float delta, Chunk *chunk) {
    Vector3 start = Vector3((int)(frand() * CHUNKX), (int)(frand() * CHUNKY), (int)(frand() * CHUNKZ)),
            currentPos = start;
    float endProbability = 2.f - delta;

    while (endProbability > frand()) {
        if (!chunk->inChunk(currentPos)) return;

        if (m_replaceWithOres.find((BlockType) chunk->getBlock(currentPos).type) != m_replaceWithOres.end()) {
            chunk->setBlock(currentPos, m_blockDefs[block]);
        }

        int dir = (int)(frand() * 6.f);
        switch (dir) {
        case 0: currentPos.x++; break;
        case 1: currentPos.x--; break;
        case 2: currentPos.y++; break;
        case 3: currentPos.y--; break;
        case 4: currentPos.z++; break;
        default: currentPos.z--; break;
        }

        endProbability -= delta;
    }
}
