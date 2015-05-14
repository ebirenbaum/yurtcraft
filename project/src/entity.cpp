#include "entity.h"

Entity::Entity(const Vector3 &pos, const Vector3 &dim, const Vector3 &vel) :
    m_pos(pos), m_dim(dim), m_vel(vel), m_acc(Vector3()), m_purge(false), m_gravable(true) {
}

Entity::~Entity() {
}

void Entity::tick(float seconds) {
    translate(m_vel * seconds);

    determineVelocity(seconds);

    m_vel += m_acc * seconds;
    m_acc = Vector3();
}

void Entity::determineVelocity(float seconds) {
}

void Entity::translate(const Vector3 &trans) {
    m_pos += trans;
}

void Entity::accelerate(const Vector3 &acc) {
    m_acc += acc;
}

void Entity::collideCylinder(const Vector3 &mtv, Entity *other) {
    translate(mtv / 2);
}

void Entity::collideVoxel(const VoxelCollision &voxel) {
}
