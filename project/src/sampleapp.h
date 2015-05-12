#ifndef SAMPLEAPP_H
#define SAMPLEAPP_H

#include <stdlib.h>
#include <set>
#include <iostream>

#include "vrfrontend.h"

class SampleApp : public VRFrontend::Application
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
    void mousePressed(const MouseEvent &e);
    void mouseReleased(const MouseEvent &e);

private:
    void updateCamera(float seconds);
    set<string> m_heldKeys;
    inline bool isKeyHeld(const string &key) { return m_heldKeys.find(key) != m_heldKeys.end(); }
};

#endif // SAMPLEAPP_H
