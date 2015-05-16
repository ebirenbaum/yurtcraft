#include "biometile.h"

#include "terrainfeature.h"
#include "biomemanager.h"
#include "biome.h"
#include "chunk.h"
#include <queue>

BiomeTile::BiomeTile(BiomeManager *_bm, Vector2 _address){
//    cout << "creating biome tile " << _address << endl;
    bm = _bm;
    address = _address;
    
    int _tileArraySize = tileSize*tileSize;
    
    biomeInfo = new BiomeGenInfo*[_tileArraySize];
    defaultInfo = new BiomeGenInfo();
    defaultInfo->primaryBiome = "undefined";
    defaultInfo->primaryBlockHeight = 40;
    
    featureArray = new TerrainFeature*[_tileArraySize];
    
    for (int i = 0; i < _tileArraySize; i++){
        featureArray[i] = NULL;
    }
    
    generateBiomeInformation();
    generateFeatureMap();
}



BiomeTile::~BiomeTile(){
    for (int i = 0; i < tileSize*tileSize; i++){
        delete biomeInfo[i];
    }
    delete[] featureArray;
}



class CompareFeaturePriority {
public:
    bool operator()(TerrainFeature *_1, TerrainFeature *_2){ // defines '<' (and pq sorts largest to smallest), so to sort for closest chunk to player, return true if c2 is closer than c1
        if (_1->priority > _2->priority){
            return true;
        }
        return false;
    }
};


BiomeGenInfo* BiomeTile::getInfo(Vector2 _pos){
    Vector2 _index = (_pos - address*tileSize).floor();
    
    if (_index.x < 0 || _index.x >= tileSize ||
            _index.y < 0 || _index.y >= tileSize){
        return defaultInfo;
    }
    
    int _rawIndex = _index.x*tileSize + _index.y;
    return biomeInfo[_rawIndex];
    
}

void BiomeTile::generateBiomeInformation(){    
    Vector2 _start = address*tileSize;
    
    for (int ix = 0; ix < tileSize; ix++){
        for (int iz = 0; iz < tileSize; iz++){
            BiomeGenInfo *_biomeInfo = new BiomeGenInfo();
            double x = _start.x + ix;
            double z = _start.y + iz;
            
            _biomeInfo->temperature = bm->chooseBiomeTemperature(x, z);
            _biomeInfo->height = bm->chooseBiomeHeight(x, z);
            _biomeInfo->humidity = bm->chooseBiomeHumidity(x, z);
            
            // adjust height value for randomization
            float randomnessVal = 0
                    + sin(x/25)*5 + sin(z/25)*5
                    + sin(.17*_biomeInfo->temperature + .23*_biomeInfo->height + .19*_biomeInfo->humidity)
                    ;
            
            // I think this shit works well
            float _sinTempOffset = 1.8*sin(randomnessVal) + 1.7*sin(1.7319);
            float _sinHeightOffset = 1.3*sin(randomnessVal) + 1.3*sin(1.345751311*randomnessVal);
            float _sinHumidityOffset = 3.3*sin(randomnessVal) + 4.3*sin(1.345751311*randomnessVal);
            
            _biomeInfo->height += _sinHeightOffset;
            _biomeInfo->temperature += _sinTempOffset;
            _biomeInfo->humidity += _sinHumidityOffset;
            
            bm->getPrimaryAndSecondaryBiomes(*_biomeInfo);
            
            if (bm->biomes.find(_biomeInfo->primaryBiome) != bm->biomes.end()){
                _biomeInfo->primaryBlockHeight = bm->biomes.at(_biomeInfo->primaryBiome)->getBlockHeight(Vector3(x, _biomeInfo->height, z), *_biomeInfo);
            }
            
            int _biomeInfoIndex = ix*tileSize + iz;
            biomeInfo[_biomeInfoIndex] = _biomeInfo;
        }
    }
}

void BiomeTile::generateFeatureMap(){
    priority_queue<TerrainFeature*, vector<TerrainFeature*>, CompareFeaturePriority> _potentialFeatureQueue;
    
    Vector2 _start = Vector2(address.x*tileSize, address.y*tileSize);
    int _x, _z;
    for (int _ix = 0; _ix < tileSize; _ix++){
        for (int _iz = 0; _iz < tileSize; _iz++){
            int _biomeInfoIndex = _ix*tileSize + _iz;
            _x = _ix + _start.x;
            _z = _iz + _start.y;
            if (bm->biomes.find(biomeInfo[_biomeInfoIndex]->primaryBiome) != bm->biomes.end()){
                TerrainFeature *_feature = bm->biomes.at(biomeInfo[_biomeInfoIndex]->primaryBiome)->getPotentialBlockFeature(Vector3(_x, biomeInfo[_biomeInfoIndex]->primaryBlockHeight, _z), *(biomeInfo[_biomeInfoIndex]));
                if (_feature != NULL){
                    if (_feature->fitsWithinBiomeTile(address)){
                        _potentialFeatureQueue.push(_feature);
                    }
                }
            }
        }
    }
    while (!_potentialFeatureQueue.empty()){
        TerrainFeature *_largest = _potentialFeatureQueue.top();
        bool _blocked = false;
        for (int i = _largest->posCorner.x - _start.x; i < _largest->posCorner.x - _start.x + _largest->size.x; i++){
            for (int j = _largest->posCorner.z - _start.y; j < _largest->posCorner.z - _start.y + _largest->size.z; j++){
                
                if (featureArray[i * BiomeTile::tileSize + j] != NULL){
                    _blocked = true;
                    break;
                }
            }
            if (_blocked) break;
        }

        if (!_blocked){
            _largest->drawToFeatureMap(address, featureArray);
        }
        _potentialFeatureQueue.pop();
    }
}

TerrainFeature* BiomeTile::getFeature(Vector2 _pos){
    Vector2 _coordinates = (_pos - address*tileSize).floor();
    int _index = (int)(_coordinates.x*tileSize + _coordinates.y);
    
    return featureArray[_index];
}
