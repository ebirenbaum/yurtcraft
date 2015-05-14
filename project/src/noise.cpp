#include "noise.h"

Noise::Noise(int seed) : m_seed(seed) {
}

Noise::~Noise() {
}

float Noise::perlin2(const Vector2 &pos, float period) {
    return interNoise2(pos.x / period, pos.y / period);
}

float Noise::perlin3(const Vector3 &pos, float period) {
    return interNoise3(pos.x / period, pos.y / period, pos.z / period);
}

// Private noise methods

int Noise::fastFloor(float n) {
    return n > 0 ? (int)n : (int)n - 1;
}

float Noise::linterpolate(float a, float b, float x) {
    return (1.f - x) * a + x * b;
}

float Noise::costerpolate(float a, float b, float x) {
    float ft = x * M_PI;
    float f = (1.f - cos(ft)) * .5f;
    return a * (1.f - f) + b * f;
}

float Noise::noise2(int x, int y) {
//    srand(qHash(Vector2(x,y).qPair(_seed)));
//    return frand();
    int n = x + y * 57 + m_seed;
    n = (n<<13) ^ n;
    float out = (1.f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
    return out * .5f + .5f;
}

float Noise::smoothNoise2(float x, float y) {
    float c = (noise2(x - 1, y - 1) + noise2(x + 1, y - 1) +
               noise2(x - 1, y + 1) + noise2(x + 1, y + 1)) / 16.f;

    float f = (noise2(x - 1, y) + noise2(x + 1, y) +
               noise2(x, y - 1) + noise2(x, y + 1)) / 8.f;

    return c + f + noise2(x, y) / 4;
}

float Noise::interNoise2(float x, float y) {
    int ix = fastFloor(x), iy = fastFloor(y);
    float fx = x - ix, fy = y - iy;

    float v1 = smoothNoise2(ix, iy),
          v2 = smoothNoise2(ix + 1, iy),
          v3 = smoothNoise2(ix, iy + 1),
          v4 = smoothNoise2(ix + 1, iy + 1);

    float i1 = costerpolate(v1, v2, fx),
          i2 = costerpolate(v3, v4, fx);

    return costerpolate(i1, i2, fy);
}

float Noise::noise3(int x, int y, int z) {
    srand(Vector3(x,y,z).hashcode(m_seed));
    return frand();
}

float Noise::smoothNoise3(float x, float y, float z) {
    float three = (noise3(x-1,y-1,z-1) + noise3(x-1,y+1,z-1) + noise3(x-1,y-1,z+1) + noise3(x-1,y+1,z+1) +
                   noise3(x+1,y-1,z-1) + noise3(x+1,y+1,z-1) + noise3(x+1,y-1,z+1) + noise3(x+1,y+1,z+1)) / 48.f;

    float two = (noise3(x,y-1,z-1) + noise3(x,y+1,z-1) + noise3(x,y-1,z+1) + noise3(x,y+1,z+1) +
                 noise3(x-1,y,z-1) + noise3(x+1,y,z-1) + noise3(x-1,y,z+1) + noise3(x+1,y,z+1) +
                 noise3(x-1,y-1,z) + noise3(x+1,y-1,z) + noise3(x-1,y+1,z) + noise3(x+1,y+1,z)) / 36.f;

    float one = (noise3(x-1,y,z) + noise3(x+1,y,z) +
                 noise3(x,y-1,z) + noise3(x,y+1,z) +
                 noise3(x,y,z-1) + noise3(x,y,z+1)) / 18.f;

    return three + two + one + noise3(x, y, z) / 6.f;
}

float Noise::interNoise3(float x, float y, float z) {
    int ix = fastFloor(x), iy = fastFloor(y), iz = fastFloor(z);
    float fx = x - ix, fy = y - iy, fz = z - iz;

    float v1 = smoothNoise3(ix, iy, iz),
          v2 = smoothNoise3(ix + 1, iy, iz),
          v3 = smoothNoise3(ix, iy + 1, iz),
          v4 = smoothNoise3(ix + 1, iy + 1, iz),
          v5 = smoothNoise3(ix, iy, iz + 1),
          v6 = smoothNoise3(ix + 1, iy, iz + 1),
          v7 = smoothNoise3(ix, iy + 1, iz + 1),
          v8 = smoothNoise3(ix + 1, iy + 1, iz + 1);

    float i1 = linterpolate(v1, v2, fx),
          i2 = linterpolate(v3, v4, fx),
          i3 = linterpolate(v5, v6, fx),
          i4 = linterpolate(v7, v8, fx);

    float j1 = linterpolate(i1, i2, fy),
          j2 = linterpolate(i3, i4, fy);

    return costerpolate(j1, j2, fz);
}
