#ifndef INCLUDES_H
#define INCLUDES_H

#include "vector.h"

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
