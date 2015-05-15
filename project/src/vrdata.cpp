#include "vrdata.h"

VrData::VrData(G3D::Table<string, G3D::CoordinateFrame> *frames) : m_frames(frames)
{
}

Vector3 VrData::getHeadLook()
{
    if (!m_frames->contains("Head_Tracker")) {
        return Vector3(0,0,0);
    }
    G3D::Vector3 look = (*m_frames)["Head_Tracker"].getLookVector();
    return Vector3(look.x, look.y, lookz);
}

Vector3 VrData::getHeadPos()
{
    if (!m_frames->contains("Head_Tracker")) {
        return Vector3(0,0,0);
    }
    G3D::Vector3 pos = (*m_frames)["Head_Tracker"].getLookRay().origin();
    return Vector3(pos.x, pos.y, pos.z);
}

Vector3 VrData::getWandDir()
{
    if (!m_frames->contains("Wand_Tracker")) {
        return Vector3(0,0,0);
    }
    G3D::Vector3 look = (*m_frames)["Wand_Tracker"].getLookVector();
    return Vector3(look.x, look.y, lookz);
}

Vector3 VrData::getWandPos()
{
    if (!m_frames->contains("Wand_Tracker")) {
        return Vector3(0,0,0);
    }
    G3D::Vector3 pos = (*m_frames)["Wand_Tracker"].getLookRay().origin();
    return Vector3(pos.x, pos.y, pos.z);
}
