#ifndef MOUNTAINPEAKFEATURE_H
#define MOUNTAINPEAKFEATURE_H

#include "terrainfeature.h"

class MountainPeakFeature : public TerrainFeature
{
public:
    MountainPeakFeature();
    MountainPeakFeature* clone(Vector3 _pos, BiomeGenInfo *_baseInfo);
    unsigned char generateBlock(Vector3 _address, BiomeGenInfo *_info);
};

#endif // MOUNTAINPEAKFEATURE_H
