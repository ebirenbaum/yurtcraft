#include "biome.h"

#include "terrainfeature.h"

Biome::Biome(BiomeManager *b, string _name){
    name = _name;
    bm = b;
}



float Biome::getBlockHeight(const Vector3 &_coordinates, BiomeGenInfo &_info){
    return _info.height;
}

inline float getFeatureValue(const BiomeGenInfo &_info){
    float a = _info.height - (int)_info.height;
    float b = 19*_info.humidity - (int)(19*_info.humidity);
    float c = 37*_info.temperature - (int)(37*_info.temperature);
    return (a+b+c)/3;
}

TerrainFeature* Biome::getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info){
    
    float _featureValue = getFeatureValue(_info);
    
    if (_featureValue > .9){ // .97
        return bm->features.at("boulder")->clone(_pos, &_info);
    } else if (_featureValue > .65){
        if (_info.height > 32){
            return bm->features.at("tree")->clone(_pos, &_info);
        }
    }

    return NULL;
}
