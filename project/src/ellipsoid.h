#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include "vector.h"

class Ellipsoid {
public:
    Ellipsoid(const Vector3 &center, const Vector3 &radii);
    virtual ~Ellipsoid();

    Ellipsoid scale(const Vector3 &basis) const;
    Ellipsoid translate(const Vector3 &translation);
    Vector3 getUnscaler() const;

    Vector3 c, r;
};

inline std::ostream &operator << (std::ostream &out, const Ellipsoid &e) {
    return out << "(" << e.c << ", " << e.r << ")";
}

#endif // ELLIPSOID_H
