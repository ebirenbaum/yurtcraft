#ifndef DESERTBIOME_H
#define DESERTBIOME_H

#include "biome.h"

class BiomeManager;

class DesertBiome : public Biome
{
public:
    DesertBiome(BiomeManager *b);
    
    float getBlockHeight(const Vector3 &_coordinates, BiomeGenInfo &_info);    
    unsigned char generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary);
    TerrainFeature* getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info);
};

#endif // DESERTBIOME_H
