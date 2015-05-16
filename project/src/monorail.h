#ifndef MONORAIL_H
#define MONORAIL_H

#include "entity.h"

class Player;
class McChunkFactory;
class VoxelSystem;
class Noise;

class Monorail : public Entity
{
public:
    Monorail(VoxelSystem *_voxel, McChunkFactory *_factory, Player *_passenger, Vector3 _pos);

    void addToPath();

    void tick(float _seconds);
    void draw(Graphics *g);

    VoxelSystem *voxel;
    McChunkFactory *factory;

    int currentGoalPointIndex;
    std::vector<Vector3> monorailGoalPoints;
    float timeToNextGoal;
    float speed;
    Vector3 baseDirection;


    Player *passenger;
    Noise *noise;

};

#endif // MONORAIL_H
