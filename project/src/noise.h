#ifndef NOISE_H
#define NOISE_H

#include "vector.h"

class Noise {
public:
    Noise(int seed);
    virtual ~Noise();

    float perlin2(const Vector2 &pos, float period);
    float perlin3(const Vector3 &pos, float period);

private:
    int fastFloor(float n);

    float linterpolate(float a, float b, float x);
    float costerpolate(float a, float b, float x);

    float noise2(int x, int y);
    float smoothNoise2(float x, float y);
    float interNoise2(float x, float y);

    float noise3(int x, int y, int z);
    float smoothNoise3(float x, float y, float z);
    float interNoise3(float x, float y, float z);

    int m_seed;
};

#endif // NOISE_H
