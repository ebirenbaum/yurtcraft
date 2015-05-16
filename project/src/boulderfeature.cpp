#include "boulderfeature.h"

#include "biomemanager.h"
#include "chunk.h"

BoulderFeature::BoulderFeature() : TerrainFeature("boulder"){}

BoulderFeature* BoulderFeature::clone(Vector3 _pos, BiomeGenInfo *_baseInfo){

    BoulderFeature *_boulder = new BoulderFeature();
    
    _boulder->posCorner = _pos;
    _boulder->type = type;
    _boulder->baseInfo = _baseInfo;
   
    Vector3 _baseSize = Vector3(2, 2, 2);
    
    float _factor = (1 + .5*sin(13*_baseInfo->humidity));
    if (_baseInfo->primaryBiome == "boulder field"){
        _factor += 1;
    }
    Vector3 _cloneSize = _baseSize * _factor;
    _boulder->updateSize(_cloneSize, 4+_factor);
    
    return _boulder;
}

unsigned char BoulderFeature::generateBlock(Vector3 _address, BiomeGenInfo *_info){
    if (_address.y < posCorner.y || _address.y > posCorner.y + size.y) return 251; // will happen a lot since feature map is 2D
    if (_address.x < posCorner.x || _address.x > posCorner.x + size.x || _address.z < posCorner.z || _address.z > posCorner.z + size.z){ // should not happen given feature map
        cerr << "--Terrain Feature Error: \""<<type<<"\" requested for point "<<_address<<" within "<<posCorner<<size<<"."<<endl;
        return 251;
    }
    Vector3 _featureCenter = posCorner + size/2;
    Vector3 _blockCenter = (_address + Vector3(.5, .5, .5));
    if (_blockCenter.distanceSquared(_featureCenter) > pow(size.x*0.5, 2)){
        return 251;
    }
    return STONE;
}

