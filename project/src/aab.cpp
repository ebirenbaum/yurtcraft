#include "aab.h"

AAB::AAB() : m_negi(Vector3()), m_posi(Vector3()) {

}

AAB::AAB(const Vector3 &negative, const Vector3 &positive) {
    m_negi = negative;
    m_posi = positive;
}

AAB::AAB(const AAB &other) {
    m_negi = other.m_negi;
    m_posi = other.m_posi;
}

AAB::~AAB() {

}

vector<Vector3> AAB::getCorners() const {
    vector<Vector3> output;
    output.push_back(m_negi);
    output.push_back(Vector3(m_posi.x, m_negi.y, m_negi.z));
    output.push_back(Vector3(m_negi.x, m_posi.y, m_negi.z));
    output.push_back(Vector3(m_negi.x, m_negi.y, m_posi.z));
    output.push_back(Vector3(m_posi.x, m_posi.y, m_negi.z));
    output.push_back(Vector3(m_posi.x, m_negi.y, m_posi.z));
    output.push_back(Vector3(m_negi.x, m_posi.y, m_posi.z));
    output.push_back(m_posi);
    return output;
}

Vector3 AAB::getMTV(const AAB &other) {
    float xMTV = intervalMTV(Vector2(m_negi.x, m_posi.x), Vector2(other.m_negi.x, other.m_posi.x)),
          yMTV = intervalMTV(Vector2(m_negi.y, m_posi.y), Vector2(other.m_negi.y, other.m_posi.y)),
          zMTV = intervalMTV(Vector2(m_negi.z, m_posi.z), Vector2(other.m_negi.z, other.m_posi.z));

    float x = fabs(xMTV), y = fabs(yMTV), z = fabs(zMTV);

    if (xMTV == -1 || yMTV == -1 || zMTV == -1) return Vector3(-1,-1,-1);
    if (x <= y && x <= z) return Vector3(xMTV, 0, 0);
    if (y <= x && y <= z) return Vector3(0, yMTV, 0);
    return Vector3(0, 0, zMTV);
}

float AAB::intervalMTV(const Vector2 &a, const Vector2 &b) {
    float aRight = b.y - a.x, aLeft = a.y - b.x;
    if (aLeft < 0 || aRight < 0) return -1;
    if (aRight < aLeft) return aRight;
    return -aLeft;
}
