#ifndef CACTUSFEATURE_H
#define CACTUSFEATURE_H

#include "vector.h"

#include "terrainfeature.h"

class CactusFeature : public TerrainFeature
{
public:
    CactusFeature();
    
    CactusFeature* clone(Vector3 _pos, BiomeGenInfo *_baseInfo);
    
    unsigned char generateBlock(Vector3 _address, BiomeGenInfo *_info);

    int splitHeight;
    bool facingX;
    
};

#endif // CACTUSFEATURE_H
