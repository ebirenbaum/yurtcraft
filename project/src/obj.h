#ifndef OBJ_H
#define OBJ_H

//#include "includes.h"
//#include "graphics.h"

//#include <vector>

//// A simple parser that reads and writes Wavefront .obj files
//class OBJ
//{
//public:
//    struct Index
//    {
//        int vertex;
//        int coord;
//        int normal;

//        Index() : vertex(-1), coord(-1), normal(-1) {}
//        Index(int vertex, int coord, int normal) : vertex(vertex), coord(coord), normal(normal) {}
//    };

//    struct Triangle
//    {
//        Index a, b, c;

//        Triangle() {}
//        Triangle(const Index &a, const Index &b, const Index &c) : a(a), b(b), c(c) {}
//    };

//    vector<Vector3> vertices;
//    vector<Vector2> coords;
//    vector<Vector3> normals;
//    vector<Triangle> triangles;

//    void draw() const;
//    bool read(const string &path);
//    bool write(const string &path) const;

//private:
//    Index getIndex(const string &str) const;
//    void drawIndex(const Index &index) const;
//};

#endif // OBJ_H
