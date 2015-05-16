#ifndef JUNGLEBIOME_H
#define JUNGLEBIOME_H

#include "biome.h"

class BiomeManager;

class JungleBiome : public Biome
{
public:
    JungleBiome(BiomeManager *b);
    
    unsigned char generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary);
    TerrainFeature* getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info);

};
#endif // JUNGLEBIOME_H
