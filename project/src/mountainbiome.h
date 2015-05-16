#ifndef MOUNTAINBIOME_H
#define MOUNTAINBIOME_H

#include "biome.h"

class BiomeManager;

class MountainBiome : public Biome
{
public:
    MountainBiome(BiomeManager *b);

    float getBlockHeight(const Vector3 &_coordinates, BiomeGenInfo &_info);    
    unsigned char generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary);
    TerrainFeature* getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info);
};

#endif // MOUNTAINBIOME_H
