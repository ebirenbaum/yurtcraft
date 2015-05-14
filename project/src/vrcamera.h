#ifndef VRCAMERA_H
#define VRCAMERA_H

#include <G3D/CoordinateFrame.h>
#include "includes.h"

class VRCamera
{
public:
    VRCamera();
    virtual ~VRCamera();

    G3D::CoordinateFrame getCoordinateFrame() const;

    void translate(const Vector3 &t);

    void setEye(const Vector3 &eye);
    Vector3 getEye() const;

    void yaw(float deg);
    void pitch(float deg);

    Vector3 getLook() const;
    Vector3 getHorizontalLook() const;
    Vector3 getRight() const;

private:
    G3D::CoordinateFrame m_frame;
    float m_yaw, m_pitch;
};

#endif // VRCAMERA_H
