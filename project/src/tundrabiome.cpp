#include "tundrabiome.h"

#include "terrainfeature.h"

TundraBiome::TundraBiome(BiomeManager *b) : Biome(b, "tundra"){}

// tree range is .8
inline float getNoiseValue(const BiomeGenInfo &_info){
    float a = _info.height - (int)_info.height;
    float b = 17*_info.humidity - (int)(17*_info.humidity);
    float c = 31*_info.temperature - (int)(31*_info.temperature);
    return (a+b+c)/3;
}


unsigned char TundraBiome::generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary){
    float _snowValue = getNoiseValue(_info);
    
    if (_snowValue > 0.8 && (_coordinates.y == (int)_info.height || _coordinates.y == (int)_info.height+1)){
        return SNOWGRASS;
    }
    
    if (_coordinates.y == (int)_info.height){
        
        return SNOWGRASS;
        
    } else if (_coordinates.y < _info.height){
        return DIRT;
    }
    
    return AIR;
}

TerrainFeature* TundraBiome::getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info){
    float _featureValue = getNoiseValue(_info);
    
    if (_featureValue > .9){
        if (_info.height > 32){
            return bm->features.at("tree")->clone(_pos, &_info);
        }
    }

    return NULL;
}

