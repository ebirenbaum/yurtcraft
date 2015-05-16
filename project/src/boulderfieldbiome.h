#ifndef BOULDERFIELDBIOME_H
#define BOULDERFIELDBIOME_H

#include "biome.h"

class BoulderFieldBiome : public Biome{
public:
    BoulderFieldBiome(BiomeManager *b);
    
    unsigned char generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary);
    
    virtual TerrainFeature* getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info);
};

#endif // BOULDERFIELDBIOME_H
