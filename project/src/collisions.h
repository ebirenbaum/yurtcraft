#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <limits>

#include "triangle.h"
#include "ellipsoid.h"
#include "cylinder.h"
#include "obj.h"
#include "aab.h"
#include "blockdef.h"

using namespace std;

struct Collision {
    float t;
    Vector3 p, n;

    Collision() : t(-1) {}
    Collision(float t, const Vector3 &p, const Vector3 &n) : t(t), p(p), n(n) {}
    Collision(const Vector3 &e1, float t, const Vector3 &p1, const Vector3 &radius) : t(t), p(p1 * radius),
        n(((e1 - p1) / radius).unit()) {}
};

struct VoxelCollision {
    VoxelCollision() : bl(Block()), mtv(Vector3()) {}
    VoxelCollision(const Block &b, const Vector3 &v) : bl(b), mtv(v) {}

    Block bl;
    Vector3 mtv;
};

struct VoxelIntersection {

    VoxelIntersection() : t(-1) {}
    VoxelIntersection(float t, const Block &block, const Vector3 &p, const Vector3 &n) : t(t), p(p), n(n) {}

    float t;
    Block bl;
    Vector3 p, n;
};

class Collisions {
public:
    Collisions();
    static Collision collide(const Triangle &unscaledTriangle, const Ellipsoid &unscaledStart, const Ellipsoid &unscaledEnd);

    static Ray intersection(const Plane &one, const Plane &two);

//    static Collision raycast(const Ray &ray, const OBJ &obj);
    static Collision raycast(const Ray &ray, vector<Triangle> *triangles);
    static Collision raycast(const Ray &ray, const Triangle &triangle);
    static Collision raycast(const Ray &unscaledRay, const Ellipsoid &unscaledEllipsoid);
    static Collision raycast(const Ray &ray, const AAB &aab);

    static Collision raycastHack(const Ray &ray, vector<Triangle> *triangles);
    static Collision raycastHack(const Ray &ray, const Triangle &t);
};

#endif // COLLISIONS_H
