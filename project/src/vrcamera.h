#ifndef VRCAMERA_H
#define VRCAMERA_H

#include <G3D/CoordinateFrame.h>

#include "includes.h"

using namespace std;

class VRCamera
{
public:
    VRCamera();
    virtual ~VRCamera();

    CoordinateFrame getCoordinateFrame() const;

    void translate(const Vector3 &t);

    void setEye(const Vector3 &eye);
    Vector3 getEye() const;

    void yaw(float deg);
    void pitch(float deg);

    Vector3 getLookVector() const;
    Vector3 getHorizontalLookVector() const;
    Vector3 getRightVector() const;

private:
    CoordinateFrame m_frame;
    float m_yaw, m_pitch;
};

#endif // VRCAMERA_H
