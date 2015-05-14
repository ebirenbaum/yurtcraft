#include "graphics.h"
#include <G3D/FileSystem.h>
#include "obj.h"

Graphics::Graphics() {
    m_quad = gluNewQuadric();

    debug = false;
}

Graphics::~Graphics() {
    delete m_quad;
    m_textures.clear();
}

void Graphics::initializeGL() {
}

void Graphics::loadTexture(const string &path, const string &key) {
    if(m_textures.find(key) != m_textures.end()) {
        return;
    }

    // Set up the texture for rendering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
cout << "Loading image from " << path << endl;
    G3D::Texture::Ref texture = G3D::Texture::fromFile(path);
    m_textures[key] = texture;
}

void Graphics::setupTexture(const string &key) {
    if (m_textures.find(key) == m_textures.end()) {
	return;
	}
	G3D::Texture::Ref texture = m_textures[key];
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textures[key]->openGLID());
}

void Graphics::teardownTexture() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

//void Graphics::loadObject(const string &path, const string &key) {
//    OBJ obj;
//    if (obj.read(path)) {
//        m_objs[key] = obj;
//    }
//}

//void Graphics::drawObject(const string &key) {
//    if (m_objs.find(key) != m_objs.end()) m_objs[key].draw();
//}

bool Graphics::isCulled(const AAB &aab) {
    vector<Vector3> corners = aab.getCorners();

    // -X
    bool check = true;
    for (int i = 0; i < corners.size(); i++) {
        check = check && (Plane::applyPlaneEquation(corners[i], m_r3 - m_r0) < 0);
    }
    if (check) return true;

    // +X
    check = true;
    for (int i = 0; i < corners.size(); i++) {
        check = check && (Plane::applyPlaneEquation(corners[i], m_r3 + m_r0) < 0);
    }
    if (check) return true;


    // -Y
    check = true;
    for (int i = 0; i < corners.size(); i++) {
        check = check && (Plane::applyPlaneEquation(corners[i], m_r3 - m_r1) < 0);
    }
    if (check) return true;


    // +Y
    check = true;
    for (int i = 0; i < corners.size(); i++) {
        check = check && (Plane::applyPlaneEquation(corners[i], m_r3 + m_r1) < 0);
    }
    if (check) return true;


    // -Z
    check = true;
    for (int i = 0; i < corners.size(); i++) {
        check = check && (Plane::applyPlaneEquation(corners[i], m_r3 - m_r2) < 0);
    }
    if (check) return true;


    // +Z
    check = true;
    for (int i = 0; i < corners.size(); i++) {
        check = check && (Plane::applyPlaneEquation(corners[i], m_r3 + m_r2) < 0);
    }
    if (check) return true;


    return false;
}

void Graphics::updateCamera() {
    Matrix4 proj, model;
    glGetFloatv(GL_PROJECTION_MATRIX, proj.data);
    glGetFloatv(GL_MODELVIEW_MATRIX, model.data);

    proj = proj.getTranspose(); model = model.getTranspose();
    Matrix4 perspective = proj * model;

    m_r0 = Vector4(perspective.a, perspective.b, perspective.c, perspective.d);
    m_r1 = Vector4(perspective.e, perspective.f, perspective.g, perspective.h);
    m_r2 = Vector4(perspective.i, perspective.j, perspective.k, perspective.l);
    m_r3 = Vector4(perspective.m, perspective.n, perspective.o, perspective.p);
}

void Graphics::renderSphere(float radius) {
    gluSphere(m_quad, radius, 32, 16);
}

void Graphics::renderCylinder(const Cylinder &cylinder) {
    glPushMatrix();
    translate(cylinder.p);
    renderCylinder(cylinder.r, cylinder.h);
    glPopMatrix();
}

void Graphics::renderCylinder(float radius, float height) {
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    gluCylinder(m_quad, radius, radius, height, 32, 16);
    gluCylinder(m_quad, EPS, radius, EPS, 32, 16);
    glRotatef(180, 1, 0, 0);
    glTranslatef(0,0,-height);
    gluCylinder(m_quad, EPS, radius, EPS, 32, 16);
    glPopMatrix();
}

void Graphics::glRenderQuad() {
    glNormal3f(0,1,0);
    glVertex3f(-.5,0,-.5);
    glVertex3f(-.5,0,.5);
    glVertex3f(.5,0,.5);
    glVertex3f(.5,0,-.5);
}

void Graphics::renderQuad() {
    glBegin(GL_QUADS);
    glRenderQuad();
    glEnd();
}

void Graphics::renderCube() {
    glBegin(GL_QUADS);

    glPushMatrix();
    glTranslatef(0,.5,0);
    glRenderQuad();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-.5,0);
    glRotatef(180,0,0,1);
    glRenderQuad();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.5,0,0);
    glRotatef(90,0,0,1);
    glRenderQuad();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.5,0,0);
    glRotatef(90,0,0,1);
    glRotatef(180,1,0,0);
    glRenderQuad();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-.5);
    glRotatef(-90,0,1,0);
    glRotatef(90,0,0,1);
    glRenderQuad();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,.5);
    glRotatef(90,0,1,0);
    glRotatef(90,0,0,1);
    glRenderQuad();
    glPopMatrix();

    glEnd();
}

void Graphics::renderTexturedQuad(string key, const Vector2 &botLeft, const Vector2 &topRight) {
    setupTexture(key);
    glBegin(GL_QUADS);
    glTexCoord2f(botLeft.x,botLeft.y); glVertex3f(-.5,0,-.5);
    glTexCoord2f(topRight.x,botLeft.y); glVertex3f(-.5,0,.5);
    glTexCoord2f(topRight.x,topRight.y); glVertex3f(.5,0,.5);
    glTexCoord2f(botLeft.x,topRight.y); glVertex3f(.5,0,-.5);
    glEnd();
    teardownTexture();
}

void Graphics::renderCubeFrame() {
    glLineWidth(1.5);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-.5,-.5,-.5);
    glVertex3f(.5,-.5,-.5);
    glVertex3f(.5,-.5,.5);
    glVertex3f(-.5,-.5,.5);
    glVertex3f(-.5,-.5,-.5);
    glVertex3f(-.5,.5,-.5);
    glVertex3f(.5,.5,-.5);
    glVertex3f(.5,.5,.5);
    glVertex3f(-.5,.5,.5);
    glVertex3f(-.5,.5,-.5);
    glVertex3f(-.5,.5,.5);
    glVertex3f(-.5,-.5,.5);
    glVertex3f(.5,-.5,.5);
    glVertex3f(.5,.5,.5);
    glVertex3f(.5,.5,-.5);
    glVertex3f(.5,-.5,-.5);
    glEnd();
}

void Graphics::renderTexturedCube(string top, string side, string bottom) {
    bool sides = (side != "");
    bool bottoms = sides && bottom != "";
    renderTexturedCube(top, bottoms ? bottom : top,
                       sides ? side : top, sides ? side : top, sides ? side : top, sides ? side : top);
}

void Graphics::renderTexturedCube(string top, string bottom, string front, string back, string left, string right) {
    glPushMatrix();
    glTranslatef(0,.5,0);
    renderTexturedQuad(top);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-.5,0);
    glRotatef(180,0,0,1);
    renderTexturedQuad(bottom);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.5,0,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(front);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.5,0,0);
    glRotatef(90,0,0,1);
    glRotatef(180,1,0,0);
    renderTexturedQuad(back);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-.5);
    glRotatef(-90,0,1,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(left);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,.5);
    glRotatef(90,0,1,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(right);
    glPopMatrix();
}

void Graphics::renderTexturedCube(string sheet, Vector2 *vecs) {
    glPushMatrix();
    glTranslatef(0,.5,0);
    renderTexturedQuad(sheet, vecs[0], vecs[1]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-.5,0);
    glRotatef(180,0,0,1);
    renderTexturedQuad(sheet, vecs[2], vecs[3]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.5,0,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(sheet, vecs[4], vecs[5]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.5,0,0);
    glRotatef(90,0,0,1);
    glRotatef(180,1,0,0);
    renderTexturedQuad(sheet, vecs[6], vecs[7]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-.5);
    glRotatef(-90,0,1,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(sheet, vecs[8], vecs[9]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,.5);
    glRotatef(90,0,1,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(sheet, vecs[10], vecs[11]);
    glPopMatrix();
}

void Graphics::renderTexturedCube(string sheet, Vector4 vec) {
    glPushMatrix();
    glTranslatef(0,.5,0);
    renderTexturedQuad(sheet, Vector2(vec.x, vec.y), Vector2(vec.z, vec.w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-.5,0);
    glRotatef(180,0,0,1);
    renderTexturedQuad(sheet, Vector2(vec.x, vec.y), Vector2(vec.z, vec.w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.5,0,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(sheet, Vector2(vec.x, vec.y), Vector2(vec.z, vec.w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.5,0,0);
    glRotatef(90,0,0,1);
    glRotatef(180,1,0,0);
    renderTexturedQuad(sheet, Vector2(vec.x, vec.y), Vector2(vec.z, vec.w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-.5);
    glRotatef(-90,0,1,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(sheet, Vector2(vec.x, vec.y), Vector2(vec.z, vec.w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,.5);
    glRotatef(90,0,1,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(sheet, Vector2(vec.x, vec.y), Vector2(vec.z, vec.w));
    glPopMatrix();
}

void Graphics::renderTexturedCube(string sheet, Vector4 *coordinates) {
    setupTexture(sheet);

    glPushMatrix();
    glTranslatef(0,.5,0);
    renderTexturedQuad(sheet, Vector2(coordinates[0].x, coordinates[0].y), Vector2(coordinates[0].z, coordinates[0].w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-.5,0);
    glRotatef(180,0,0,1);
    renderTexturedQuad(sheet, Vector2(coordinates[1].x, coordinates[2].y), Vector2(coordinates[1].z, coordinates[1].w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-.5,0,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(sheet, Vector2(coordinates[4].x, coordinates[4].y), Vector2(coordinates[4].z, coordinates[4].w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.5,0,0);
    glRotatef(90,0,0,1);
    glRotatef(180,1,0,0);
    renderTexturedQuad(sheet, Vector2(coordinates[5].x, coordinates[5].y), Vector2(coordinates[5].z, coordinates[5].w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-.5);
    glRotatef(-90,0,1,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(sheet, Vector2(coordinates[2].x, coordinates[2].y), Vector2(coordinates[2].z, coordinates[2].w));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,.5);
    glRotatef(90,0,1,0);
    glRotatef(90,0,0,1);
    renderTexturedQuad(sheet, Vector2(coordinates[3].x, coordinates[3].y), Vector2(coordinates[3].z, coordinates[3].w));
    glPopMatrix();

    teardownTexture();
}

//void Graphics::renderText(int x, int y, const string & str, const QFont &fnt, int listBase) {
//    _view->renderText(x, y, str, fnt, listBase);
//}

void Graphics::renderCompass(const Vector3 &look, const Vector2 &dim, const vector<Vector3> &directions) {
    float halfWidth = dim.x / 2, yOffset = dim.y * .065 - 1;
    float width = .46f * dim.x, height = 20;

    glPushMatrix();
    glTranslatef(halfWidth, yOffset, 0);

    float angleForAxes = (Vector3(1,0,0).cross(look).y > 0 ? 1 : -1) * acos(look.dot(Vector3(1,0,0)));
    if (angleForAxes < 0) { angleForAxes += 2 * M_PI; } angleForAxes = R2D(angleForAxes);

    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glVertex2f(-width/1.9, -height/2);
    glVertex2f(width/1.9, -height/2);
    glVertex2f(width/1.9, height/2);
    glVertex2f(-width/1.9, height/2);
    glEnd();

    glColor3f(1,1,1);
    renderAxes(angleForAxes, width);

    for (int i = 0; i < directions.size(); i++) {
        float toPlayer = (directions[i].cross(look).y > 0 ? 1 : -1) * acos(look.dot(directions[i]));
        toPlayer = R2D(toPlayer);

        if (INRANGE(toPlayer, -46.f, 46.f)) {
            float xOffset = (toPlayer / 90.f) * width;

            glColor3f(0,1,0);
            glPushMatrix();
            glTranslatef(xOffset, 0, 0);
            glBegin(GL_LINE_STRIP);
            glVertex2f(0, 7);
            glVertex2f(-6, -7);
            glVertex2f(6, -7);
            glVertex2f(0, 7);
            glEnd();
            glPopMatrix();
        }
    }

    glPopMatrix();
}

void Graphics::renderAxes(float angleForAxes, float width) {
    width -= 12;
    glPushMatrix();
    glTranslatef(6,0,0);
    float halfWidth = width / 2;
    // +X axis 1
    if (INRANGE(angleForAxes, -1.f, 46.f)) {
        float xOffset = (angleForAxes / 90.f) * width;
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        renderXZ(true, true);
        glPopMatrix();
    }

    // +X axis 2
    if (INRANGE(angleForAxes, 314.f, 361.f)) {
        float xOffset = ((angleForAxes - 315.f) / 90.f) * width - halfWidth;
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        renderXZ(true, true);
        glPopMatrix();
    }

    // -Z axis
    if (INRANGE(angleForAxes, 44.f, 136.f)) {
        float xOffset = ((angleForAxes - 45.f) / 90.f) * width - halfWidth;
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        renderXZ(false, false);
        glPopMatrix();
    }

    // -X axis
    if (INRANGE(angleForAxes, 134.f, 226.f)) {
        float xOffset = ((angleForAxes - 135.f) / 90.f) * width - halfWidth;
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        renderXZ(true, false);
        glPopMatrix();
    }

    // +Z axis
    if (INRANGE(angleForAxes, 224.f, 316.f)) {
        float xOffset = ((angleForAxes - 225.f) / 90.f) *width - halfWidth;
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        renderXZ(false, true);
        glPopMatrix();
    }

    // Between 1
    if (INRANGE(angleForAxes, -1.f, 91.f)) {
        float xOffset = (angleForAxes / 90.f) *width - halfWidth;
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(0,7);
        glVertex2f(0,-7);
        glEnd();
        glPopMatrix();
    }

    // Between 2
    if (INRANGE(angleForAxes, 89.f, 181.f)) {
        float xOffset = ((angleForAxes - 90.f) / 90.f) *width - halfWidth;
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(0,7);
        glVertex2f(0,-7);
        glEnd();
        glPopMatrix();
    }

    // Between 3
    if (INRANGE(angleForAxes, 179.f, 271.f)) {
        float xOffset = ((angleForAxes - 180.f) / 90.f) *width - halfWidth;
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(0,7);
        glVertex2f(0,-7);
        glEnd();
        glPopMatrix();
    }

    // Between 4
    if (INRANGE(angleForAxes, 269.f, 361.f)) {
        float xOffset = ((angleForAxes - 270.f) / 90.f) *width - halfWidth;
        glPushMatrix();
        glTranslatef(xOffset, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(0,7);
        glVertex2f(0,-7);
        glEnd();
        glPopMatrix();
    }
}

void Graphics::renderXZ(bool x, bool positive) {
    glLineWidth(2);
    glBegin(GL_LINE_STRIP);
    if (!x && !positive) {
        glVertex2f(-7,-7);
        glVertex2f(-7,7);
        glVertex2f(7,-7);
        glVertex2f(7,7);
    } else if (!x && positive) {
        glVertex2f(-7,-4);
        glVertex2f(-4,-7);
        glVertex2f(4,-7);
        glVertex2f(7,-4);
        glVertex2f(-7,4);
        glVertex2f(-4,7);
        glVertex2f(4,7);
        glVertex2f(7,4);
    } else if (x && !positive) {
        glVertex2f(-7,7);
        glVertex2f(-3,-7);
        glVertex2f(0,7);
        glVertex2f(3,-7);
        glVertex2f(7,7);
    } else {
        glVertex2f(7,7);
        glVertex2f(-7,7);
        glVertex2f(-7,0);
        glVertex2f(7,0);
        glVertex2f(-7,0);
        glVertex2f(-7,-7);
        glVertex2f(7,-7);
    }
    glEnd();
}
