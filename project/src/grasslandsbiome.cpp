#include "grasslandsbiome.h"

#include "terrainfeature.h"

GrasslandsBiome::GrasslandsBiome(BiomeManager *b) : Biome(b, "grasslands"){}


unsigned char GrasslandsBiome::generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary){
    
    float _height = _info.height;
    
    if (_height > 40){
        _height = 40 + pow(_height-40, .25);
    }
    
    if (_coordinates.y == (int)_info.height){
        if (_info.humidity < 30) return SAND;
        
        return GRASS;
    } else if (_coordinates.y < _info.height) {
        return DIRT;
    }
    
    return AIR;
}


// tree range is .8
inline float getTreeValue(const BiomeGenInfo &_info){
    float a = _info.height - (int)_info.height;
    float b = 17*_info.humidity - (int)(17*_info.humidity);
    float c = 31*_info.temperature - (int)(31*_info.temperature);
    return (a+b+c)/3;
}

TerrainFeature* GrasslandsBiome::getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info){
    float _featureValue = getTreeValue(_info);
    if (_featureValue > .95){
        return bm->features.at("boulder")->clone(_pos, &_info);
    } else if (_featureValue > .92){
        if (_info.height > 32){
            return bm->features.at("tree")->clone(_pos, &_info);
        }
    }

    return NULL;
}
