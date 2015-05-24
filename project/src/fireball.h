#ifndef FIREBALL_H
#define FIREBALL_H

#include "particlefireball.h"
#include "entity.h"

class Fireball : public Entity {
public:
    Fireball(const Vector3 &pos, const Vector3 &vel, bool team, const Vector3 &col);
    virtual ~Fireball();

    void draw(Graphics *g);
    void tick(float seconds);

    inline void setLife(float val) { m_life = val; }
    bool isExploding();
    Vector3 getColor();

    virtual void collideCylinder(const Vector3 &mtv, Entity *other);
    virtual void collideVoxel(const VoxelCollision &voxel);

	bool m_friendly;
void explode();

    ParticleFireball *m_fireball;
    float m_life;
    bool m_explode;

    
};

#endif // FIREBALL_H
