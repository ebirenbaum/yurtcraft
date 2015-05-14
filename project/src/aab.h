#ifndef AAB_H
#define AAB_H

#include "includes.h"
#include "vector.h"
#include <vector>

class AAB {
public:
    AAB();
    AAB(const Vector3& negative, const Vector3 &positive);
    AAB(const AAB &other);
    virtual ~AAB();

    inline void translate(const Vector3 &translation) { m_negi += translation; m_posi += translation; }
    inline Vector3 center() const { return (m_negi + m_posi) / 2; }

    inline bool collides(const AAB &other) const {
        return m_negi.x <= other.m_posi.x && m_posi.x >= other.m_negi.x &&
                m_negi.y <= other.m_posi.y && m_posi.y >= other.m_negi.y &&
                m_negi.z <= other.m_posi.z && m_posi.z >= other.m_negi.z; }

    inline bool contains(const Vector3 &p) const {
        return INRANGE(p.x, m_negi.x, m_posi.x) &&
                INRANGE(p.y, m_negi.y, m_posi.y) &&
                INRANGE(p.z, m_negi.z, m_posi.z); }

    Vector3 getMTV(const AAB &other);

    vector<Vector3> getCorners() const;
    Vector3 m_negi, m_posi;

private:
    float intervalMTV(const Vector2 &a, const Vector2 &b);
};

#endif // AAB_H
