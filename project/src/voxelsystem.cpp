#include "voxelsystem.h"

#include "entity.h"

VoxelSystem::VoxelSystem(/*const string &atlas*/) : /*m_atlas(atlas), */m_factory(NULL) {
    int n = 4;
    m_xb = Vector2(-n,n);
    m_yb = Vector2(0,n);
    m_zb = Vector2(-n,n);

    for (int i = 0; i < NUM_THREADS; i++){
        m_threadData[i].active = false;
        m_threadData[i].ready = false;
    }
}

VoxelSystem::~VoxelSystem() {
    delete m_factory;
}

void VoxelSystem::draw(Graphics *g) {

    g->setupTexture("atlas");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);


    for (map<pair<int, pair<int, int> >, Chunk *>::iterator it = m_chunks.begin(); it != m_chunks.end(); it++) {
        Chunk *chunk = it->second;
        if (!g->isCulled(chunk->getBoundingBox())) {
            chunk->draw(g);
        }
    }

    glDisableClientState(GL_NORMAL_ARRAY);

    if (!g->debug) {
        g->enableBlend();
        for (map<pair<int, pair<int, int> >, Chunk *>::iterator it = m_chunks.begin(); it != m_chunks.end(); it++) {
            Chunk *chunk = it->second;
            if (!g->isCulled(chunk->getBoundingBox())) {
                chunk->transparentDraw(g);
            }
        }
        g->disableBlend();
    }

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    g->teardownTexture();
}

void VoxelSystem::collide(float seconds, vector<Entity *> *entities) {
    for (int i = 0; i < entities->size(); i++) {
        if (entities->at(i)->m_collidesTerrain){
            collideEntity(seconds, entities->at(i));
        }
    }
}

void VoxelSystem::collideEntity(float seconds, Entity *e) {
    Vector3 currChunk = (e->m_pos / Vector3((float)CHUNKX, (float)CHUNKY, (float)CHUNKZ)).floor();

    if (m_chunks.find(currChunk.toPair()) != m_chunks.end()) {
        Chunk *chunk = m_chunks[currChunk.toPair()];
        chunk->collide(seconds, e);
    } else {
        //m_chunks[currChunk.toPair()] = m_factory->getChunk(currChunk);
    }

    Vector3 nextTranslation = e->nextTranslation(seconds),
            nextPos = e->m_pos + nextTranslation,
            nextChunk = (nextPos / Vector3((float)CHUNKX, (float)CHUNKY, (float)CHUNKZ)).floor();
    if (m_chunks.find(nextChunk.toPair()) != m_chunks.end()) {
        Chunk *chunk = m_chunks[nextChunk.toPair()];
        chunk->collide(seconds, e);
    } else {
        //m_chunks[nextChunk.toPair()] = m_factory->getChunk(nextChunk);
    }

    AAB entityAAB = e->getBoundingBox();
    entityAAB.translate(nextTranslation);
    for (int i = 0; i < 27; i++) {
        if (i != 13) {
            Vector3 diff = Vector3((i / 3) % 3 - 1, i / 9 - 1, i % 3 - 1),
                    temp = nextChunk + diff;

            if (m_chunks.find(temp.toPair()) != m_chunks.end()) {
                Chunk *chunk = m_chunks[temp.toPair()];
                if (chunk->getBoundingBox().collides(entityAAB)) {
                    chunk->collide(seconds, e);
                }
            } else {
                //m_chunks[temp.toPair()] = m_factory->getChunk(temp);
            }
        }
    }
}

VoxelIntersection VoxelSystem::raycast(const Ray &ray, vector<Entity *> *entities) {
    const int stepX = (ray.d.x > 0 ? 1 : (ray.d.x < 0 ? -1 : 0)),
            stepY = (ray.d.y > 0 ? 1 : (ray.d.y < 0 ? -1 : 0)),
            stepZ = (ray.d.z > 0 ? 1 : (ray.d.z < 0 ? -1 : 0));

    const float tDeltaX = fabs(1.0 / ray.d.x),
            tDeltaY = fabs(1.0 / ray.d.y),
            tDeltaZ = fabs(1.0 / ray.d.z);

    int capX = (stepX >= 0 ? floor(ray.p.x) : ceil(ray.p.x)),
            capY = (stepY >= 0 ? floor(ray.p.y) : ceil(ray.p.y)),
            capZ = (stepZ >= 0 ? floor(ray.p.z) : ceil(ray.p.z));

    float tMaxX = ((double)(capX + stepX) - ray.p.x) / ray.d.x,
            tMaxY = ((double)(capY + stepY) - ray.p.y) / ray.d.y,
            tMaxZ = ((double)(capZ + stepZ) - ray.p.z) / ray.d.z;

    int lastStep = -1;

    while (true) {
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                capX += stepX;
                tMaxX += tDeltaX;
                lastStep = 0;
            } else {
                capZ += stepZ;
                tMaxZ += tDeltaZ;
                lastStep = 2;
            }
        } else {
            if (tMaxY < tMaxZ) {
                capY += stepY;
                tMaxY += tDeltaY;
                lastStep = 1;
            } else {
                capZ += stepZ;
                tMaxZ += tDeltaZ;
                lastStep = 2;
            }
        }

        Vector3 next = Vector3(capX + (stepX >= 0 ? 0 : -1),
                               capY + (stepY >= 0 ? 0 : -1),
                               capZ + (stepZ >= 0 ? 0 : -1));
        Vector3 nextChunk = (next / Vector3(CHUNKX, CHUNKY, CHUNKZ)).floor();
        if (m_chunks.find(nextChunk.toPair()) == m_chunks.end()) {
            return VoxelIntersection();
        } else {
            Chunk *chunk = m_chunks[nextChunk.toPair()];
            Block block = chunk->getTranslatedBlock(next);
            if (!block.isPassable()) {
                Vector3 extra = (lastStep == 0 ? Vector3(-stepX, 0, 0)
                                               : (lastStep == 1 ? Vector3(0, -stepY, 0) : Vector3(0, 0, -stepZ)));
                //                if (_selected.valid && (_selected.p != next || _selected.n != extra)) {
                //                    _heldTime = 0;
                //                }
                return VoxelIntersection(lastStep == 0 ? tMaxX - tDeltaX : (lastStep == 1 ? tMaxY - tDeltaY : tMaxZ - tDeltaZ),
                                         block, next, extra);
                //                _selected = Intersection(lastStep == 0 ? tMaxX - tDeltaX : (lastStep == 1 ? tMaxY - tDeltaY : tMaxZ - tDeltaZ),
                //                                    next, extra, true, (int)block);
                //                _mob = Intersection();
                //                _mob.t = 999999;
                //                _killIt = NULL;
            }
        }
    }

    //    foreach (Entity *et, _entities) {
    //        MinecraftEntity *e = (MinecraftEntity *)et;
    //        if (e != _player) {
    //            Intersection check = e->raycast(ray);
    //            if (check.valid && check.t < _selected.t && check.t < _mob.t) {
    //                _mob = check;
    //                _killIt = e;
    //            }
    //        }
    //    }

    //    if (_mob.valid) _selected = Intersection();
}

void *generateChunkInThread(void *threadDataThing){
    struct ChunkGenThreadData *data = (struct ChunkGenThreadData *) threadDataThing;

    data->chunk = data->factory->createChunk(data->address);
    data->ready = true;
    pthread_exit(NULL);
}

void VoxelSystem::setChunkFixation(const Vector3 &pos) {
    Vector3 currChunk = (pos / Vector3(CHUNKX, CHUNKY, CHUNKZ)).floor();

    int count = 1;
    while (!_incoming.empty()) {
        Vector3 next = _incoming.front();
        _incoming.pop();
        if (m_chunks.find(next.toPair()) != m_chunks.end()) continue;
        if (USE_THREADS){
            int _useableThread = -1;
            for (int i = 0; i < NUM_THREADS; i++){
                if (!m_threadData[i].active){
                    _useableThread = i;
                    break;
                }
            }
            if (_useableThread == -1){
                _incoming.push(next);
                break;
            }
            m_threadData[_useableThread].address = next;
            m_threadData[_useableThread].factory = m_factory;
            m_threadData[_useableThread].active = true;
            m_threadData[_useableThread].ready = false;
            pthread_create(&m_threads[_useableThread], NULL, generateChunkInThread, (void *)&(m_threadData[_useableThread]));
        } else {
            m_chunks[next.toPair()] = m_factory->getChunk(next);
        }
        if (--count == 0) break;
    }

    if (USE_THREADS){
        for (int i = 0; i < NUM_THREADS; i++){
            if (m_threadData[i].ready){
                m_threadData[i].chunk->resetVbo();
                m_chunks[m_threadData[i].address.toPair()] = m_threadData[i].chunk;
                m_threadData[i].ready = false;
                m_threadData[i].active = false;
                break; // only one per frame
            }
        }
    }

    count = 2;
    while (!_outgoing.empty()) {
        Vector3 next = _outgoing.front();
        _outgoing.pop();
        if (m_chunks.find(next.toPair()) != m_chunks.end()); {
            Chunk *c = m_chunks[next.toPair()];
            m_chunks.erase(next.toPair());
            delete c;
            if (--count == 0) break;
        }
    }

    // Check for new chunks to stream
    int xCo = (m_xb.x + m_xb.y) / 2;
    if (currChunk.x == xCo + 1) {
        for (int i = m_yb.x; i <= m_yb.y; i++) {
            for (int j = m_zb.x; j <= m_zb.y; j++) {
                _incoming.push(Vector3(m_xb.y + 1, i, j));
                _outgoing.push(Vector3(m_xb.x, i, j));
            }
        }
        m_xb += Vector2(1,1);
    } else if (currChunk.x == xCo - 1) {
        for (int i = m_yb.x; i <= m_yb.y; i++) {
            for (int j = m_zb.x; j <= m_zb.y; j++) {
                _incoming.push(Vector3(m_xb.x - 1, i, j));
                _outgoing.push(Vector3(m_xb.y, i, j));
            }
        }
        m_xb -= Vector2(1,1);
    }

    int zCo = (m_zb.x + m_zb.y) / 2;
    if (currChunk.z == zCo + 1) {
        for (int i = m_yb.x; i <= m_yb.y; i++) {
            for (int j = m_xb.x; j <= m_xb.y; j++) {
                _incoming.push(Vector3(j, i, m_zb.y + 1));
                _outgoing.push(Vector3(j, i, m_zb.x));
            }
        }
        m_zb += Vector2(1,1);
    } else if (currChunk.z == zCo - 1) {
        for (int i = m_yb.x; i <= m_yb.y; i++) {
            for (int j = m_xb.x; j <= m_xb.y; j++) {
                _incoming.push(Vector3(j, i, m_zb.x - 1));
                _outgoing.push(Vector3(j, i, m_zb.y));
            }
        }
        m_zb -= Vector2(1,1);
    }

    int yCo = (m_yb.x + m_yb.y) / 2;
    if (currChunk.y == yCo + 1) {
        for (int i = m_xb.x; i <= m_xb.y; i++) {
            for (int j = m_zb.x; j <= m_zb.y; j++) {
                _incoming.push(Vector3(i, m_yb.y + 1, j));
                _outgoing.push(Vector3(i, m_yb.x, j));
            }
        }
        m_yb += Vector2(1,1);
    } else if (currChunk.y == yCo - 1) {
        for (int i = m_xb.x; i <= m_xb.y; i++) {
            for (int j = m_zb.x; j <= m_zb.y; j++) {
                _incoming.push(Vector3(i, m_yb.x - 1, j));
                _outgoing.push(Vector3(i, m_yb.y, j));
            }
        }
        m_yb -= Vector2(1,1);
    }
}

void VoxelSystem::setChunkFactory(ChunkFactory *factory) {
    m_factory = factory;

    cout << "Building initial chunks..." << endl;

    for (int i = m_xb.x; i <= m_xb.y; i++) {
        for (int j = m_yb.x; j <= m_yb.y; j++) {
            for (int k = m_zb.x; k <= m_zb.y; k++) {
                Vector3 next(i,j,k);
                m_chunks[next.toPair()] = m_factory->getChunk(next);
            }
        }
    }
}

Block VoxelSystem::queryBlock(const Vector3 &pos) {
    Vector3 chunk = (pos / Vector3((float)CHUNKX, (float)CHUNKY, (float)CHUNKZ)).floor();
    if (m_chunks.find(chunk.toPair()) != m_chunks.end()) {
        return m_chunks[chunk.toPair()]->getTranslatedBlock(pos.floor());
    }
    return Block();
}

void VoxelSystem::setBlock(const Vector3 &pos, char type) {
    Vector3 chunkPos = (pos / Vector3((float)CHUNKX, (float)CHUNKY, (float)CHUNKZ)).floor();
    if (m_chunks.find(chunkPos.toPair()) != m_chunks.end()) {
        Chunk *chunk = m_chunks[chunkPos.toPair()];
        chunk->setTranslatedBlock(pos, m_factory->getBlockDef(type));
        chunk->resetVbo();
    }
}
