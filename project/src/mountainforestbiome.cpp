#include "mountainforestbiome.h"

#include "biomemanager.h"
#include "terrainfeature.h"

MountainForestBiome::MountainForestBiome(BiomeManager *b) : Biome(b, "mountain forest"){}

inline float getTreeValue(const BiomeGenInfo &_info){
    float a = _info.height - (int)_info.height;
    float b = 17*_info.humidity - (int)(17*_info.humidity);
    float c = 31*_info.temperature - (int)(31*_info.temperature);
    return (a+b+c)/3;
}

// temp height humid
unsigned char MountainForestBiome::generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary){
    float _height = _info.height;
    
    float _grassLimit = .65;
    float _dirtLimit = .45;
    float _stoneLimit = .25;
    float _treeValue = getTreeValue(_info);
    
    _height += _treeValue/2;
    
    if (_coordinates.y == (int)_height){
        if (_treeValue > _grassLimit){
            return GRASS;
        }
        if (_treeValue > _dirtLimit){
            return DIRT;
        }
        if (_treeValue > _stoneLimit){
            return STONE;
        }
        
    } else if (_coordinates.y < _height){
        return STONE;
    }
    
    return AIR;
}

TerrainFeature* MountainForestBiome::getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info){
    
    float _featureValue = getTreeValue(_info);
    
    if (_featureValue > .7){
        if (_info.height > 32){
            return bm->features.at("tree")->clone(_pos, &_info);
        }
    }

    return NULL;
}
