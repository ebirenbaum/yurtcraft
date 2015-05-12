#ifndef VRFrontend_H
#define VRFrontend_H

#include <vrg3d/VRG3D.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <cmath>

#include "vrcamera.h"

using namespace std;

// A wrapper for VRApp providing a cleaner interface (Application) for code to be built on
class VRFrontend : public VRApp {

public:
    class Application
    {
    public:
        Application() {}
        virtual ~Application() {}

        virtual void tick(float seconds) = 0;
        virtual void draw() = 0;
        virtual void initGL() = 0;

        virtual void keyPressed(const string &key) = 0;
        virtual void keyReleased(const string &key) = 0;

        virtual void mouseMoved(const Vector2 &delta) = 0;
        virtual void mousePressed(const MouseEvent &e) = 0;
        virtual void mouseReleased(const MouseEvent &e) = 0;

        VRCamera m_camera;

        inline void setFrontend(VRFrontend *front) { m_frontend = front; }
        inline void setPureMouseDelta(bool val) { m_frontend->_userInput->setPureDeltaMouse(val); }

    private:
        VRFrontend *m_frontend;
    };

    VRFrontend(const string &mySetup, Application *app);
    virtual ~VRFrontend();

    // VRG3D events, translated into custom events and passed to m_app
    void doGraphics(RenderDevice *rd);
    void doUserInput(Array<VRG3D::EventRef> &events);

protected:

    // VRG3D tracking and basic drawing information
    Table<string, CoordinateFrame> m_trackerFrames;
    GFontRef m_font;
    MouseToTrackerRef m_mouseToTracker;

private:

    // For dispatching ticks tp m_app
    void doTick();
    long m_prevTime;

    // For parsing event strings into events for m_app
    void parseKeyEvent(const string &event);
    void parseMouseEvent(const string &event);

    // The application this frontend will run
    Application *m_app;
};

#endif // VRFrontend_H
