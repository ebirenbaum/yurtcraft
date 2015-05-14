#include "triangle.h"
#include "stdio.h"

Triangle::Triangle() {
    v[0] = Vector3();
    v[1] = Vector3(0,0,1);
    v[2] = Vector3(1,0,1);

    zAligned = false;
}

Triangle::Triangle(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3){
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
    n = (v[1] - v[0]).cross(v[2] - v[0]);
    n.normalize();

    zAligned = false;
}

Triangle::Triangle(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, bool z){
    v[0] = v1;
    v[1] = v2;
    v[2] = v3;
    n = (v[1] - v[0]).cross(v[2] - v[0]);
    n.normalize();

    zAligned = z;
}

Triangle::~Triangle() {
}

Triangle Triangle::scale(Vector3 basis) const {
    return Triangle(v[0] * basis, v[1] * basis, v[2] * basis);
}

Triangle Triangle::flatten() const {
    Vector3 one = v[0], two = v[1], three = v[2];
    one.y = 0; two.y = 0; three.y = 0;
    return Triangle(one, two, three);
}

Triangle Triangle::translate(const Vector3 &t) const {
    return Triangle(v[0] + t, v[1] + t, v[2] + t);
}

bool Triangle::contains(const Vector3 &p) const {
    Vector3 n1 = (v[1] - v[0]).cross(p - v[0]),
            n2 = (v[2] - v[1]).cross(p - v[1]),
            n3 = (v[0] - v[2]).cross(p - v[2]);
    return (n1.dot(n2) > 0) && (n2.dot(n3) > 0);
}

bool Triangle::contains2(const Vector3 &p) const {
    float Area = .5*(-v[1].y*v[2].x + v[0].y*(-v[1].x + v[2].x) + v[0].x*(v[1].y - v[2].y) + v[1].x*v[2].y);
    float s = 1.f/(2.f*Area)*(v[0].y*v[2].x - v[0].x*v[2].y + (v[2].y - v[0].y)*p.x + (v[0].x - v[2].x)*p.y);
    if (s < 0) return false;
    float t = 1.f/(2.f*Area)*(v[0].x*v[1].y - v[0].y*v[1].x + (v[0].y - v[1].y)*p.x + (v[1].x - v[0].x)*p.y);
    return (t < 0 && 1 - s - t < 0);
}

bool Triangle::contains3(const Vector3 &p) const {
    int as_x = p.x-v[0].x;
    int as_y = p.y-v[0].y;

    bool s_ab = (v[1].x-v[0].x)*as_y-(v[1].y-v[0].y)*as_x > 0;

    if ((v[2].x-v[0].x)*as_y-(v[2].y-v[0].y)*as_x > 0 == s_ab) return false;

    if ((v[2].x-v[1].x)*(p.y-v[1].y)-(v[2].y-v[1].y)*(p.x-v[1].x) > 0 != s_ab) return false;

    return true;
}

Vector3 Triangle::center() const {
    return (v[0] + v[1] + v[2]) / 3.0;
}
