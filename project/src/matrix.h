#ifndef MATRIX_H
#define MATRIX_H

#include <assert.h>
#include <math.h>
#include "vector.h"
#include <string.h>

#include <iostream>
using namespace std;

struct Matrix4 {
    Matrix4( float _00 = 0, float _01 = 0, float _02 = 0, float _03 = 0,
             float _10 = 0, float _11 = 0, float _12 = 0, float _13 = 0,
             float _20 = 0, float _21 = 0, float _22 = 0, float _23 = 0,
             float _30 = 0, float _31 = 0, float _32 = 0, float _33 = 0 ){
        data[0] = _00;        data[1] = _01;        data[2] = _02;        data[3] = _03;
        data[4] = _10;        data[5] = _11;        data[6] = _12;        data[7] = _13;
        data[8] = _20;        data[9] = _21;        data[10] = _22;       data[11] = _23;
        data[12] = _30;       data[13] = _31;       data[14] = _32;       data[15] = _33;
    }

    Matrix4(const float *copy)                 { memcpy(data, copy,      sizeof(float) * 16); }
    Matrix4(const Matrix4 &copy)           { memcpy(data, copy.data, sizeof(float) * 16); }
    Matrix4& operator=(const Matrix4 &rhs) { memcpy(data, rhs.data,  sizeof(float) * 16); return *this; }

    inline void fillArray(float* dest) const   { memcpy(dest, data,      sizeof(float) * 16); }

    inline       float& get(int row, int col)       { return data[row * 4 + col]; }
    inline const float& get(int row, int col) const { return data[row * 4 + col]; }

    static Matrix4 identity();

    inline Matrix4 getTranspose() {
        return Matrix4(a, e, i, m,
                       b, f, j, n,
                       c, g, k, o,
                       d, h, l, p);
    }

    inline Matrix4& operator+=(const Matrix4& rhs) {
#pragma vector align
        for (unsigned i = 0; i < 16; ++i)
            data[i] += rhs.data[i];
        return *this;
    }

    inline Matrix4& operator-=(const Matrix4& rhs) {
#pragma vector align
        for (unsigned i = 0; i < 16; ++i)
            data[i] -= rhs.data[i];
        return *this;
    }

    inline Matrix4 operator-(const Matrix4 &rhs) {
        return Matrix4(*this) -= rhs;
    }

    inline Matrix4 operator+(const Matrix4 &rhs) {
        return Matrix4(*this) += rhs;
    }

    inline Matrix4& operator*=(const float rhs) {
        data[0] *= rhs;
        data[1] *= rhs;
        data[2] *= rhs;
        data[3] *= rhs;
        data[4] *= rhs;
        data[5] *= rhs;
        data[6] *= rhs;
        data[7] *= rhs;
        data[8] *= rhs;
        data[9] *= rhs;
        data[10] *= rhs;
        data[11] *= rhs;
        data[12] *= rhs;
        data[13] *= rhs;
        data[14] *= rhs;
        data[15] *= rhs;
        return *this;
    }

    inline Vector4 operator*(const Vector4 &rhs) const {
        return Vector4( a*rhs.x + b*rhs.y + c*rhs.z + d*rhs.w,
                        e*rhs.x + f*rhs.y + g*rhs.z + h*rhs.w,
                        i*rhs.x + j*rhs.y + k*rhs.z + l*rhs.w,
                        m*rhs.x + n*rhs.y + o*rhs.z + p*rhs.w );
    }

    inline void mulVec4(const Vector4 &rhs, Vector4 &out) {
        out.x = a*rhs.x + b*rhs.y + c*rhs.z + d*rhs.w;
        out.y = e*rhs.x + f*rhs.y + g*rhs.z + h*rhs.w;
        out.z = i*rhs.x + j*rhs.y + k*rhs.z + l*rhs.w;
        out.w = m*rhs.x + n*rhs.y + o*rhs.z + p*rhs.w;
    }

    inline Matrix4& operator/=(const float rhs) {
        data[0] /= rhs;
        data[1] /= rhs;
        data[2] /= rhs;
        data[3] /= rhs;
        data[4] /= rhs;
        data[5] /= rhs;
        data[6] /= rhs;
        data[7] /= rhs;
        data[8] /= rhs;
        data[9] /= rhs;
        data[10] /= rhs;
        data[11] /= rhs;
        data[12] /= rhs;
        data[13] /= rhs;
        data[14] /= rhs;
        data[15] /= rhs;
        return *this;
    }

    inline Matrix4 operator/(const float rhs) {
        return Matrix4(*this) /= rhs;
    }



    inline Matrix4 getInverse() const {
        return Matrix4(*this).invert();
    }

    inline float getDeterminant() {
        return  a*f*k*p - a*f*l*o - a*g*j*p + a*g*l*n + a*h*j*o -
                a*h*k*n - b*e*k*p + b*e*l*o + b*g*i*p - b*g*l*m -
                b*h*i*o + b*h*k*m + c*e*j*p - c*e*l*n - c*f*i*p +
                c*f*l*m + c*h*i*n - c*h*j*m - d*e*j*o + d*e*k*n +
                d*f*i*o - d*f*k*m - d*g*i*n + d*g*j*m;
    }

    inline Matrix4& invert() {
        float det = getDeterminant(),
        _00 = (f*k*p+g*l*n+h*j*o-f*l*o-g*j*p-h*k*n)/det,
        _01 = (b*l*o+c*j*p+d*k*n-b*k*p-c*l*n-d*j*o)/det,
        _02 = (b*g*p+c*h*n+d*f*o-b*h*o-c*f*p-d*g*n)/det,
        _03 = (b*h*k+c*f*l+d*g*j-b*g*l-c*h*j-d*f*k)/det,
        _10 = (e*l*o+h*k*m+g*i*p-e*k*p-g*l*m-h*i*o)/det,
        _11 = (a*k*p+c*l*m+d*i*o-a*l*o-c*i*p-d*k*m)/det,
        _12 = (a*h*o+c*e*p+d*g*m-a*g*p-c*h*m-d*e*o)/det,
        _13 = (a*g*l+c*h*i+d*e*k-a*h*k-c*e*l-d*g*i)/det,
        _20 = (e*j*p+f*l*m+h*i*n-e*l*n-f*i*p-h*j*m)/det,
        _21 = (a*l*n+b*i*p+d*j*m-a*j*p-b*l*m-d*i*n)/det,
        _22 = (a*f*p+b*h*m+d*e*n-a*h*n-b*e*p-d*f*m)/det,
        _23 = (a*h*j+b*e*l+d*f*i-a*f*l-b*h*i-d*e*j)/det,
        _30 = (e*k*n+f*i*o+g*j*m-e*j*o-f*k*m-g*i*n)/det,
        _31 = (a*j*o+b*k*m+c*i*n-a*k*n-b*i*o-c*j*m)/det,
        _32 = (a*g*n+b*e*o+c*f*m-a*f*o-b*g*m-c*e*n)/det,
        _33 = (a*f*k+b*g*i+c*e*j-a*g*j-b*e*k-c*f*i)/det;
        a = _00;
        b = _01;
        c = _02;
        d = _03;
        e = _10;
        f = _11;
        g = _12;
        h = _13;
        i = _20;
        j = _21;
        k = _22;
        l = _23;
        m = _30;
        n = _31;
        o = _32;
        p = _33;
        return *this;
    }

    inline Matrix4& operator*=(const Matrix4 &rhs) {
        float _00 = data[0]  * rhs.data[0] + data[1]  * rhs.data[4] + data[2]  * rhs.data[8]  + data[3]  * rhs.data[12];
        float _01 = data[0]  * rhs.data[1] + data[1]  * rhs.data[5] + data[2]  * rhs.data[9]  + data[3]  * rhs.data[13];
        float _02 = data[0]  * rhs.data[2] + data[1]  * rhs.data[6] + data[2]  * rhs.data[10] + data[3]  * rhs.data[14];
        float _03 = data[0]  * rhs.data[3] + data[1]  * rhs.data[7] + data[2]  * rhs.data[11] + data[3]  * rhs.data[15];

        float _10 = data[4]  * rhs.data[0] + data[5]  * rhs.data[4] + data[6]  * rhs.data[8]  + data[7]  * rhs.data[12];
        float _11 = data[4]  * rhs.data[1] + data[5]  * rhs.data[5] + data[6]  * rhs.data[9]  + data[7]  * rhs.data[13];
        float _12 = data[4]  * rhs.data[2] + data[5]  * rhs.data[6] + data[6]  * rhs.data[10] + data[7]  * rhs.data[14];
        float _13 = data[4]  * rhs.data[3] + data[5]  * rhs.data[7] + data[6]  * rhs.data[11] + data[7]  * rhs.data[15];

        float _20 = data[8]  * rhs.data[0] + data[9]  * rhs.data[4] + data[10] * rhs.data[8]  + data[11] * rhs.data[12];
        float _21 = data[8]  * rhs.data[1] + data[9]  * rhs.data[5] + data[10] * rhs.data[9]  + data[11] * rhs.data[13];
        float _22 = data[8]  * rhs.data[2] + data[9]  * rhs.data[6] + data[10] * rhs.data[10] + data[11] * rhs.data[14];
        float _23 = data[8]  * rhs.data[3] + data[9]  * rhs.data[7] + data[10] * rhs.data[11] + data[11] * rhs.data[15];

        float _30 = data[12] * rhs.data[0] + data[13] * rhs.data[4] + data[14] * rhs.data[8]  + data[15] * rhs.data[12];
        float _31 = data[12] * rhs.data[1] + data[13] * rhs.data[5] + data[14] * rhs.data[9]  + data[15] * rhs.data[13];
        float _32 = data[12] * rhs.data[2] + data[13] * rhs.data[6] + data[14] * rhs.data[10] + data[15] * rhs.data[14];
        float _33 = data[12] * rhs.data[3] + data[13] * rhs.data[7] + data[14] * rhs.data[11] + data[15] * rhs.data[15];

        data[0] = _00;
        data[1] = _01;
        data[2] = _02;
        data[3] = _03;
        data[4] = _10;
        data[5] = _11;
        data[6] = _12;
        data[7] = _13;
        data[8] = _20;
        data[9] = _21;
        data[10] = _22;
        data[11] = _23;
        data[12] = _30;
        data[13] = _31;
        data[14] = _32;
        data[15] = _33;
        return *this;
    }

    inline Matrix4 operator*(const Matrix4& rhs) const {
        return Matrix4(*this) *= rhs;
    }

    inline Matrix4 operator*(const float rhs) const {
        return Matrix4(*this) *= rhs;
    }

    union {
        float data[16];
        struct {
            float a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p;
        };
    };
};

inline std::ostream &operator << (std::ostream &out, const Matrix4 &m) {
    return out << m.a << " " << m.b << " " << m.c << " " << m.d << endl <<
           m.e << " " << m.f << " " << m.g << " " << m.h << endl <<
           m.i << " " << m.j << " " << m.k << " " << m.l << endl <<
           m.m << " " << m.n << " " << m.o << " " << m.p;
}

// Extra operators where Matrix is on right-hand side
// -----------------------------------------------------

//! 1xM row vector * MxN matrix yields a 1xN vector
inline Vector4 operator* (const Vector4 &lhs, const Matrix4 &rhs) {
   return Vector4(rhs.m*lhs.w+rhs.a*lhs.x+rhs.e*lhs.y+rhs.i*lhs.z,
                  rhs.n*lhs.w+rhs.b*lhs.x+rhs.f*lhs.y+rhs.j*lhs.z,
                  rhs.o*lhs.w+rhs.c*lhs.x+rhs.g*lhs.y+rhs.k*lhs.z,
                  rhs.p*lhs.w+rhs.d*lhs.x+rhs.h*lhs.y+rhs.l*lhs.z);
}

//! @returns the MxN matrix resulting from multiplying a scalar by an MxN matrix
inline Matrix4 operator* (const float &scale, const Matrix4 &rhs) {
    return rhs * scale; }

#endif // MATRIX_H

