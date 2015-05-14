#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <utility>
#include <functional>

// Returns a random number between 0 and 1
inline double frand() { return (double)rand() / (double)RAND_MAX; }
inline float frandf() { return (float)rand() / (float)RAND_MAX; }

inline float urand(float lower = 0.f, float upper = 0.f) {
    return frand() * (upper - lower) + lower;
}

using namespace std;

#define SQ(x) (x)*(x)

#define MIN(x, y) ({ ((x < y) ? x : y); })
#define MAX(x, y) ({ ((x > y) ? x : y); })

#define MIN3(x, y, z) ({ (MIN(x, MIN(y, z))); })
#define MAX3(x, y, z) ({ (MAX(x, MAX(y, z))); })

#define D2R(d) (d * M_PI / 180)
#define R2D(r) (r * 180 / M_PI)
#define INRANGE(v,min,max) (v >= min && v <= max)
#define BETWEEN(v,min,max) (v > min && v < max)

#define EPS .0001

#define EQ(x, y) (INRANGE(x - y, -EPS, EPS))

#define M2_PI (M_PI * 2)

class Vector2
{
public:
    // This union provides both individual members "x" and "y" and an array "xy"
    // that are automatically in sync (since they actually refer to the same data).
    // This is useful for OpenGL to pass vectors to the gl*2fv() functions:
    //
    //     Vector2 vec(1, 2);
    //     glVertex2fv(vec.xy);
    //
    union
    {
        struct { float x, y; };
        float xy[2];
    };

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
    Vector2(const Vector2 &vec) : x(vec.x), y(vec.y) {}

    Vector2 operator + () const { return Vector2(+x, +y); }
    Vector2 operator - () const { return Vector2(-x, -y); }

    Vector2 operator + (const Vector2 &vec) const { return Vector2(x + vec.x, y + vec.y); }
    Vector2 operator - (const Vector2 &vec) const { return Vector2(x - vec.x, y - vec.y); }
    Vector2 operator * (const Vector2 &vec) const { return Vector2(x * vec.x, y * vec.y); }
    Vector2 operator / (const Vector2 &vec) const { return Vector2(x / vec.x, y / vec.y); }
    Vector2 operator + (float s) const { return Vector2(x + s, y + s); }
    Vector2 operator - (float s) const { return Vector2(x - s, y - s); }
    Vector2 operator * (float s) const { return Vector2(x * s, y * s); }
    Vector2 operator / (float s) const { return Vector2(x / s, y / s); }

    friend Vector2 operator + (float s, const Vector2 &vec) { return Vector2(s + vec.x, s + vec.y); }
    friend Vector2 operator - (float s, const Vector2 &vec) { return Vector2(s - vec.x, s - vec.y); }
    friend Vector2 operator * (float s, const Vector2 &vec) { return Vector2(s * vec.x, s * vec.y); }
    friend Vector2 operator / (float s, const Vector2 &vec) { return Vector2(s / vec.x, s / vec.y); }

    Vector2 &operator += (const Vector2 &vec) { return *this = *this + vec; }
    Vector2 &operator -= (const Vector2 &vec) { return *this = *this - vec; }
    Vector2 &operator *= (const Vector2 &vec) { return *this = *this * vec; }
    Vector2 &operator /= (const Vector2 &vec) { return *this = *this / vec; }
    Vector2 &operator += (float s) { return *this = *this + s; }
    Vector2 &operator -= (float s) { return *this = *this - s; }
    Vector2 &operator *= (float s) { return *this = *this * s; }
    Vector2 &operator /= (float s) { return *this = *this / s; }

    bool operator == (const Vector2 &vec) const { return x == vec.x && y == vec.y; }
    bool operator != (const Vector2 &vec) const { return x != vec.x || y != vec.y; }

    float lengthSquared() const { return x * x + y * y; }
    float length() const { return sqrtf(lengthSquared()); }
    float dot(const Vector2 &vec) const { return x * vec.x + y * vec.y; }
    float cross(const Vector2 &vec) const { return x*vec.y - vec.x*y; }
    Vector2 unit() const { return *this / length(); }
    void normalize() { *this = unit(); }

    // Fraction should be a value in the range [0, 1]
    static Vector2 lerp(const Vector2 &a, const Vector2 &b, float fraction) { return a + (b - a) * fraction; }
    static Vector2 lerp(const Vector2 &a, const Vector2 &b, const Vector2 &fraction) { return a + (b - a) * fraction; }

    float min() const { return fminf(x, y); }
    float max() const { return fmaxf(x, y); }
    static Vector2 min(const Vector2 &a, const Vector2 &b) { return Vector2(fminf(a.x, b.x), fminf(a.y, b.y)); }
    static Vector2 max(const Vector2 &a, const Vector2 &b) { return Vector2(fmaxf(a.x, b.x), fmaxf(a.y, b.y)); }

    Vector2 floor() const { return Vector2(floorf(x), floorf(y)); }
    Vector2 ceil() const { return Vector2(ceilf(x), ceilf(y)); }
    Vector2 abs() const { return Vector2(fabsf(x), fabsf(y)); }

    // Returns an angle in the range [-pi, pi]
    float toAngle() const { return atan2f(y, x); }
    static Vector2 fromAngle(float theta) { return Vector2(cosf(theta), sinf(theta)); }

    // Returns a uniformly-random unit vector
    static Vector2 randomDirection() { return fromAngle(frand() * 2 * M_PI); }

    pair<float, float> toPair() { return pair<float, float>(x,y); }
    int hashcode() const {
        int prime = 31;
        int result = 1;
        result = prime * result + (~0 & int(x));
        result = prime * result + (~0 & int(y));
        return result;
    }
};

class Vector3
{
public:
    // This union provides both individual members "x", "y", and "z" an array "xyz"
    // that are automatically in sync (since they actually refer to the same data).
    // This is useful for OpenGL to pass vectors to the gl*3fv() functions:
    //
    //     Vector3 vec(1, 2, 3);
    //     glVertex3fv(vec.xyz);
    //
    union
    {
        struct { float x, y, z; };
        float xyz[3];
    };

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(const Vector3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}

    Vector3 operator + () const { return Vector3(+x, +y, +z); }
    Vector3 operator - () const { return Vector3(-x, -y, -z); }

    Vector3 operator + (const Vector3 &vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
    Vector3 operator - (const Vector3 &vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
    Vector3 operator * (const Vector3 &vec) const { return Vector3(x * vec.x, y * vec.y, z * vec.z); }
    Vector3 operator / (const Vector3 &vec) const { return Vector3(x / vec.x, y / vec.y, z / vec.z); }
    Vector3 operator + (float s) const { return Vector3(x + s, y + s, z + s); }
    Vector3 operator - (float s) const { return Vector3(x - s, y - s, z - s); }
    Vector3 operator * (float s) const { return Vector3(x * s, y * s, z * s); }
    Vector3 operator / (float s) const { return Vector3(x / s, y / s, z / s); }

    friend Vector3 operator + (float s, const Vector3 &vec) { return Vector3(s + vec.x, s + vec.y, s + vec.z); }
    friend Vector3 operator - (float s, const Vector3 &vec) { return Vector3(s - vec.x, s - vec.y, s - vec.z); }
    friend Vector3 operator * (float s, const Vector3 &vec) { return Vector3(s * vec.x, s * vec.y, s * vec.z); }
    friend Vector3 operator / (float s, const Vector3 &vec) { return Vector3(s / vec.x, s / vec.y, s / vec.z); }

    Vector3 &operator += (const Vector3 &vec) { return *this = *this + vec; }
    Vector3 &operator -= (const Vector3 &vec) { return *this = *this - vec; }
    Vector3 &operator *= (const Vector3 &vec) { return *this = *this * vec; }
    Vector3 &operator /= (const Vector3 &vec) { return *this = *this / vec; }
    Vector3 &operator += (float s) { return *this = *this + s; }
    Vector3 &operator -= (float s) { return *this = *this - s; }
    Vector3 &operator *= (float s) { return *this = *this * s; }
    Vector3 &operator /= (float s) { return *this = *this / s; }

    bool operator == (const Vector3 &vec) const { return x == vec.x && y == vec.y && z == vec.z; }
    bool operator != (const Vector3 &vec) const { return x != vec.x || y != vec.y || z != vec.z; }

    float lengthSquared() const { return x * x + y * y + z * z; }
    float length() const { return sqrtf(lengthSquared()); }
    float horizontalLengthSquared() const { return x * x + z * z; }
    float horizontalLength() const { return sqrtf(horizontalLengthSquared()); }
    float distanceTo(const Vector3 &vec) const { return (*this - vec).length(); }
    float distanceSquared(const Vector3 &vec) const { return (*this - vec).lengthSquared(); }
    float horizontalDistanceTo(const Vector3 &vec) const { return (*this - vec).horizontalLength(); }
    float horizontalDistanceSquared(const Vector3 &vec) const { return (*this - vec).horizontalLengthSquared(); }
    float dot(const Vector3 &vec) const { return x * vec.x + y * vec.y + z * vec.z; }
    Vector3 cross(const Vector3 &vec) const { return Vector3(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x); }
    Vector3 unit() const { return *this / length(); }
    void normalize() { *this = unit(); }

    // Fraction should be a value in the range [0, 1]
    static Vector3 lerp(const Vector3 &a, const Vector3 &b, float fraction) { return a + (b - a) * fraction; }
    static Vector3 lerp(const Vector3 &a, const Vector3 &b, const Vector3 &fraction) { return a + (b - a) * fraction; }

    float min() const { return fminf(x, fminf(y, z)); }
    float max() const { return fmaxf(x, fmaxf(y, z)); }
    static Vector3 min(const Vector3 &a, const Vector3 &b) { return Vector3(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z)); }
    static Vector3 max(const Vector3 &a, const Vector3 &b) { return Vector3(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z)); }

    Vector3 floor() const { return Vector3(floorf(x), floorf(y), floorf(z)); }
    Vector3 ceil() const { return Vector3(ceilf(x), ceilf(y), ceilf(z)); }
    Vector3 abs() const { return Vector3(fabsf(x), fabsf(y), fabsf(z)); }

	// All the angle-related methods assume OpenGL coordinates: +y is up, (z, x) is the horizontal plane.
    float angleTo(const Vector3 &other) { return acos((*this).dot(other) / (this->length() * other.length())); }

	// Returns yaw (aka heading) in the range [-pi, pi]
	float yaw() const { return atan2f(z, x); }
	// Returns the pitch in the range [-pi/2, pi/2]
	float pitch() const { return asinf(y / length()); }

    // Returns a vector of angles (yaw, pitch)
    Vector2 toAngles() const { return Vector2(yaw(), pitch()); }

    // Returns a unit vector from the given yaw and pitch angles
    static Vector3 fromAngles(float yaw, float pitch) { return Vector3(cosf(yaw) * cosf(pitch), sinf(pitch), sinf(yaw) * cosf(pitch)); }
    static Vector3 fromAngles(const Vector2 &yawPitch) { return fromAngles(yawPitch.x, yawPitch.y); }

    // Returns a uniformly-random unit vector
    static Vector3 randomDirection() { return fromAngles(frand() * 2 * M_PI, asinf(frand() * 2 - 1)); }

    // Retruns a random offset vector from this vector by the given angles
    Vector3 randomRotation(float angle) {
        Vector2 angles = toAngles();
        angles += Vector2(angle * (frand() - .5), angle * (frand() - .5));

        if (angles.x > M_PI) angles.x = angles.x - 2 * M_PI;
        else if (angles.x > M_PI) angles.x = angles.x + 2 * M_PI;

        if (angles.y > M_PI) angles.y = angles.y - M_PI;
        else if (angles.y > M_PI) angles.y = angles.y + M_PI;
        return fromAngles(angles);
    }

    // Projects this vector onto a line
    Vector3 projectOnto(const Vector3 &p, const Vector3 &d) { return ((*this - p).dot(d)) * d + p; }

    // Projects this vector onto another
    Vector3 projectOnto(const Vector3 &b) { return ((*this).dot(b) / b.lengthSquared()) * b; }

    float distToPlane(const Vector3 &plane, float d) {
        return fabs((*this).dot(plane) - d) / plane.length();
    }

    pair<float, pair<float, float> > toPair() { return pair<float, pair<float, float> >(x, Vector2(y,z).toPair()); }
    int hashcode() const {
        int prime = 31;
        int result = 1;
        result = prime * result + (~0 & int(x));
        result = prime * result + (~0 & int(y));
        result = prime * result + (~0 & int(z));
        return result;
    }
    int hashcode(int seed) const {
        int prime = 31;
        int result = 1;
        result = prime * result + (~0 & int(x));
        result = prime * result + (~0 & int(y));
        result = prime * result + (~0 & int(z));
        result = prime * result + (~0 & int(seed));
        return result;
    }
};

class Vector4
{
public:
    // This union provides both individual members "x", "y", "z", and "w" an array "xyzw"
    // that are automatically in sync (since they actually refer to the same data).
    // This is useful for OpenGL to pass vectors to the gl*4fv() functions:
    //
    //     Vector4 vec(1, 2, 3, 1);
    //     glVertex4fv(vec.xyz);
    //
    union
    {
        struct { float x, y, z, w; };
        float xyzw[4];
    };

    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4(const Vector4 &vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
    Vector4(const Vector3 &vec, float w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}

    Vector4 operator + () const { return Vector4(+x, +y, +z, +w); }
    Vector4 operator - () const { return Vector4(-x, -y, -z, -w); }

    Vector4 operator + (const Vector4 &vec) const { return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w); }
    Vector4 operator - (const Vector4 &vec) const { return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
    Vector4 operator * (const Vector4 &vec) const { return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w); }
    Vector4 operator / (const Vector4 &vec) const { return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w); }
    Vector4 operator + (float s) const { return Vector4(x + s, y + s, z + s, w + s); }
    Vector4 operator - (float s) const { return Vector4(x - s, y - s, z - s, w - s); }
    Vector4 operator * (float s) const { return Vector4(x * s, y * s, z * s, w * s); }
    Vector4 operator / (float s) const { return Vector4(x / s, y / s, z / s, w / s); }

    friend Vector4 operator + (float s, const Vector4 &vec) { return Vector4(s + vec.x, s + vec.y, s + vec.z, s + vec.w); }
    friend Vector4 operator - (float s, const Vector4 &vec) { return Vector4(s - vec.x, s - vec.y, s - vec.z, s - vec.w); }
    friend Vector4 operator * (float s, const Vector4 &vec) { return Vector4(s * vec.x, s * vec.y, s * vec.z, s * vec.w); }
    friend Vector4 operator / (float s, const Vector4 &vec) { return Vector4(s / vec.x, s / vec.y, s / vec.z, s / vec.w); }

    Vector4 &operator += (const Vector4 &vec) { return *this = *this + vec; }
    Vector4 &operator -= (const Vector4 &vec) { return *this = *this - vec; }
    Vector4 &operator *= (const Vector4 &vec) { return *this = *this * vec; }
    Vector4 &operator /= (const Vector4 &vec) { return *this = *this / vec; }
    Vector4 &operator += (float s) { return *this = *this + s; }
    Vector4 &operator -= (float s) { return *this = *this - s; }
    Vector4 &operator *= (float s) { return *this = *this * s; }
    Vector4 &operator /= (float s) { return *this = *this / s; }

    bool operator == (const Vector4 &vec) const { return x == vec.x && y == vec.y && z == vec.z && w == vec.w; }
    bool operator != (const Vector4 &vec) const { return x != vec.x || y != vec.y || z != vec.z || w != vec.w; }

    float lengthSquared() const { return x * x + y * y + z * z + w * w; }
    float length() const { return sqrtf(lengthSquared()); }
    float dot(const Vector4 &vec) const { return x * vec.x + y * vec.y + z * vec.z + w * vec.w; }
    Vector4 homogenized() const { return *this / w; }
    void homogenize() { *this /= w; }

    // Fraction should be a value in the range [0, 1]
    static Vector4 lerp(const Vector4 &a, const Vector4 &b, float fraction) { return a + (b - a) * fraction; }
    static Vector4 lerp(const Vector4 &a, const Vector4 &b, const Vector4 &fraction) { return a + (b - a) * fraction; }

    float min() const { return fminf(fminf(x, y), fminf(z, w)); }
    float max() const { return fmaxf(fmaxf(x, y), fmaxf(z, w)); }
    static Vector4 min(const Vector4 &a, const Vector4 &b) { return Vector4(fminf(a.x, b.x), fminf(a.y, b.y), fminf(a.z, b.z), fminf(a.w, b.w)); }
    static Vector4 max(const Vector4 &a, const Vector4 &b) { return Vector4(fmaxf(a.x, b.x), fmaxf(a.y, b.y), fmaxf(a.z, b.z), fmaxf(a.w, b.w)); }

    Vector4 floor() const { return Vector4(floorf(x), floorf(y), floorf(z), floorf(w)); }
    Vector4 ceil() const { return Vector4(ceilf(x), ceilf(y), ceilf(z), ceilf(w)); }
    Vector4 abs() const { return Vector4(fabsf(x), fabsf(y), fabsf(z), fabsf(w)); }

    Vector3 toVector3() const { return Vector3(x,y,z); }
};

inline std::ostream &operator << (std::ostream &out, const Vector2 &v) { return out << "(" << v.x << ", " << v.y << ")"; }
inline std::ostream &operator << (std::ostream &out, const Vector3 &v) { return out << "(" << v.x << ", " << v.y << ", " << v.z << ")"; }
inline std::ostream &operator << (std::ostream &out, const Vector4 &v) { return out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")"; }

inline std::string appendNum(std::string s, int num) {
    std::ostringstream txt;
    txt << s << num;
    return txt.str();
}

struct Ray {
    Vector3 p, d;
    Ray() : p(Vector3()), d(Vector3()) {}
    Ray(const Vector3 &point, const Vector3 &dir) : p(point), d(dir.unit()) {}

    Vector3 cast(float t) const { return p + d * t; }
    Ray scale(const Vector3 &basis) const {
        Vector3 firstP = p * basis, secondP = (p + d) * basis;
        return Ray(firstP, (secondP - firstP).unit());
    }

    float distanceSquared(const Vector3 &point) {
        Vector3 originPoint = point - p;
        Vector3 closestPoint = Ray::cast(*this, originPoint.dot(d));
        return closestPoint.distanceSquared(point);
    }

    static Vector3 cast(const Ray& ray, float t) { return ray.p + ray.d * t; }

    Ray randomOffset(float spread) {
        Vector3 randDir = Vector3(frand() - .5, frand() - .5, frand() - .5);
        randDir.normalize();
        randDir = randDir / 2.f;
        Vector3 newD = d + randDir * (spread / 10.f);
        return Ray(p, newD);
    }
};

struct Plane {
    Vector3 n;
    float d;
    Plane(const Vector3 &normal, const Vector3 &point) : n(normal), d(normal.dot(point)) {}
    Plane(const Vector3 &normal, float d) : n(normal), d(d) {}

    float apply(const Vector3 &point) { return point.dot(n) - d; }

    static float applyPlaneEquation(const Vector3 &pos, const Vector4 &equation) {
        return pos.x * equation.x + pos.y * equation.y + pos.z * equation.z + equation.w;
    }
};

struct Light {

    Vector3 s; // shift
    Vector4 c; // color
    float r; // radius

    Light(const Vector3 &s, const Vector4 &c, float r) : s(s), c(c), r(r){
    }

    Light() : s(Vector3()), c(Vector4(1,1,1,1)), r(0){
    }
};

#endif // VECTOR_H

