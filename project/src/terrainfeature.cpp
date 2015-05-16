#include "terrainfeature.h"

#include "biomemanager.h"
#include "biometile.h"
#include "blocktype.h"

TerrainFeature::TerrainFeature(string _type){
    
    type = _type;
    posCorner = Vector3(0, 0, 0); // set in clone
    
    // will need to be set in updateSize before use
    priority = -1;
    size = Vector3(0, 0, 0);
}

TerrainFeature::TerrainFeature(string _type, Vector3 _size, float _priority){
    
    size = _size;
    posCorner = Vector3(0, 0, 0); // set in clone
        
    priority = _priority;
    
    type = _type;
    
}

TerrainFeature::TerrainFeature(TerrainFeature *_base, Vector3 _pos, BiomeGenInfo *_baseInfo){
    copyExisting(_base, _pos, _baseInfo);
}

void TerrainFeature::copyExisting(TerrainFeature *_base, Vector3 _pos, BiomeGenInfo *_baseInfo){
    posCorner = _pos;
    
    baseInfo = _baseInfo;
    
    size = _base->size;
    type = _base->type;
}

TerrainFeature* TerrainFeature::clone(Vector3 _pos, BiomeGenInfo *_baseInfo){
    return new TerrainFeature(this, _pos, _baseInfo);
}

TerrainFeature::~TerrainFeature(){
}

bool TerrainFeature::fitsWithinBiomeTile(Vector2 _mapCoords){
    Vector2 _mapStartPos = _mapCoords * BiomeTile::tileSize;
    
    return (!(posCorner.x < _mapStartPos.x || posCorner.x + size.x > _mapStartPos.x + BiomeTile::tileSize ||
            posCorner.z < _mapStartPos.y || posCorner.z + size.z > _mapStartPos.y + BiomeTile::tileSize));
}

// 'rasterization' step
void TerrainFeature::drawToFeatureMap(Vector2 _mapCoords, TerrainFeature **_map){
//    cout << "drawing feature "<<type << " at "<< posCorner << " in map " << _mapCoords << endl;
    Vector2 _mapStartPos = _mapCoords * BiomeTile::tileSize;
    
    if (posCorner.x < _mapStartPos.x || posCorner.x + size.x > _mapStartPos.x + BiomeTile::tileSize ||
            posCorner.z < _mapStartPos.y || posCorner.z + size.z > _mapStartPos.y + BiomeTile::tileSize){
        cerr << "--Terrain Feature Error: \""<<type<<"\" (" <<posCorner<<size<< ") would draw outside given terrain map tile: "<<_mapCoords<<" -> "<<_mapStartPos<<endl;
        return;
    }
    
    Vector2 _relPos = Vector2(posCorner.x, posCorner.z) - _mapStartPos;
    
    // copy m'pointer errywhere
    for (int i = _relPos.x; i < _relPos.x + size.x; i++){
        for (int j = _relPos.y; j < _relPos.y + size.z; j++){
            _map[i * BiomeTile::tileSize + j] = this;
        }
    }
}

unsigned char TerrainFeature::generateBlock(Vector3 _address, BiomeGenInfo *_info){
    if (_address.y < posCorner.y || _address.y > posCorner.y + size.y) return -1; // will happen a lot since feature map is 2D
    if (_address.x < posCorner.x || _address.x > posCorner.x + size.x || _address.z < posCorner.z || _address.z > posCorner.z + size.z){ // should not happen given feature map
        cerr << "--Terrain Feature Error: \""<<type<<"\" requested for point "<<_address<<" within "<<posCorner<<size<<"."<<endl;
        return -1;
    }
    return PLANK;
    
}

void TerrainFeature::updateSize(Vector3 _size, float _priority){
    size = _size;
    priority = _priority;
}



