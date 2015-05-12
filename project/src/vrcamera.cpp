#include "vrcamera.h"

VRCamera::VRCamera()
{
    m_yaw = 0.f;
    m_pitch = 0.f;
}

VRCamera::~VRCamera()
{
}

void VRCamera::translate(const Vector3 &t)
{
    m_frame.translation -= t;
}

void VRCamera::setEye(const Vector3 &eye)
{
    m_frame.translation = -eye;
}

Vector3 VRCamera::getEye() const
{
    return -m_frame.translation;
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

CoordinateFrame VRCamera::getCoordinateFrame() const
{
    return CoordinateFrame(Matrix3::fromAxisAngle(Vector3(0,1,0), D2R(m_yaw))) *
           CoordinateFrame(Matrix3::fromAxisAngle(Vector3(cos(D2R(m_yaw)), 0, sin(D2R(m_yaw))), D2R(m_pitch))) *
           m_frame;
}

Vector3 VRCamera::getLookVector() const
{
    float cosine = cos(D2R(m_pitch));
    return Vector3(sin(D2R(m_yaw)) * cosine, sin(D2R(m_pitch)), cosine * -cos(D2R(m_yaw)));
}

Vector3 VRCamera::getHorizontalLookVector() const
{
    return Vector3(sin(D2R(m_yaw)), 0, -cos(D2R(m_yaw)));
}

Vector3 VRCamera::getRightVector() const
{
    return Vector3(cos(D2R(m_yaw)), 0, sin(D2R(m_yaw)));
}
