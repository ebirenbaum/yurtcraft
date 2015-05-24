#include "enemy.h"

#include "mcworld.h"

#define TIMER 5

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
        m_world->m_entities.push_back(new Fireball(m_pos + _dir, _dir*17, false, Vector3(0,.5,1)));

    }

    m_pos.y = initY + (2 + 2 * cos((TIMER - yTimer) * M_PI / 4));
}

void Enemy::draw(Graphics *g){
    glPushMatrix();
    g->translate(m_pos);
    if (m_world->m_next % 3 == 0) glColor4f(1,0,0,1);
    else if (m_world->m_next % 3 == 2) glColor4f(0,1,0,1);
    else glColor4f(0,0,1,1);
    glEnable(GL_COLOR_MATERIAL);
    g->renderCylinder(m_dim.x/2, m_dim.y);
    glPopMatrix();
    glColor3f(1, 1, 1);

}
