#ifndef ENEMY_H2
#define ENEMY_H2

#include "entity.h"
#include "vector.h"
#include "fireball.h"

class McWorld;

class Enemy : public Entity
{
public:
    Enemy(McWorld *world, const Vector3 &pos);
    virtual ~Enemy();

    virtual void tick(float seconds);
    virtual void draw(Graphics *g);

	void collideCylinder(const Vector3 &mtv, Entity *other) {
	if (Fireball *f = dynamic_cast<Fireball *>(other)) {
		if (f->m_friendly) { m_purge = true; f->explode(); }
	}
}


    McWorld *m_world;
    float shootTimer, yTimer, initY;
};

#endif // ENEMY_H2
