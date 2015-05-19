#ifndef MINECRAFTWORLD_H
#define MINECRAFTWORLD_H

#define DAYLENGTH 5
#define REMOVETIME .5f

#include "world.h"
#include "skybox.h"
#include "mcchunkfactory.h"
#include "player.h"
#include "voxelsystem.h"
#include "vrdata.h"

class Enemy;

class McWorld : public World {

public:
    McWorld(int seed, VrCamera *cam, VrData *data);
    virtual ~McWorld();

    void tick(float seconds);
    void spawnEnemies();
    void draw(Graphics *g);
    void drawHUD(Graphics *g, const Vector2 &dim);

void resolveCollisions();

    void adjustLighting();


    bool isGameOver() {
        if (m_player->m_life < 0) { cout << "Score: " << m_next << endl; }
        return m_player->m_life < 0;
    }

    void keyPressed(const string &key);
    void keyReleased(const string &key);

    void mousePressed(MouseEvent *event);
    void mouseReleased(MouseEvent *event);
    void mouseMoved(const Vector2 &delta);
    void mouseWheeled(int delta);

    void wandButtonPressed(WandButton button);
	void wandButtonReleased(WandButton button);

    void resize(float aspectRatio);

    Vector3 getPlayerPosition();

private:
    Skybox m_skybox;
    Player *m_player;

    VoxelSystem *m_system;
    VoxelIntersection m_selected;

    VrData *m_data;

    float m_time, m_timeRemove;
    bool m_removeHeld;

    float m_spawnTimer, m_hit;
	int m_num, m_next;
};

#endif // MINECRAFTWORLD_H
