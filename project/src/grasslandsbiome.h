#ifndef GRASSLANDSBIOME_H
#define GRASSLANDSBIOME_H

#include "biome.h"

class GrasslandsBiome : public Biome
{
public:
    GrasslandsBiome(BiomeManager *b);
    
    unsigned char generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary);
    TerrainFeature* getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info);
};

#endif // GRASSLANDSBIOME_H
