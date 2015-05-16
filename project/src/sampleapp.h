#ifndef SAMPLEAPP_H
#define SAMPLEAPP_H

#include <stdlib.h>
#include <set>
#include <iostream>

#include "vrfrontend.h"
#include "graphics.h"
#include "mcworld.h"

class SampleApp : public VrFrontend::Application
{
public:
    SampleApp();
    virtual ~SampleApp();

    void tick(float seconds);
    void draw();
    void initGL();

    void keyPressed(const string &key);
    void keyReleased(const string &key);

    void mouseMoved(const Vector2 &delta);
    void mousePressed(MouseEvent *e);
    void mouseReleased(MouseEvent *e);
    void mouseWheeled(int delta);

    void joystickPressed();

private:
    Graphics *m_graphics;
    McWorld *m_world;
};

#endif // SAMPLEAPP_H
