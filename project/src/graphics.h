#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GLG3D/Texture.h>

#include "includes.h"
#include "vector.h"
#include "matrix.h"
#include "cylinder.h"
#include "aab.h"
#include <map>

class OBJ;

class Graphics {
public:

    bool debug;

    Graphics();
    virtual ~Graphics();

    void initializeGL();

    inline void translate(const Vector3 &trans) { glTranslatef(trans.x, trans.y, trans.z); }
    inline void scale(const Vector3 &scale) { glScalef(scale.x, scale.y, scale.z); }

    inline void enableBlend() { glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); }
    inline void disableBlend() { glDisable(GL_BLEND); }

    void loadTexture(const string &path, const string &key);
    void setupTexture(const string &key);
    void teardownTexture();

//    void loadObject(const string &path, const string &key);
//    void drawObject(const string &key);

    bool isCulled(const AAB& aab);
    void updateCamera();

    void renderSphere(float radius);

    void renderCylinder(const Cylinder &cylinder);
    void renderCylinder(float radius, float height);

    void renderQuad();
    void renderCube();

    void renderTexturedQuad(string key, const Vector2 &botLeft = Vector2(0,0), const Vector2 &topRight = Vector2(1,1));
    void renderTexturedQuad(const Vector2 &botLeft = Vector2(0,0), const Vector2 &topRight = Vector2(1,1));
    void renderCubeFrame();

    void renderTexturedCube(string top, string side = "", string bottom = "");
    void renderTexturedCube(string top, string bottom, string front, string back, string left, string right);
    void renderTexturedCube(string sheet, Vector2 *vecs);
    void renderTexturedCube(string sheet, Vector4 vec);
    void renderTexturedCube(string sheet, Vector4 *coordinates);

//    void renderText(int x, int y, const string & str, const QFont &fnt = QFont(), int listBase = 2000);

    void renderCompass(const Vector3 &look, const Vector2 &dim, const vector<Vector3> &directions = vector<Vector3>());

private:

    void glRenderQuad();

    void renderAxes(float angleForAxes, float width);
    void renderXZ(bool x, bool positive);

    GLUquadric *m_quad;

    map<string, G3D::Texture::Ref> m_textures;
    map<string, map<string, Vector4> > m_atlases;
//    map<string, OBJ> m_objs;

    Vector4 m_r0, m_r1, m_r2, m_r3;
};

#endif // GRAPHICS_H
