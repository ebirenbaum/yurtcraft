#include "fireball.h"

Fireball::Fireball(const Vector3 &pos, const Vector3 &vel) : Entity(pos, Vector3(1,1,1), vel), m_life(3)
{
    m_fireball = new ParticleFireball(pos);
    m_explode = false;
    m_gravable = false;
}

Fireball::~Fireball()
{
    delete m_fireball;
}

void Fireball::draw(Graphics *g)
{
    m_fireball->draw(g);
}

void Fireball::tick(float seconds)
{
    m_life -= seconds;
    if (!m_explode) {
        if (m_life <= seconds) {
            explode();
        }
    } else {
        if (m_life <= seconds) {
            m_purge = true;
        }
    }
}

Vector3 Fireball::getColor()
{
    return Vector3(1,.5,.2);
}

void Fireball::collideVoxel(const VoxelCollision &voxel)
{
    explode();
}

void Fireball::collideCylinder(const Vector3 &mtv, Entity *other)
{

}

bool Fireball::isExploding()
{
    return m_explode;
}

void Fireball::explode()
{
    m_explode = true;
    m_life = 1.5;
    m_vel = Vector3();
    m_fireball->explode();
}
