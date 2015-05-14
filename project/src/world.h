#ifndef WORLD_H
#define WORLD_H

#include <vector>

#include "vrcamera.h"
#include "includes.h"
#include "entity.h"

/**
 * @brief The World class represents the entire game world and everything contained by it.
 */
class World {

public:
    World(VRCamera *camera);
    virtual ~World();

    virtual void tick(float seconds);
    virtual void draw(Graphics *g);
    virtual void drawHUD(Graphics *g, const Vector2 &dim) = 0;

    virtual void mousePressed(MouseEvent *event) = 0;
    virtual void mouseReleased(MouseEvent *event) = 0;
    virtual void mouseMoved(const Vector2 &delta) = 0;
    virtual void mouseWheeled(int delta) = 0;

    virtual void keyPressed(const string &key) = 0;
    virtual void keyReleased(const string &key) = 0;

    virtual void resize(float aspectRatio) = 0;

    inline void setGravity(const Vector3 &gravity) { m_gravity = gravity; }

protected:
    vector<Entity *> m_entities;
    VRCamera *m_camera;

private:
    Vector3 m_gravity;
};

#endif // WORLD_H
