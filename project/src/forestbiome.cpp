#include "forestbiome.h"

#include "biomemanager.h"
#include "terrainfeature.h"

ForestBiome::ForestBiome(BiomeManager *b) : Biome(b, "forest"){}

// tree range is .8
inline float getTreeValue(const BiomeGenInfo &_info){
    float a = _info.height - (int)_info.height;
    float b = 17*_info.humidity - (int)(17*_info.humidity);
    float c = 31*_info.temperature - (int)(31*_info.temperature);
    return (a+b+c)/3;
}

// temp height humid
unsigned char ForestBiome::generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary){
    float _height = _info.height;
    
    float _treeHeight = ((int)_info.humidity*7)%5 + 4;
    
    if (_coordinates.y >= (int)_height && _coordinates.y < _height + _treeHeight){
        if (_coordinates.y == (int)_height){
            return GRASS;
        }
    } else if (_coordinates.y < _height){
        return DIRT;
    }
    
    return AIR;
}

TerrainFeature* ForestBiome::getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info){
    
    float _featureValue = getTreeValue(_info);
    
    if (_featureValue > .65){
        if (_info.height > 32){
            return bm->features.at("tree")->clone(_pos, &_info);
        }
    }

    return NULL;
}

