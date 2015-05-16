#include "treefeature.h"

#include "biomemanager.h"

TreeFeature::TreeFeature() : TerrainFeature("tree", Vector3(3, 6, 3), 5){}


TreeFeature* TreeFeature::clone(Vector3 _pos, BiomeGenInfo *_baseInfo){
    TreeFeature *_tree = new TreeFeature();
    _tree->posCorner = _pos;
    _tree->type = type;
    _tree->baseInfo = _baseInfo;
    
    _tree->size = size;
    _tree->size.y += (int)(2*sin(_baseInfo->humidity) + 1);
    
    if (_tree->baseInfo->primaryBiome == "jungle"){
        _tree->size *= (int)(.3*sin(_baseInfo->temperature)+1.4);
    }
        
    return _tree;
}



unsigned char TreeFeature::generateBlock(Vector3 _address, BiomeGenInfo *_info){
    if (_address.y < posCorner.y || _address.y > posCorner.y + size.y) return 251; // will happen a lot since feature map is 2D
    if (_address.x < posCorner.x || _address.x > posCorner.x + size.x || _address.z < posCorner.z || _address.z > posCorner.z + size.z){ // should not happen given feature map
        cerr << "--Terrain Feature Error: \""<<type<<"\" requested for point "<<_address<<" within "<<posCorner<<size<<"."<<endl;
        return 251;
    }
    
    unsigned char _logTexture = WOOD;
    unsigned char _leafTexture = LEAVES;
//    if (_info->primaryBiome == "jungle"){
//        _logTexture = "spruce_log";
//    }
    
    Vector3 _relative = _address - posCorner;
    if (_relative.x == (int)(size.x/2) && _relative.z == (int)(size.z/2) && _relative.y < size.y-1){
        return _logTexture;
    }
    if (_relative.y > size.y/2+1){
        if (_relative.y < size.y-1 || (_relative.x > 0 && _relative.x < size.x-1 && _relative.z > 0 && _relative.z < size.z-1)){
            return _leafTexture;
        }
    } else if (_relative.y > size.y/2 && fabs(_relative.x - (int)size.x/2) > 1 && fabs(_relative.z - (int)size.z/2) > 1){
        return _leafTexture;   
    }
    
    return 251;
}

