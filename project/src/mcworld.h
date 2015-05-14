#ifndef MINECRAFTWORLD_H
#define MINECRAFTWORLD_H

#define DAYLENGTH 5
#define REMOVETIME .5f

#include "world.h"
#include "skybox.h"
#include "mcchunkfactory.h"
#include "player.h"
#include "voxelsystem.h"

class McWorld : public World {

public:
    McWorld(int seed, VRCamera *cam);
    virtual ~McWorld();

    void tick(float seconds);
    void draw(Graphics *g);
    void drawHUD(Graphics *g, const Vector2 &dim);

    void adjustLighting();

    bool isGameOver() { return m_player->m_life < 0; }

    void mousePressed(MouseEvent *event);
    void mouseReleased(MouseEvent *event);
    void mouseMoved(const Vector2 &delta);
    void mouseWheeled(int delta);

    void keyPressed(const string &key);
    void keyReleased(const string &key);

    void resize(float aspectRatio);

    Vector3 getPlayerPosition();

private:
    Skybox m_skybox;
    Player *m_player;

    VoxelSystem *m_system;
    VoxelIntersection m_selected;

    float m_time;

    bool m_removeHeld;
    float m_timeRemove;
};

#endif // MINECRAFTWORLD_H
