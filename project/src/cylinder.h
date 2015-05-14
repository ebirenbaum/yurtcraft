#ifndef CYLINDER_H
#define CYLINDER_H

#include <limits>

#include "vector.h"

class Cylinder {

public:
    Cylinder(const Vector3 &center = Vector3(), float radius = -1, float height = -1);
    virtual ~Cylinder();

    inline void translate(const Vector3 &trans) { p += trans; }
    Vector3 collide(const Cylinder &other) const;

    Vector3 p;
    float r, h;

private:
    inline float top() const { return p.y + h; }
};

inline std::ostream &operator << (std::ostream &out, const Cylinder &c) {
    return out << "(" << c.r << ", " << c.r << ") at " << c.p ;
}

#endif // CYLINDER_H
