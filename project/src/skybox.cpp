#include "skybox.h"

Skybox::Skybox() {
}

Skybox::~Skybox() {
}

void Skybox::draw(Graphics *g) {

    glColor3f(1,1,1);
    float scale = 150;
    glNormal3f(0,-1,0);

    // Top
    glPushMatrix();
    glTranslatef(0, scale, 0);
    glRotatef(180, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    glScalef(2 * scale, 2 * scale, 2 * scale);
    g->renderTexturedQuad("skyboxtop");
    glPopMatrix();

    // Bottom
    glPushMatrix();
    glTranslatef(0, -scale, 0);
    glScalef(2 * scale, 2 * scale, 2 * scale);
    g->renderTexturedQuad("skyboxbot");
    glPopMatrix();

    // Positive x = front
    glPushMatrix();
    glTranslatef(scale, 0, 0);
    glRotatef(90, 0, 0, 1);
    glRotatef(180, 0, 1, 0);
    glScalef(2 * scale, 2 * scale, 2 * scale);
    g->renderTexturedQuad("skyboxfront");
    glPopMatrix();

    // Negative x = back
    glPushMatrix();
    glTranslatef(-scale, 0, 0);
    glRotatef(270, 0, 0, 1);
    glScalef(2 * scale, 2 * scale, 2 * scale);
    g->renderTexturedQuad("skyboxback");
    glPopMatrix();

    // Positive z = right
    glPushMatrix();
    glTranslatef(0, 0, scale);
    glRotatef(270, 1, 0, 0);
    glRotatef(90, 0, 1, 0);
    glScalef(2 * scale, 2 * scale, 2 * scale);
    g->renderTexturedQuad("skyboxleft");
    glPopMatrix();

    // Negative z = left
    glPushMatrix();
    glTranslatef(0, 0, -scale);
    glRotatef(90, 1, 0, 0);
    glRotatef(270, 0, 1, 0);
    glScalef(2 * scale, 2 * scale, 2 * scale);
    g->renderTexturedQuad("skyboxright");
    glPopMatrix();
}
