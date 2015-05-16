#ifndef TUNDRABIOME_H
#define TUNDRABIOME_H

#include "biome.h"

class TundraBiome : public Biome
{
public:
    TundraBiome(BiomeManager *b);
    
    unsigned char generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary);
    TerrainFeature* getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info);
    
};

#endif // TUNDRABIOME_H
