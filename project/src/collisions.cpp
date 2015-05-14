#include "collisions.h"

Collisions::Collisions() {
}

Collision Collisions::collide(const Triangle &unscaledTriangle, const Ellipsoid &unscaledStart,
                              const Ellipsoid &unscaledEnd) {
    // Converted triangle and ellipsoids
    Vector3 basis = unscaledStart.getUnscaler();
    Triangle triangle = unscaledTriangle.scale(basis);
    Ellipsoid start = unscaledStart.scale(basis), end = unscaledEnd.scale(basis);
    Vector3 a = start.c, b = end.c, bMinusA = b - a, n = triangle.n;

    // Sphere-interior
    float iT = -1.0 * (n.dot(a - n - triangle.v[0])) / (n.dot(bMinusA));
    Vector3 contact = (a - n) + (iT * bMinusA);
    if (INRANGE(iT, 0, 1) && triangle.contains(contact)) {
        Vector3 e = a + (iT * bMinusA);
        return Collision(e, iT, contact, unscaledStart.r);
    }

    // Sphere-edge
    float eT = 2;
    Vector3 sphereEdgePoint;
    for (int i = 0; i < 3; i++) {

        // Get all the appropriate edge parameters
        Vector3 c = triangle.v[i], d = triangle.v[(i + 1) % 3], dMinusC = d - c, dmcU = dMinusC.unit(),
                cross1 = (a - c).cross(dMinusC), cross2 = bMinusA.cross(dMinusC);
        float dmcs = dMinusC.lengthSquared(),
              alpha = (cross2).lengthSquared(),
              beta = (2.0 * cross2).dot(cross1),
              gamma = (cross1).lengthSquared() - dmcs;
        float determinant = beta * beta - 4.0 * alpha * gamma;

        // Do the quadratic formula if necessary
        if (determinant >= 0) {
            float twoAlpha = (2.0 * alpha);
            float eT1 = (-beta + sqrt(determinant)) / twoAlpha,
                  eT2 = (-beta - sqrt(determinant)) / twoAlpha;

            Vector3 p1 = a + eT1 * bMinusA,
                    p2 = a + eT2 * bMinusA;

            // Only take valid t values, between 0 and 1, and then pick the minimum one if so
            if (INRANGE(eT1, 0, 1) && BETWEEN((p1 - c).dot(dMinusC), 0, dmcs)) {
                if (eT1 <= eT) {
                    eT = eT1;
                    sphereEdgePoint = p1.projectOnto(c, dmcU);
                }
            }
            if (INRANGE(eT2, 0, 1) && BETWEEN((p2 - c).dot(dMinusC), 0, dmcs)) {
                if (eT2 <= eT) {
                    eT = eT2;
                    sphereEdgePoint = p2.projectOnto(c, dmcU);
                }
            }
        }
    }

    // Sphere-vertex
    float vT = 2;
    Vector3 sphereVertexPoint;
    for (int i = 0; i < 3; i++) {
        Vector3 v = triangle.v[i], vMinusA = v - a;
        float alpha = bMinusA.lengthSquared(),
              beta = (-2.0 * bMinusA).dot(vMinusA),
              gamma = vMinusA.lengthSquared() - 1;
        float determinant = beta * beta - 4.0 * alpha * gamma;

        // Do the quadratic formula if necessary
        if (determinant >= 0) {
            float twoAlpha = (2.0 * alpha);
            float vT1 = (-beta + sqrt(determinant)) / twoAlpha,
                  vT2 = (-beta - sqrt(determinant)) / twoAlpha;

            // Only take valid t values, between 0 and 1, and then pick the minimum one if so
            if (INRANGE(vT1, 0, 1)) {
                if (vT1 <= vT) {
                    vT = vT1;
                    sphereVertexPoint = v;
                }
            }

            if (INRANGE(vT2, 0, 1)) {
                if (vT2 <= vT) {
                    vT = vT2;
                    sphereVertexPoint = v;
                }
            }
        }
    }

    if (eT <= 1) {
        if (vT <= 1) {
            return Collision(a + (MIN(vT, eT) * bMinusA), MIN(vT, eT), vT < eT ? sphereVertexPoint : sphereEdgePoint, unscaledStart.r);
        }
        return Collision(a + (eT * bMinusA), eT, sphereEdgePoint, unscaledStart.r);
    } else if (vT <= 1) {
        return Collision(a + (vT * bMinusA), vT, sphereVertexPoint, unscaledStart.r);
    }
    return Collision();
}

Ray Collisions::intersection(const Plane &one, const Plane &two) {
    float k = one.n.dot(two.n);
    if (k == 1 || one.n == Vector3()) return Ray(Vector3(), Vector3());
    Vector3 output = (one.n * (one.d - two.d * k) + two.n * (two.d - one.d * k)) / (1 - k * k);
    return Ray(output, one.n.cross(two.n));
}

//Collision Collisions::raycast(const Ray &ray, const OBJ &obj) {
//    Collision best;
//    for (int i = 0; i < obj._triangles.size(); i++) {
//        Triangle t = obj._triangles[i];
//        Collision next = Collisions::raycast(ray, t);
//        if (next.t >= 0 && (best.t < 0 || next.t < best.t)) {
//            best = next;
//        }
//    }
//    return best;
//}

Collision Collisions::raycast(const Ray &ray, vector<Triangle> *triangles) {
    Collision best;
    for (int i = 0; i < triangles->size(); i++) {
        Triangle t = triangles->operator[](i);
        Collision next = Collisions::raycast(ray, t);
        if (next.t >= 0 && (best.t < 0 || next.t < best.t)) {
            best = next;
        }
    }
    return best;
}

Collision Collisions::raycast(const Ray &ray, const Triangle &triangle) {
    float t = (triangle.v[0] - ray.p).dot(triangle.n) / (ray.d.dot(triangle.n));
    if (t >= 0) {
        Vector3 point = Ray::cast(ray, t);
        if (triangle.contains(point)) return Collision(t, point, triangle.n);
    }
    return Collision();
}

Collision Collisions::raycastHack(const Ray &ray, vector<Triangle> *triangles) {
    Collision best;
    for (int i = 0; i < triangles->size(); i++) {
        Triangle t = triangles->operator[](i);
        Collision next = Collisions::raycastHack(ray, t);
        if (next.t >= 0 && (best.t < 0 || next.t < best.t)) {
            best = next;
        }
    }
    return best;
}

Collision Collisions::raycastHack(const Ray &ray, const Triangle &tr) {
    float t = (tr.v[0] - ray.p).dot(tr.n) / (ray.d.dot(tr.n));
    if (t >= 0) {
        Vector3 p = Ray::cast(ray, t);
        if (INRANGE(p.x, tr.minX(), tr.maxX()) && INRANGE(p.z, tr.minZ(), tr.maxZ()))
            return Collision(t, p, tr.n);
    }
    return Collision();
}

Collision Collisions::raycast(const Ray &unscaledRay, const Ellipsoid &unscaledEllipsoid) {
    Vector3 basis = unscaledEllipsoid.getUnscaler();
    Ellipsoid sphere = unscaledEllipsoid.scale(basis);
    Ray ray = unscaledRay.scale(basis);

    // Ray source outside the sphere
    if (ray.p.distanceSquared(sphere.c) > 1) {
        float scalarProjection = (sphere.c - ray.p).dot(ray.d);

        // Scalar projection must be positive
        if (scalarProjection > 0) {
            Vector3 closestPoint = ray.cast(scalarProjection);

            // Closest point has to be inside the sphere
            if (closestPoint.distanceSquared(sphere.c) <= 1) {
                float x = sphere.c.distanceTo(closestPoint);
                if (x > 1) return Collision();
                float t = scalarProjection - sqrt(1 - x * x);
                Vector3 intersection = ray.cast(t),
                        unscaledIntersection = intersection / basis;
                float unscaledT = -1;
                if (unscaledRay.d.x != 0) {
                    unscaledT = (unscaledIntersection.x - unscaledRay.p.x) / unscaledRay.d.x;
                } else if (unscaledRay.d.y != 0) {
                    unscaledT = (unscaledIntersection.y - unscaledRay.p.y) / unscaledRay.d.y;
                } else {
                    unscaledT = (unscaledIntersection.z - unscaledRay.p.z) / unscaledRay.d.z;
                }
                return Collision(unscaledT, unscaledIntersection, (unscaledIntersection - unscaledEllipsoid.c).unit());
            }
        }
    }
    return Collision();
}

Collision Collisions::raycast(const Ray &ray, const AAB &aab) {
    Vector3 negi = aab.m_negi, posi = aab.m_posi;
    float vals[6] = { -1,-1,-1,-1,-1,-1 };

    vals[0] = (negi.x - ray.p.x) / ray.d.x;
    vals[1] = (posi.x - ray.p.x) / ray.d.x;
    vals[2] = (negi.y - ray.p.y) / ray.d.y;
    vals[3] = (posi.y - ray.p.y) / ray.d.y;
    vals[4] = (negi.z - ray.p.z) / ray.d.z;
    vals[5] = (posi.z - ray.p.z) / ray.d.z;

    for (int i = 0; i < 6; i++) {
        Vector3 po = ray.cast(vals[i]);
        if (!aab.contains(po)) vals[i] = -1;
    }

    Vector3 normal;
    float min = numeric_limits<float>::max(); bool check = false;
    for (int i = 0; i < 6; i++) {
        if (vals[i] >= 0 && vals[i] < min) {
            min = vals[i];
            check = true;

            switch (i) {
            case 0:
                normal = Vector3(-1,0,0);
                break;
            case 1:
                normal = Vector3(1,0,0);
                break;
            case 2:
                normal = Vector3(0,-1,0);
                break;
            case 3:
                normal = Vector3(0,1,0);
                break;
            case 4:
                normal = Vector3(0,0,-1);
                break;
            default:
                normal = Vector3(0,0,1);
                break;
            }
        }
    }

    if (!check) return Collision();
    return Collision(min, ray.cast(min), normal);
}
