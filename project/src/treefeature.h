#ifndef TREEFEATURE_H
#define TREEFEATURE_H

#include "vector.h"

#include "terrainfeature.h"

class TreeFeature : public TerrainFeature
{
public:
    TreeFeature();
    
    TreeFeature* clone(Vector3 _pos, BiomeGenInfo *_baseInfo);
    
    unsigned char generateBlock(Vector3 _address, BiomeGenInfo *_info);
    
    
};

#endif // TREEFEATURE_H
