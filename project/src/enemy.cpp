#include "enemy.h"

#include "mcworld.h"

#define TIMER 8

Enemy::Enemy(McWorld *world, const Vector3 &pos)
    : Entity(pos, Vector3(2.5, 3, 2.5)) {
    m_world = world;
    shootTimer = 1;
	yTimer = 8;
initY = pos.y;
}

Enemy::~Enemy()
{

}


void Enemy::tick(float seconds){

    shootTimer -= seconds;
	yTimer -= seconds;
    if (shootTimer <= 0){
        shootTimer = TIMER * .8 + frand() * 3;
        if (m_world->m_player->m_life <= 0) return;

        Vector3 _dir = (m_world->getPlayerPosition() - m_pos);
        _dir.normalize();
        m_world->m_entities.push_back(new Fireball(m_pos + _dir, _dir*10, false, Vector3(0,.5,1)));

    }

    m_pos.y = initY + (2 + 2 * cos((TIMER - yTimer) * M_PI / 4));
}

void Enemy::draw(Graphics *g){
    glPushMatrix();
    g->translate(m_pos);
    glColor3f(.2, .3, .7);
    g->renderCylinder(m_dim.x/2, m_dim.y);

    glPopMatrix();
    glColor3f(1, 1, 1);

}
