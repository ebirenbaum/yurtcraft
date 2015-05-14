#include "world.h"

World::World(VRCamera *camera) : m_camera(camera), m_gravity(Vector3()) {
}

World::~World() {
    for (int i = m_entities.size() - 1; i > 0; i--) {
        Entity *e = m_entities.at(i);
        delete e;
        m_entities.pop_back();
    }
}

void World::tick(float seconds) {
    for (int i = m_entities.size() - 1; i >= 0; i--) {
        Entity *e = m_entities.at(i);
        if (e->m_purge) {
            m_entities.erase(m_entities.begin() + i);
            delete e;
        }
    }

    for (int i = 0; i < m_entities.size(); i++) {
        m_entities.at(i)->tick(seconds);
    }

    if (m_gravity != Vector3()) {
        for (int i = 0; i < m_entities.size(); i++) {
            m_entities.at(i)->accelerate(m_gravity);
        }
    }
}

void World::draw(Graphics *g) {
    g->updateCamera();

    for (int i = 0; i < m_entities.size(); i++) {
        m_entities.at(i)->draw(g);
    }
}
