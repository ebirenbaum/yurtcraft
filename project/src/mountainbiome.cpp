#include "mountainbiome.h"

#include "biomemanager.h"
#include "terrainfeature.h"

MountainBiome::MountainBiome(BiomeManager *b) : Biome(b, "mountain"){}

// tree range is .8
inline float getNoiseValue(const BiomeGenInfo &_info){
    float a = _info.height - (int)_info.height;
    float b = 17*_info.humidity - (int)(17*_info.humidity);
    float c = 31*_info.temperature - (int)(31*_info.temperature);
    return (a+b+c)/3;
}

inline float getNoiseValue2(const BiomeGenInfo &_info){
    float a = _info.height - (int)_info.height;
    float b = 23*_info.humidity - (int)(23*_info.humidity);
    float c = 59*_info.temperature - (int)(59*_info.temperature);
    return (a+b+c)/3;
}


float MountainBiome::getBlockHeight(const Vector3 &_coordinates, BiomeGenInfo &_info){
    float _height = _info.height;
    
    _height = _height + _info.percentageToSecondary * 30;
    
    _height += getNoiseValue(_info);
    
    return (int)(_height);
}

// temp height humid
unsigned char MountainBiome::generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary){
    int _height = getBlockHeight(_coordinates, _info);
    if (!_isPrimary){
        _info.secondaryBlockHeight = _height;
    }
    
    if (_coordinates.y <= _height){
        return STONE;
    }
    
    return AIR;
}


TerrainFeature* MountainBiome::getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info){
    
    float _featureValue = getNoiseValue(_info);
    float _featureValue2 = getNoiseValue2(_info);
    
    if (_info.height > 50){
        if (_featureValue2 > .6){//.89){
//            cout << "mountain peak at "<<_pos<<endl;
            return bm->features.at("mountainPeak")->clone(_pos, &_info);
        }
    }
    if (_info.height < 48){
        if (_featureValue > .91){
            if (_info.height > 32){
                return bm->features.at("tree")->clone(_pos, &_info);
            }
        }
    }
    if (_featureValue > .84){
        return bm->features.at("boulder")->clone(_pos, &_info);
    }
    
    return NULL;
}

