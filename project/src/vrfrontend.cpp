#include "vrfrontend.h"

VRFrontend::VRFrontend(const string &mySetup, Application *app) : VRApp(), m_app(app)
{
    m_app->setFrontend(this);

    // Initialize the VRApp
    Log  *demoLog = new Log("demo-log.txt");
    init(mySetup, demoLog);

    // Setup the mouse tracker, using pure mouse delta for the first person camera in dekstop mode.
    m_mouseToTracker = new MouseToTracker(getCamera(), 2);
    _clearColor = Color3(0,0,0);

    m_app->initGL();
}

VRFrontend::~VRFrontend()
{
    delete m_app;
}

// Called by VRApp when render events happen - this calls draw(), where most rendering code should go
void VRFrontend::doGraphics(RenderDevice *rd)
{
    while(glGetError() != GL_NO_ERROR) {
    }

    // Use the tracker frames to draw the laser pointer
    Array<string> trackerNames = m_trackerFrames.getKeys();
    for (int i=0;i<trackerNames.size();i++) {
        CoordinateFrame trackerFrame = m_trackerFrames[trackerNames[i]];

        // Draw laser pointer
        if (trackerNames[i] == "Wand_Tracker") {
            Vector3 lookVector = trackerFrame.lookVector();

            glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT);

            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glLineWidth(2.0f);
            glColor4f(1.0f,0.0f,0.0f,1.0f);
            glBegin(GL_LINES);
            glVertex3f(trackerFrame.translation.x,
                       trackerFrame.translation.y,
                       trackerFrame.translation.z);
            glVertex3f(trackerFrame.translation.x + 6.0 * lookVector.x,
                       trackerFrame.translation.y + 6.0 * lookVector.y,
                       trackerFrame.translation.z + 6.0 * lookVector.z);
            glEnd();
            glPopAttrib();
        }
    }


    // The tracker frames above are drawn with the object to world
    // matrix set to the identity because tracking data comes into the
    // system in the Room Space coordinate system.  Room Space is tied
    // to the dimensions of the room and the projection screen within
    // the room, thus it never changes as your program runs.  However,
    // it is often convenient to move objects around in a virtual
    // space that can change relative to the screen.  For these
    // objects, we put a virtual to room space transform on the OpenGL
    // matrix stack before drawing them, as is done here...
    rd->disableLighting();
    rd->pushState();

    // Tick the world before updating the camera
    doTick();

    // Basically, set the camera equal to the current camera parameters
    rd->setObjectToWorldMatrix(m_app->m_camera.getCoordinateFrame());

//    // Lighting parameters and setup
//    GLfloat ambient[] = {0.0, 0.0, 0.0, 1.0};
//    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
//    GLfloat position[] = {0.0, 3.0, 3.0, 0.0};
//    GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
//    GLfloat local_view[] = {0.0};

//    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
//    glLightfv(GL_LIGHT0, GL_POSITION, position);
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
//    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
//    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);

    // More GL setup...
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);

//    // Use these if you plan to scale an object
//    glEnable (GL_AUTO_NORMAL);
//    glEnable (GL_NORMALIZE);

    // This draws a simple piece of geometry using G3D::Draw at the
    // origin of Virtual Space.
    // Draw::axes( CoordinateFrame(), rd, Color3::red(), Color3::green(), Color3::blue(), 0.25 );

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glDisable(GL_COLOR_MATERIAL);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glDisable(GL_LIGHTING);

    // After tons of setup, finally do our rendering
    m_app->draw();

    rd->popState();
}

// Called by VRApp when user input happens
void VRFrontend::doUserInput(Array<VRG3D::EventRef> &events)
{
    m_app->mouseMoved(_userInput->mouseDXY());

    double joystick_x = 0.0;
    double joystick_y = 0.0;

    // Populate the list of new events
    Array<VRG3D::EventRef> newEvents;
    m_mouseToTracker->doUserInput(events, newEvents);
    events.append(newEvents);

    // Handle events one at a time
    for (int i = 0; i < events.size(); i++) {
        string event = events[i]->getName();

        // Save all the tracker events that come in so we can use them
        // in the doGraphics routine
        if (endsWith(event, "_Tracker")) {
            if (m_trackerFrames.containsKey(event)) {
                m_trackerFrames[event] = events[i]->getCoordinateFrameData();
            } else {
                m_trackerFrames.set(event, events[i]->getCoordinateFrameData());
            }
        }

        // Check for key events
        else if (event.find("kbd") == 0) {
            parseKeyEvent(event);
        }

        // Used for mouse move events?
        else if (event == "Mouse_Pointer") {
            static Vector2 lastPos;
            if (events[i]->get2DData() != lastPos) {
                lastPos = events[i]->get2DData();
            }
        }

        // Check for mouse events
        else if (beginsWith(event, "Mouse")) {
            parseMouseEvent(event);
        }

        else if (event == "Wand_Joystick_X") {
            joystick_x = events[i]->get1DData();
        }

        else if (event == "Wand_Joystick_Y") {
            joystick_y = events[i]->get1DData();
        }

        else if (event == "SpaceNav_Trans") {
            cout << "Keyboard event 2: " << event<< events[i]->get3DData() << endl;
        }

        else if (event == "SpaceNav_Rot") {
            cout << "Keyboard event 3: " << event<< events[i]->get3DData() << endl;
        }

        else if (event == "SynchedTime") {
            continue;
        }

        else {
            // This will print out the names of all events, but can be too
            // much if you are getting several tracker updates per frame.
            // Uncomment this to see everything..
             cout << event << endl;
        }

//        // Rotate the camera using the wand
//        if (fabs(joystick_x) > 0.01) {
//            // fprintf(stderr, "Joystick x: %lf\n", joystick_x);
//            double angle = M_PI/180.0*joystick_x;
//            angle /= 2.0;
//            CoordinateFrame rotation = CoordinateFrame(Matrix3::fromEulerAnglesXYZ(0,angle,0));
//            m_camera = rotation*m_camera;
//        }

//        // Translate the camera in the direction of the wand
//        if (fabs(joystick_y) > 0.0 && m_trackerFrames.containsKey("Wand_Tracker") == true) {
//            m_camera.translation -= .05f*joystick_y*m_trackerFrames[string("Wand_Tracker")].lookVector();
//        }
    }
}

void VRFrontend::doTick()
{
    // Get the elapsed time since the last tick
    long now = glutGet(GLUT_ELAPSED_TIME);
    long dtLong = (now - m_prevTime);
    float dt = (float) (dtLong) / 1000.f;

    // Special case for the first tick
    if (m_prevTime == 0) {
        m_prevTime = now;
        dt = 0;
    }

    // Update previous time for the next tick.
    m_prevTime = now;
    m_app->tick(dt);
}

void VRFrontend::parseKeyEvent(const string &event)
{
    string withoutKBD = event.substr(4);
    int index = withoutKBD.find("_");
    string key = withoutKBD.substr(0, index);
    string upOrDown = withoutKBD.substr(index+1);

    if (upOrDown[0] == 'd') {
        m_app->keyPressed(key);
    } else {
        m_app->keyReleased(key);
    }
}

void VRFrontend::parseMouseEvent(const string &event)
{
    if (endsWith(event, "Pointer")) return;
    string withoutMouse = event.substr(6);
    MouseEvent e;
    e.button = beginsWith(withoutMouse, "Left") ? MOUSE_LEFT :
                  (beginsWith(withoutMouse, "Right") ? MOUSE_RIGHT : MOUSE_MIDDLE);

    if (endsWith(withoutMouse, "down")) {
        m_app->mousePressed(e);
    } else {
        m_app->mouseReleased(e);
    }
}