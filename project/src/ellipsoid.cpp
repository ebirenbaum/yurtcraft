#include "ellipsoid.h"

Ellipsoid::Ellipsoid(const Vector3 &center, const Vector3 &radii) : c(center), r(radii) {
    if (r.x <= 0) r.x = .0001;
    if (r.y <= 0) r.y = .0001;
    if (r.z <= 0) r.z = .0001;
}

Ellipsoid::~Ellipsoid() {
}

Ellipsoid Ellipsoid::scale(const Vector3 &basis) const {
    return Ellipsoid(c * basis, r * basis);
}

Ellipsoid Ellipsoid::translate(const Vector3 &translation) {
    return Ellipsoid(c + translation, r);
}

Vector3 Ellipsoid::getUnscaler() const {
    return Vector3(1.0 / r.x, 1.0 / r.y, 1.0 / r.z);
}
