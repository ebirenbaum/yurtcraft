#ifndef TERRAINFEATURE_H
#define TERRAINFEATURE_H

#include "vector.h"
#include "blocktype.h"

class BiomeGenInfo;

class TerrainFeature
{
public:
    TerrainFeature(string _type);
    TerrainFeature(string _type, Vector3 _size, float _priority);
    TerrainFeature(TerrainFeature *_base, Vector3 _pos, BiomeGenInfo *_baseInfo);
    ~TerrainFeature();
    
    virtual TerrainFeature* clone(Vector3 _pos, BiomeGenInfo *_baseInfo);
    
    bool fitsWithinBiomeTile(Vector2 _mapCoords);
    void updateSize(Vector3 _size, float _priority);
    
    
    virtual void drawToFeatureMap(Vector2 _mapCoords, TerrainFeature **_map);
    virtual unsigned char generateBlock(Vector3 _address, BiomeGenInfo *_info);
    
    
    Vector3 posCorner;
    Vector3 size;
        
    float priority;
    
    string type;
    
    BiomeGenInfo *baseInfo;
    
protected:
    
    void copyExisting(TerrainFeature *_base, Vector3 _pos, BiomeGenInfo *_baseInfo);
    
    
};

#endif // TERRAINFEATURE_H
