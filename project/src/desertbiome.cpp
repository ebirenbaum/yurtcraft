#include "desertbiome.h"

#include "biomemanager.h"
#include "terrainfeature.h"

DesertBiome::DesertBiome(BiomeManager *b) : Biome(b, "desert"){}

// tree range is .8
inline float getNoiseValue(const BiomeGenInfo &_info){
    float a = _info.height - (int)_info.height;
    float b = 17*_info.humidity - (int)(17*_info.humidity);
    float c = 31*_info.temperature - (int)(31*_info.temperature);
    return (a+b+c)/3;
}

float DesertBiome::getBlockHeight(const Vector3 &_coordinates, BiomeGenInfo &_info){
    float _height = _info.height;
    _height += getNoiseValue(_info);
    return (int)(_height);
}

// temp height humid
unsigned char DesertBiome::generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary){
    float _height = getBlockHeight(_coordinates, _info);
    
    if (_coordinates.y == (int)_height){
        return SAND;
    } else if (_coordinates.y < _height){
        return SAND;
    }
    
    return AIR;
}

TerrainFeature* DesertBiome::getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info){
    
    float _featureValue = getNoiseValue(_info);
    
    if (_featureValue > .8){
        if (_info.height > 32){
            return bm->features.at("cactus")->clone(_pos, &_info);
        }
    }

    return NULL;
}
