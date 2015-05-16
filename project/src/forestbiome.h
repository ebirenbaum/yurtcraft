#ifndef FORESTBIOME_H
#define FORESTBIOME_H

#include "biome.h"

class BiomeManager;

class ForestBiome : public Biome
{
public:
    ForestBiome(BiomeManager *b);
    
    unsigned char generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary);
    TerrainFeature* getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info);

};

#endif // FORESTBIOME_H
