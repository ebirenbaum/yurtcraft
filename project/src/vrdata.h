#ifndef VRDATA_H
#define VRDATA_H

#include <G3D/Table.h>
#include <G3D/CoordinateFrame.h>

#include "vector.h"

class VrData
{
public:
    VrData(G3D::Table<string, G3D::CoordinateFrame> *frames);
    virtual ~VrData();

    Vector3 getHeadLook();
    Vector3 getHeadPos();
    Vector3 getWandDir();
    Vector3 getWandPos();

    Vector2 m_joystick;

private:
    G3D::Table<string, G3D::CoordinateFrame> *m_frames;
};

#endif // VRDATA_H
