#ifndef ENTITY_H
#define ENTITY_H

#include "includes.h"
#include <stdio.h>

#include "graphics.h"
#include "cylinder.h"
#include "aab.h"
#include "collisions.h"

using namespace std;

/**
 * @brief The Entity interface defines any game object.
 */
class Entity {

public:
    Entity(const Vector3 &pos, const Vector3 &dim, const Vector3 &vel = Vector3());
    virtual ~Entity();

    virtual void tick(float seconds);
    virtual void draw(Graphics *g) = 0;

    virtual void determineVelocity(float seconds);
    virtual void translate(const Vector3 &trans);
    virtual void accelerate(const Vector3 &acc);

    virtual Cylinder getBoundingCylinder() const { return Cylinder(m_pos, MAX(m_dim.x, m_dim.z) / 2, m_dim.y); }
    inline AAB getBoundingBox() const { Vector3 diff(m_dim.x / 2, 0, m_dim.z / 2);
                                        return AAB(m_pos - diff, m_pos + diff + Vector3(0, m_dim.y, 0)); }

    virtual void collideCylinder(const Vector3 &mtv, Entity *other);
    virtual void collideVoxel(const VoxelCollision &voxel);

	virtual Vector3 getPos() const { return m_pos; }

    inline Vector3 nextTranslation(float seconds) { return m_vel * seconds + m_acc * seconds * seconds; }

    Vector3 m_pos, m_vel, m_acc, m_dim;
    bool m_purge, m_gravable, m_collidesTerrain;
};

#endif // ENTITY_H
