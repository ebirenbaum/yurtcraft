#ifndef INCLUDES_H
#define INCLUDES_H

using namespace G3D;

#include <G3D/Vector3.h>

inline float frand() { return (float)rand() / (float)RAND_MAX; }
inline float urand(double a, double b) { return a + (b - a) * frand(); }

#include <iostream>
using namespace std;

#define EPSILON .0001

#define D2R(d) (d * M_PI / 180)
#define R2D(r) (r * 180 / M_PI)

#define MAX(a,b) (a >= b ? a : b)
#define MIN(a,b) (a <= b ? a : b)

enum MouseButton {
    MOUSE_LEFT,
    MOUSE_RIGHT,
    MOUSE_MIDDLE
};

struct MouseEvent {
    Vector2 pos;
    MouseButton button;
};

#endif // INCLUDES_H
