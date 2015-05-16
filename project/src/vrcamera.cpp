#include "vrcamera.h"

VrCamera::VrCamera()
{
    m_yaw = 90.f;
    m_pitch = 0.f;
}

VrCamera::~VrCamera()
{
}

void VrCamera::translate(const Vector3 &t)
{
    m_frame.translation -= G3D::Vector3(t.x, t.y, t.z);
}

void VrCamera::setEye(const Vector3 &eye)
{
    m_frame.translation = -G3D::Vector3(eye.x, eye.y, eye.z);
}

Vector3 VrCamera::getEye() const
{
    return Vector3(-m_frame.translation.x, -m_frame.translation.y, -m_frame.translation.z);
}

void VrCamera::yaw(float deg)
{
    m_yaw = fmod(m_yaw + deg, 360.f);
}

void VrCamera::pitch(float deg)
{
    m_pitch += deg;
    m_pitch = MAX(-89, MIN(89, m_pitch));
}

G3D::CoordinateFrame VrCamera::getCoordinateFrame() const
{
    return G3D::CoordinateFrame(G3D::Matrix3::fromAxisAngle(G3D::Vector3(0,1,0), D2R(m_yaw))) *
           G3D::CoordinateFrame(G3D::Matrix3::fromAxisAngle(G3D::Vector3(cos(D2R(m_yaw)), 0, sin(D2R(m_yaw))), D2R(m_pitch))) *
           m_frame;
}

Vector3 VrCamera::getLook() const
{
    float cosine = cos(D2R(m_pitch));
    return Vector3(sin(D2R(m_yaw)) * cosine, -sin(D2R(m_pitch)), cosine * -cos(D2R(m_yaw)));
}

Vector3 VrCamera::getHorizontalLook() const
{
    return Vector3(sin(D2R(m_yaw)), 0, -cos(D2R(m_yaw)));
}

Vector3 VrCamera::getRight() const
{
    return Vector3(cos(D2R(m_yaw)), 0, sin(D2R(m_yaw)));
}
