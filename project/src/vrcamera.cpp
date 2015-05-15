#include "vrcamera.h"

VRCamera::VRCamera()
{
    m_yaw = 90.f;
    m_pitch = 0.f;
}

VRCamera::~VRCamera()
{
}

void VRCamera::translate(const Vector3 &t)
{
    m_frame.translation -= G3D::Vector3(t.x, t.y, t.z);
}

void VRCamera::setEye(const Vector3 &eye)
{
    m_frame.translation = -G3D::Vector3(eye.x, eye.y, eye.z);
}

Vector3 VRCamera::getEye() const
{
    return Vector3(-m_frame.translation.x, -m_frame.translation.y, -m_frame.translation.z);
}

void VRCamera::yaw(float deg)
{
    m_yaw = fmod(m_yaw + deg, 360.f);
}

void VRCamera::pitch(float deg)
{
    m_pitch += deg;
    m_pitch = MAX(-89, MIN(89, m_pitch));
}

G3D::CoordinateFrame VRCamera::getCoordinateFrame() const
{
    return G3D::CoordinateFrame(G3D::Matrix3::fromAxisAngle(G3D::Vector3(0,1,0), D2R(m_yaw))) *
           G3D::CoordinateFrame(G3D::Matrix3::fromAxisAngle(G3D::Vector3(cos(D2R(m_yaw)), 0, sin(D2R(m_yaw))), D2R(m_pitch))) *
           m_frame;
}

Vector3 VRCamera::getLook() const
{
    float cosine = cos(D2R(m_pitch));
    return Vector3(sin(D2R(m_yaw)) * cosine, -sin(D2R(m_pitch)), cosine * -cos(D2R(m_yaw)));
}

Vector3 VRCamera::getHorizontalLook() const
{
    return Vector3(sin(D2R(m_yaw)), 0, -cos(D2R(m_yaw)));
}

Vector3 VRCamera::getRight() const
{
    return Vector3(cos(D2R(m_yaw)), 0, sin(D2R(m_yaw)));
}
