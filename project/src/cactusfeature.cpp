#include "cactusfeature.h"

#include "biomemanager.h"

CactusFeature::CactusFeature() : TerrainFeature("tree", Vector3(3, 7, 3), 5){}


CactusFeature* CactusFeature::clone(Vector3 _pos, BiomeGenInfo *_baseInfo){
    CactusFeature *_cactus = new CactusFeature();
    _cactus->posCorner = _pos;
    _cactus->type = type;
    _cactus->baseInfo = _baseInfo;
    
    float _factor = 1 + (.3 + .3*sin(_baseInfo->humidity + _baseInfo->height));
    _cactus->size = (size*_factor).floor();
    _cactus->size.y += (int)(2*sin(_baseInfo->humidity) + 1);
        
    _cactus->splitHeight = (int)(_cactus->size.y/2 + (1+sin(_baseInfo->height)));
    _cactus->facingX = (sin(_baseInfo->temperature) > 0);
    
    return _cactus;
}



unsigned char CactusFeature::generateBlock(Vector3 _address, BiomeGenInfo *_info){
    if (_address.y < posCorner.y || _address.y > posCorner.y + size.y) return 251; // will happen a lot since feature map is 2D
    if (_address.x < posCorner.x || _address.x > posCorner.x + size.x || _address.z < posCorner.z || _address.z > posCorner.z + size.z){ // should not happen given feature map
        cerr << "--Terrain Feature Error: \""<<type<<"\" requested for point "<<_address<<" within "<<posCorner<<size<<"."<<endl;
        return 251;
    }
    
    unsigned char _blockTexture = LEAVES;
    
    Vector3 _relative = _address - posCorner;
    if (_relative.y < splitHeight){
        if (_relative.x == (int)(size.x/2) && _relative.z == (int)(size.z/2)){ // main trunk
            return _blockTexture;
        }
    } else if (_relative.y == splitHeight){
        if ((facingX && _relative.x == (int)(size.x/2)) ||
                (!facingX && _relative.z == (int)(size.z/2))){
            return _blockTexture;
        }
        return 251;
    } else { //if (_relative.y > splitHeight){
        Vector3 _mask;
        if (facingX) _mask = Vector3(0, 0, (int)(size.x/2));
        else _mask = Vector3((int)(size.z/2), 0, 0);
        Vector3 _center = (size/2).floor();
        if ((_relative.x == (_center+_mask).x || _relative.x == (_center-_mask).x) &&
            (_relative.z == (_center+_mask).z || _relative.z == (_center-_mask).z)){
            
            if (size.y - _relative.y <= (2*sin(_info->temperature)+1)){ // 'randomly' cut off the branches
                return 251;
            }
            return _blockTexture;
        }
    }
    
    return 251;
}
    
    
