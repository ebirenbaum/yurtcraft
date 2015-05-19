#include "mcworld.h"
#include "fireball.h"
#include "monorail.h"
#include "enemy.h"

McWorld::McWorld(int seed, VrCamera *cam, VrData *data)
    : World(cam), m_data(data), m_time(0), m_removeHeld(false), m_timeRemove(0) {
    m_system = new VoxelSystem(/*"atlas"*/);

    McChunkFactory *factory = new McChunkFactory(seed);
    m_system->setChunkFactory(factory);

    m_player = new Player(Vector3(-5,50,0), m_system, m_camera, data);
    m_entities.push_back(m_player);

    m_hit = 1;
    m_num = 0;
    m_next = 1;

    m_spawnTimer = 15;
    //Monorail *monorail = new Monorail(m_system, factory, m_player, m_player->m_pos);
    //m_entities.push_back(monorail);

    //setGravity(Vector3(0,-15,0));
}

McWorld::~McWorld() {
    delete m_system;
}

void McWorld::tick(float seconds) {
    m_system->setChunkFixation(m_player->m_pos);
    m_system->collide(seconds, &m_entities);

    World::tick(seconds);

    m_time += seconds;
    if (m_time > DAYLENGTH) {
        m_time = -DAYLENGTH / 2.f;
    }
    m_time = DAYLENGTH / 2.f;

    VoxelIntersection oldSelectedBlock = m_selected;
    m_selected = m_system->raycast(m_player->getRay(), &m_entities);

    if (m_removeHeld && oldSelectedBlock.p == m_selected.p) {
        m_timeRemove += seconds;
        if (m_timeRemove >= REMOVETIME) {
            m_system->setBlock(m_selected.p, (char) AIR);
            m_timeRemove = 0;
        }
    } else {
        m_timeRemove = 0;
    }

    m_spawnTimer -= seconds;
    m_hit += seconds; if (m_hit > 1) { m_hit = 1; }
    spawnEnemies();
    resolveCollisions();

    if (isGameOver()) {
        m_hit = 0;
    }
}

void McWorld::spawnEnemies(){

    if (m_spawnTimer <= 0 && m_num < m_next){
        m_spawnTimer = frand()*10+15 * (1. / m_next);

        Vector3 newPos = m_player->m_pos;
        Vector3 delta = Vector3::randomDirection()*30;
        delta.x = fabs(delta.x);
        delta.x += 20;
        delta.y = fabs(delta.y);

        newPos += delta;

        for (int ty = 0; ty < 100; ty++){
            bool passable = m_system->queryBlock(Vector3(newPos.x, newPos.y+ty, newPos.z)).isPassable();
            if (passable){
                newPos.y += ty + 2;
                break;
            }
        }

        Enemy *newEnemy = new Enemy(this, newPos);
        m_entities.push_back(newEnemy);
        m_num++;
    }

    if (m_num == m_next && m_entities.size() == 1) {
        m_next++;
        m_num = 0;
        m_spawnTimer = 0;
    }
}

void McWorld::resolveCollisions() {
    for (int i = 0; i < m_entities.size(); i++) {
        Entity *e1 = m_entities[i];
        for (int j = i + 1; j < m_entities.size(); j++) {
            Entity *e2 = m_entities.at(j);
            Vector3 overlap = e1->getBoundingCylinder().collide(e2->getBoundingCylinder());
            if (overlap != Vector3()) {
                e1->collideCylinder(overlap, e2);
                e2->collideCylinder(-overlap, e1);
                //cout << "HIT something" << endl;

                if (Player *p = dynamic_cast<Player *>(e1)) {
                    //cout << "HIT" << endl;
                    m_hit = 0;
                }
                if (Player *p = dynamic_cast<Player *>(e2)) {
                    m_hit = 0;
                    //cout << "HIT" << endl;
                }
            }
        }
    }
}

void McWorld::draw(Graphics *g) {
    adjustLighting();
    glColor3f(1, m_hit, m_hit);
    glPushMatrix();
    g->translate(m_player->m_pos);
    m_skybox.draw(g);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glColor3f(1, m_hit, m_hit);
    m_system->draw(g);

    glColor3f(1, m_hit, m_hit);
    World::draw(g);

    if (m_selected.t != -1) {
        glColor3f(1,1,0);
        glPushMatrix();
        g->translate(m_selected.p + Vector3(.5f,.5f,.5f));
        glScalef(1.05, 1.05, 1.05);
        g->renderCubeFrame();
        glBegin(GL_LINES);
        glVertex3f(0,0,0);
        glVertex3f(m_selected.n.x, m_selected.n.y, m_selected.n.z);
        glEnd();
        glPopMatrix();

        if (m_removeHeld) {
            g->enableBlend();
            glColor4f(1,1,1,1);

            glPushMatrix();
            g->translate(m_selected.p + Vector3(.5f,.5f,.5f));
            glScalef(1.01, 1.01, 1.01);

            int key = (int) ((m_timeRemove / REMOVETIME) * 10);
            g->renderTexturedCube(appendNum("break", key));

            glPopMatrix();

            g->disableBlend();
        }
    }
}

void McWorld::drawHUD(Graphics *g, const Vector2 &dim) {
    m_player->drawHUD(g, dim);
}

void McWorld::adjustLighting() {
    float period = (m_time / DAYLENGTH) * M_PI, sinValue = sin(period), cosValue = cos(period);

    if (m_time > 0) {
        float ambient = sinValue * .8f;
        GLfloat ambientLight[] = { ambient, ambient, ambient, ambient };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

        float diffuse = sinValue * .6f;
        GLfloat diffuseLight[] = { diffuse, diffuse, diffuse, diffuse };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

        GLfloat position[] = { cosValue, sinValue, cosValue, 0.f };
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glEnable(GL_LIGHT0);

    } else {
        glDisable(GL_LIGHT0);
    }
}

void McWorld::keyPressed(const string &key) {
    m_player->keyPressed(key);
}

void McWorld::keyReleased(const string &key) {
    m_player->keyReleased(key);
}

void McWorld::mousePressed(MouseEvent *event) {
    m_player->mousePressed(event);

    if (event->button == MOUSE_RIGHT) {
        Vector3 toAdd = m_selected.p + m_selected.n;
        m_system->setBlock(toAdd, m_player->m_block);
    }

    if (event->button == MOUSE_LEFT) {
        Vector3 base = m_camera->getEye() + m_camera->getLook(),
                dir = m_camera->getLook() * 20;
        m_entities.push_back(new Fireball(base, dir, true, Vector3(1,1,0)));
    }
}

void McWorld::mouseReleased(MouseEvent *event) {
    m_player->mouseReleased(event);
}

void McWorld::mouseMoved(const Vector2 &delta) {
    m_player->mouseMoved(delta);
}

void McWorld::mouseWheeled(int delta) {
    m_player->mouseWheeled(delta);
}

Vector3 McWorld::getPlayerPosition() {
    return m_player->getPos();
}

void McWorld::resize(float aspectRatio) {
    m_player->resize(aspectRatio);
}

void McWorld::wandButtonPressed(WandButton button)
{
    if (isGameOver()) {
        return;
    }
    m_player->wandButtonPressed(button);

    Vector3 base = m_player->getPos() + Vector3(0,.6,0) + m_data->getWandDir() * 4,
            dir = m_data->getWandDir() * 20;
    m_entities.push_back(new Fireball(base, dir, true, Vector3(1,1,0)));

}

void McWorld::wandButtonReleased(WandButton button)
{
    m_player->wandButtonReleased(button);
}

void McWorld::tryRestart()
{
    if (!isGameOver()) {
        return;
    }

    cout << "Score: " << m_next << endl;

    m_hit = 1;
    m_num = 0;
    m_next = 1;

    m_spawnTimer = 15;

    m_entities.clear();
    m_entities.push_back(m_player);
}

