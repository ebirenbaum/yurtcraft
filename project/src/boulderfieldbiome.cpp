#include "boulderfieldbiome.h"

#include "terrainfeature.h"

BoulderFieldBiome::BoulderFieldBiome(BiomeManager *b) : Biome(b, "boulder field"){

}


// tree range is .8
inline float getNoiseValue(const BiomeGenInfo &_info){
    float a = 13*_info.height - (int)(13*_info.height);
    float b = 17*_info.humidity - (int)(17*_info.humidity);
    float c = 31*_info.temperature - (int)(31*_info.temperature);
    return (a+b+c)/3;
}



unsigned char BoulderFieldBiome::generateBlock(const Vector3 & _coordinates, BiomeGenInfo &_info, bool _isPrimary){
    
    if (_coordinates.y >= _info.height-1){
        float _boulderValue = getNoiseValue(_info);
        
        float _above = _coordinates.y - _info.height; // = number of blocks above ground height
        
        if (_boulderValue > .8 && _above > 0 && _above < 1.3){
            return STONE;
        }
        
    }
    
    if (_coordinates.y == (int)_info.height){
        return GRASS;
        
    } else if (_coordinates.y < _info.height) {
        return DIRT;
    }
    
    return AIR;
}


TerrainFeature* BoulderFieldBiome::getPotentialBlockFeature(Vector3 _pos, BiomeGenInfo _info){
    
    float _featureValue = getNoiseValue(_info);
    
    if (_featureValue > .9){
        if (_info.height > 32){
            return bm->features.at("tree")->clone(_pos, &_info);
        }
    } else if (_featureValue > .75){
        return bm->features.at("boulder")->clone(_pos, &_info);
    }
    
    //    else if (_featureValue > .85){
    //        return bm->features.value("brownClayBlob")->clone(_pos, &_info);

    
    return NULL;
}
