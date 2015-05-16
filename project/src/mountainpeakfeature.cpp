#include "mountainpeakfeature.h"

#include "biomemanager.h"
#include "biometile.h"

MountainPeakFeature::MountainPeakFeature() : TerrainFeature("mountain peak"){}

MountainPeakFeature* MountainPeakFeature::clone(Vector3 _pos, BiomeGenInfo *_baseInfo){
    MountainPeakFeature *_peak = new MountainPeakFeature();
    
    _peak->posCorner = _pos;
    _peak->type = type;
    _peak->baseInfo = _baseInfo;
   
    float _x = 80 + 40*sin(_baseInfo->humidity);
    float _y = 160 + 60*sin(2*_baseInfo->height);
    float _z = 80 + 40*sin(_baseInfo->temperature);
    Vector3 _baseSize = Vector3(_x, _y, _z);
    float _factor = (1 + .5*sin(13*_baseInfo->humidity));
    
    float _massivePeakChance = fabs(sin(97*_baseInfo->height + 103*_baseInfo->humidity + 105*_baseInfo->temperature));
    if (_massivePeakChance < .3){
        _factor += .3;
    }
    
    Vector3 _cloneSize = _baseSize * _factor;
    if (_cloneSize.x > BiomeTile::tileSize - 2){
//        cout << "x too big: " << _cloneSize.x << endl;
        _cloneSize.x = BiomeTile::tileSize - 2;
    }
    if (_cloneSize.z > BiomeTile::tileSize - 2){
//        cout << "z too big: " << _cloneSize.z << endl;
        _cloneSize.z = BiomeTile::tileSize - 2;
    }
    _peak->updateSize(_cloneSize, 4+_factor);
    
    return _peak;
}

inline float getNoiseValue(const BiomeGenInfo &_info){
    float a = _info.height - (int)_info.height;
    float b = 17*_info.humidity - (int)(17*_info.humidity);
    float c = 31*_info.temperature - (int)(31*_info.temperature);
    return (a+b+c)/3;
}

unsigned char MountainPeakFeature::generateBlock(Vector3 _coordinates, BiomeGenInfo *_info){
    // choosing not to check y because this is affecting the feature's entire column
//    if (_address.y < posCorner.y || _address.y > posCorner.y + size.y) return 251; // will happen a lot since feature map is 2D
    if (_coordinates.x < posCorner.x || _coordinates.x > posCorner.x + size.x || _coordinates.z < posCorner.z || _coordinates.z > posCorner.z + size.z){ // should not happen given feature map
        cerr << "--Terrain Feature Error: \""<<type<<"\" requested for point "<<_coordinates<<" within "<<posCorner<<size<<"."<<endl;
        return 251;
    }
    
    Vector3 _center = posCorner+size/2;
    float _height = _info->primaryBlockHeight;

    _height = _height + _info->percentageToSecondary * size.y;
    _height += 2*getNoiseValue(*_info);
    
    float _borderBlend = MAX(fabs(_coordinates.x - _center.x)/(size.x/2), fabs(_coordinates.z - _center.z)/(size.z/2));
    if (_borderBlend > 1) _borderBlend = 1;
    _borderBlend = 1 - _borderBlend;
    _height = (1-_borderBlend)*_info->primaryBlockHeight + _height*_borderBlend;
    
    if (_coordinates.y <= _height){
        if (_info->temperature < 30 && _coordinates.y > (_height+_info->height)*.6+2*sin(_info->temperature)){
            return SNOWGRASS;
        }
        return STONE;
    } 
    
    return 251;
}

