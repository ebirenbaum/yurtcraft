#ifndef SKYBOX_H
#define SKYBOX_H

#include "graphics.h"

class Skybox {
public:

    Skybox();
    virtual ~Skybox();

    void draw(Graphics *g);
};

#endif // SKYBOX_H
