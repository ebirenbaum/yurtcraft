#ifndef JOINT_H
#define JOINT_H

class Graphics;
class Rotation;

#include "vector.h"
#include "vector"
#include "map"

struct Rotation {
    Rotation(Vector3 _axis, float _degrees){
        axis = _axis;
        degrees = _degrees;
    }

    Vector3 axis;
    float degrees;
} __attribute__((packed));

class Joint
{
public:
    Joint(string _id, Vector3 _relativePosition, Rotation *_rotation1, Rotation *_rotation2 = NULL, Rotation *_rotation3 = NULL, bool _isRoot = false);
    virtual ~Joint();

    void paint(Graphics *g);
    virtual void paintAttachedObjects(Graphics *g); // this is what you override

    Joint* addChild(Joint *_child);


public:
    bool isRoot;
    map<string, Joint*> *rootListOfChildren;

    string id;

    Vector3 relativePos;

    Rotation *rotation1; // will need to add more than one eventually
    Rotation *rotation2;
    Rotation *rotation3;

    vector<Joint*> children;

};

#endif // JOINT_H
