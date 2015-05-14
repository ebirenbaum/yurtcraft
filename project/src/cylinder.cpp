#include "cylinder.h"

Cylinder::Cylinder(const Vector3 &pos, float radius, float height) :
    p(pos), r(radius), h(height) {
}

Cylinder::~Cylinder() {
}

Vector3 Cylinder::collide(const Cylinder &other) const {
    Vector3 bestMtv;

    if (other.p.y < top() && other.top() > p.y) {
        float radialSum = r + other.r,
              horizontalDistanceSquared = p.horizontalDistanceSquared(other.p);
        if (horizontalDistanceSquared < radialSum * radialSum) {
            float meUp = other.top() - p.y,
                  meDown = top() - other.p.y,
                  sideways = radialSum - p.horizontalDistanceTo(other.p);

            float best = MIN3(meUp, meDown, sideways);

            if (best == meUp) {
                bestMtv = Vector3(0, meUp, 0);
            } else if (best == meDown) {
                bestMtv = Vector3(0, -meDown, 0);
            } else {
                bestMtv = p - other.p;
                bestMtv.y = 0;
                bestMtv.normalize();
                bestMtv *= sideways;
            }
        }
    }

    return bestMtv;
}
