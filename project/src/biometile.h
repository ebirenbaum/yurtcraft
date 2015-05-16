#ifndef BIOMETILE_H
#define BIOMETILE_H

#include "vector.h"


class TerrainFeature;
class BiomeManager;
class BiomeGenInfo;

class BiomeTile
{
public:
    BiomeTile(BiomeManager *_bm, Vector2 _address);
    ~BiomeTile();
    
    void generateBiomeInformation();
    void generateFeatureMap();
    
    BiomeGenInfo* getInfo(Vector2 _pos);
    TerrainFeature* getFeature(Vector2 _pos);
    
    
    BiomeManager *bm;
    
    Vector2 address;
    
    BiomeGenInfo *defaultInfo;
    BiomeGenInfo **biomeInfo;
    
    TerrainFeature **featureArray;
    
    const static int tileSize = 128;
    
    
    
};

#endif // BIOMETILE_H
