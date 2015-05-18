#ifndef ENEMY_H2
#define ENEMY_H2

#include "entity.h"
#include "vector.h"

class McWorld;

class Enemy : public Entity
{
public:
    Enemy(McWorld *world, const Vector3 &pos);
    virtual ~Enemy();

    virtual void tick(float seconds);
    virtual void draw(Graphics *g);


    McWorld *m_world;
    float shootTimer;
};

#endif // ENEMY_H2
