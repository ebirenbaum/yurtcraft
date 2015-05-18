#include "enemy.h"

#include "mcworld.h"
#include "fireball.h"

Enemy::Enemy(McWorld *world, const Vector3 &pos)
    : Entity(pos, Vector3(1, 3, 1)) {
    m_world = world;
    shootTimer = 1;
}

Enemy::~Enemy()
{

}


void Enemy::tick(float seconds){

    shootTimer -= seconds;
    if (shootTimer <= 0){
        shootTimer = 4;

        Vector3 _dir = (m_world->getPlayerPosition() - m_pos);
        _dir.normalize();
        m_world->m_entities.push_back(new Fireball(m_pos + _dir, _dir*10));

    }
}

void Enemy::draw(Graphics *g){
    glPushMatrix();
    g->translate(m_pos);
    glColor3f(.2, .3, .7);
    g->renderCylinder(m_dim.x/2, m_dim.y);

    glPopMatrix();
    glColor3f(1, 1, 1);

}
