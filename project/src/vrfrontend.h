#ifndef VRFRONTEND_H
#define VRFRONTEND_H

#include <vrg3d/VRG3D.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <cmath>

#include "vrcamera.h"
#include "vrdata.h"

// A wrapper for VRApp providing a cleaner interface (Application) for code to be built on
class VrFrontend : public VRApp {

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
        virtual void mousePressed(MouseEvent *e) = 0;
        virtual void mouseReleased(MouseEvent *e) = 0;
        virtual void mouseWheeled(int delta) = 0;

        virtual void wandButtonPressed(WandButton button) = 0;
	virtual void wandButtonReleased(WandButton button) = 0;

        VrCamera m_camera;

        virtual void setFrontend(VrFrontend *front, VrData *data) { m_frontend = front; m_data = data; }
        inline void setPureMouseDelta(bool val) { m_frontend->_userInput->setPureDeltaMouse(val); }

        void setClipPlanes(float near, float far) {
            VRG3D::ProjectionVRCameraRef camera = m_frontend->getCamera();
            VRG3D::DisplayTile tile = camera->getTile();
            tile.nearClip = near;
            tile.farClip = far;
            camera->setDisplayTile(tile);
        }

    protected:
        VrData *m_data;

    private:
        VrFrontend *m_frontend;
    };

    VrFrontend(const string &mySetup, Application *app);
    virtual ~VrFrontend();

    // VRG3D events, translated into custom events and passed to m_app
    void doGraphics(G3D::RenderDevice *rd);
    void doUserInput(G3D::Array<VRG3D::EventRef> &events);

protected:

    // VRG3D tracking and basic drawing information
    G3D::Table<string, G3D::CoordinateFrame> m_trackerFrames;
    G3D::GFontRef m_font;
    MouseToTrackerRef m_mouseToTracker;

private:

    // For dispatching ticks tp m_app
    void doTick();
    long m_prevTime;

    // For parsing event strings into events for m_app
    void parseKeyEvent(const string &event);
    void parseMouseEvent(const string &event);
    void parseWandEvent(const string &event);

    // The application this frontend will run
    Application *m_app;
    VrData m_data;
};

#endif // VRFRONTEND_H
