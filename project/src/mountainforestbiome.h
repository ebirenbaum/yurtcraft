#ifndef MOUNTAINFORESTBIOME_H
#define MOUNTAINFORESTBIOME_H

#include "biome.h"

class BiomeManager;

class MountainForestBiome : public Biome
{
public:
    MountainForestBiome(BiomeManager *b);
    
    unsigned char generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary);
    TerrainFeature* getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info);
};


#endif // MOUNTAINFORESTBIOME_H
