#ifndef PLAYER_H
#define PLAYER_H

#include "vrcamera.h"
#include "entity.h"
#include "voxelsystem.h"
#include "mcchunkfactory.h"
#include "vrdata.h"

#define HEIGHT 1.6
#define WIDTH .8

/**
 * @brief The Player class represents the player entity, and contains the camera information
 */
class Player : public Entity {

public:
    Player(const Vector3 &pos, VoxelSystem *system, VrCamera *cam, VrData *data);
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

Cylinder getBoundingCylinder() const { Vector3 p = getPos(); return Cylinder(p, MAX(m_dim.x, m_dim.z) / 2, m_dim.y); }

    Vector3 getHorizontalLook();
    Ray getRay();

    int m_life, m_block;

	Vector3 getPos() const { Vector3 vec = m_data->getHeadPos(); vec.y = HEIGHT / 2.; return m_pos + vec; }

protected:

    VrCamera *m_camera;
    set<string> m_heldKeys;

    bool m_canJump;
    float m_speed;

    VoxelSystem *m_voxels;
	VrData *m_data;
};

#endif // PLAYER_H
