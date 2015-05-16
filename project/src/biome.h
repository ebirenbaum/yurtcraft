#ifndef BIOME_H
#define BIOME_H

#include "vector.h"
#include "biomemanager.h"
#include "blocktype.h"

class VoxelManager;


class Biome
{
public:
    Biome(BiomeManager *b, std::string _name);
    
    /* temp height humidity */
    virtual unsigned char generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary) = 0;
    virtual float getBlockHeight(const Vector3 &_coordinates, BiomeGenInfo &_info);
    
    virtual TerrainFeature* getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info);
    
public:
    string name;
    
    BiomeManager *bm;
    
    
};

#endif // BIOME_H
