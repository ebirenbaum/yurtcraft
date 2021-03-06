#include "sampleapp.h"

SampleApp::SampleApp() : Application()
{
}

SampleApp::~SampleApp()
{

}

void SampleApp::initGL()
{
    this->setPureMouseDelta(true);
    this->setClipPlanes(.1, 400);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glEnable(GL_DEPTH_TEST);

    m_graphics = new Graphics();

    m_graphics->loadTexture("./res/img/terrain.jpg", "atlas");

    m_graphics->loadTexture("./res/img/skybox/posy.jpg", "skyboxtop");
    m_graphics->loadTexture("./res/img/skybox/negy.jpg", "skyboxbot");
    m_graphics->loadTexture("./res/img/skybox/posz.jpg", "skyboxfront");
    m_graphics->loadTexture("./res/img/skybox/negz.jpg", "skyboxback");
    m_graphics->loadTexture("./res/img/skybox/negx.jpg", "skyboxleft");
    m_graphics->loadTexture("./res/img/skybox/posx.jpg", "skyboxright");

    //m_graphics->loadTexture("./res/img/particle.png", "particle");

    /*for (int i = 0; i < 10; i++) {
        m_graphics->loadTexture(appendNum("./res/img/breaking/", i).append(".png"),
                               appendNum("break", i));
    }*/

    //    // Basically, disable lighting for textures
    //    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Set up global (ambient) lighting
    GLfloat global_ambient[] = { .05f, .05f, .05f, .05f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    // Set up GL_LIGHT0 with a position and lighting properties
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    m_world = new McWorld(0, &m_camera, m_data);
}

void SampleApp::tick(float seconds)
{
    m_world->tick(seconds);
}

void SampleApp::draw()
{
    m_world->draw(m_graphics);
}

void SampleApp::mouseMoved(const Vector2 &delta)
{
    m_world->mouseMoved(delta);
}

void SampleApp::mousePressed(MouseEvent *e)
{
    m_world->mousePressed(e);
}

void SampleApp::mouseReleased(MouseEvent *e)
{
    m_world->mouseReleased(e);
}

void SampleApp::mouseWheeled(int delta)
{
    m_world->mouseWheeled(delta);
}

void SampleApp::keyPressed(const string &key)
{
    // Exit on escape
    if (key == "ESC") {
        exit(0);
    } else if (key == "R") {
        m_world = new McWorld(0, &m_camera, m_data);
        return;
    }

    m_world->keyPressed(key);
}

void SampleApp::keyReleased(const string &key)
{
    m_world->keyReleased(key);
}

void SampleApp::wandButtonPressed(WandButton button)
{
    if (button == WAND_LEFT) m_world->tryRestart();
    else m_world->wandButtonPressed(button);
}

void SampleApp::wandButtonReleased(WandButton button)
{
    m_world->wandButtonReleased(button);
}

