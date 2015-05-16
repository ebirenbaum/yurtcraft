#ifndef BOULDERFEATURE_H
#define BOULDERFEATURE_H

#include "terrainfeature.h"

class BoulderFeature : public TerrainFeature
{
public:
    BoulderFeature();
    
    BoulderFeature* clone(Vector3 _pos, BiomeGenInfo *_baseInfo);
    
    unsigned char generateBlock(Vector3 _address, BiomeGenInfo *_info);
    
    
};


#endif // BOULDERFEATURE_H
