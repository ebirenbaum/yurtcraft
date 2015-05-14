#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

class Triangle {
public:

    // Vertices must be in CCW order
    Triangle();
    Triangle(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);
    Triangle(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, bool z);
    virtual ~Triangle();

    Triangle scale(Vector3 basis) const;
    Triangle flatten() const;
    Triangle translate(const Vector3 &t) const;
    bool contains(const Vector3 &p) const;
    bool contains2(const Vector3 &p) const;
    bool contains3(const Vector3 &p) const;

    inline float minX() const { return MIN(v[0].x, MIN(v[1].x, v[2].x)); }
    inline float maxX() const { return MAX(v[0].x, MAX(v[1].x, v[2].x)); }

    inline float minY() const { return MIN(v[0].y, MIN(v[1].y, v[2].y)); }
    inline float maxY() const { return MAX(v[0].y, MAX(v[1].y, v[2].y)); }

    inline float minZ() const { return MIN(v[0].z, MIN(v[1].z, v[2].z)); }
    inline float maxZ() const { return MAX(v[0].z, MAX(v[1].z, v[2].z)); }

    Vector3 center() const;

    Vector3 v[3];
    Vector3 n;
    bool zAligned;
};

inline std::ostream &operator << (std::ostream &out, const Triangle &t) {
    return out << "(" << t.v[0] << ", " << t.v[1] << ", " << t.v[2] << ")";
}


#endif // TRIANGLE_H
