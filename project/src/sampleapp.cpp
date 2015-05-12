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
    m_camera.translate(Vector3(0,2,0));
}

void SampleApp::tick(float seconds)
{
    if (m_camera.getEye().y > 2) {
        m_camera.translate(seconds * Vector3(0,-9.8,0));
    }
    updateCamera(seconds);
}

void SampleApp::draw()
{
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
    glVertex3f(-10,0,10);
    glVertex3f(10,0,10);
    glVertex3f(10,0,-10);
    glVertex3f(-10,0,-10);
    glEnd();
}

void SampleApp::mouseMoved(const Vector2 &delta)
{
    m_camera.yaw(delta.x / 5);
    m_camera.pitch(delta.y / 5);
}

void SampleApp::mousePressed(const MouseEvent &e)
{
    cout << e.button << " pressed!" << endl;
}

void SampleApp::mouseReleased(const MouseEvent &e)
{
    cout << e.button << " released!" << endl;
}

void SampleApp::keyPressed(const string &key)
{
    m_heldKeys.insert(key);

    // Exit on escape
    if (key == "ESC") {
        exit(0);
    }

    if (key == "SPACE") {
        m_camera.setEye(m_camera.getEye() + Vector3(0,10,0));
    }
}

void SampleApp::keyReleased(const string &key)
{
    m_heldKeys.erase(key);
}

// Basic math to set up the camera
void SampleApp::updateCamera(float seconds) {
    float speed = 10;
    Vector3 look = m_camera.getHorizontalLookVector() * seconds * speed;
    Vector3 perpendicular = m_camera.getRightVector() * seconds * speed;

    if (isKeyHeld("A")) {
        m_camera.translate(-perpendicular);
    }

    if (isKeyHeld("D")) {
        m_camera.translate(perpendicular);
    }

    if (isKeyHeld("W")) {
        m_camera.translate(look);
    }

    if (isKeyHeld("S")) {
        m_camera.translate(-look);
    }
}
