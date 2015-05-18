#ifndef PLAYER_H
#define PLAYER_H

#include "vrcamera.h"
#include "entity.h"
#include "voxelsystem.h"
#include "mcchunkfactory.h"

#define HEIGHT 1.6
#define WIDTH .8

/**
 * @brief The Player class represents the player entity, and contains the camera information
 */
class Player : public Entity {

public:
    Player(const Vector3 &pos, VoxelSystem *system, VrCamera *cam);
    virtual ~Player();

    void tick(float seconds);
    void draw(Graphics *g);
    void drawHUD(Graphics *g, const Vector2 &dim);

    void determineVelocity(float seconds);
    void resetJump();

    void collideCylinder(const Vector3 &mtv, Entity *other);
    void collideVoxel(const VoxelCollision &voxel);

    void mousePressed(MouseEvent *event);
    void mouseReleased(MouseEvent *event);
    void mouseMoved(const Vector2 &delta);
    void mouseWheeled(int delta);

    void keyPressed(const string &key);
    void keyReleased(const string &key);

void wandButtonPressed(WandButton button);
	void wandButtonReleased(WandButton button);

    void resize(float aspectRatio);

    Vector3 getHorizontalLook();
    Ray getRay();

    int m_life, m_block;

protected:

    VrCamera *m_camera;
    set<string> m_heldKeys;

    bool m_canJump;
    float m_speed;

    VoxelSystem *m_voxels;
};

#endif // PLAYER_H
