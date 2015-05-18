#include "vrdata.h"
#include <G3D/Ray.h>

VrData::VrData(G3D::Table<string, G3D::CoordinateFrame> *frames) : m_frames(frames)
{
}

VrData::~VrData()
{
}

Vector3 VrData::getHeadLook()
{
    if (!m_frames->containsKey("Head_Tracker")) {
        return Vector3(0,0,0);
    }
    G3D::Vector3 look = (*m_frames)["Head_Tracker"].lookVector();
    return Vector3(look.x, look.y, look.z);
}

Vector3 VrData::getHeadPos()
{
    if (!m_frames->containsKey("Head_Tracker")) {
        return Vector3(0,0,0);
    }
    G3D::Vector3 pos = (*m_frames)["Head_Tracker"].lookRay().origin();
    return Vector3(pos.x, pos.y, pos.z);
}

Vector3 VrData::getWandDir()
{
    if (!m_frames->containsKey("Wand_Tracker")) {
        return Vector3(0,0,0);
    }
    G3D::Vector3 look = (*m_frames)["Wand_Tracker"].lookVector();
    return Vector3(-look.z, look.y, look.x);
}

Vector3 VrData::getWandPos()
{
    if (!m_frames->containsKey("Wand_Tracker")) {
        return Vector3(0,0,0);
    }
    G3D::Vector3 pos = (*m_frames)["Wand_Tracker"].lookRay().origin();
    return Vector3(pos.x, pos.y, pos.z);
}
