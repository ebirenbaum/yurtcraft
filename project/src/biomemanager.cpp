#include "biomemanager.h"

#include "noise.h"
#include "terrainfeature.h"

//#include "plainsbiome.h"
#include "grasslandsbiome.h"
#include "desertbiome.h"
#include "boulderfieldbiome.h"
//#include "lakebiome.h"
//#include "oceanbiome.h"
#include "forestbiome.h"
#include "mountainbiome.h"
//#include "icelakebiome.h"
#include "mountainforestbiome.h"
//#include "swampbiome.h"
//#include "marshbiome.h"
#include "tundrabiome.h"
//#include "glacierbiome.h"
//#include "stonybeachbiome.h"
#include "junglebiome.h"
//#include "icerocksbiome.h"
//#include "mountaincreekbiome.h"
//#include "icemountainforestbiome.h"

#include "treefeature.h"
#include "cactusfeature.h"
#include "boulderfeature.h"
#include "mountainpeakfeature.h"
//#include "volcanofeature.h"
//#include "testtower.h"

BiomeManager::BiomeManager(Noise *_perlin){
    biomePerlin = _perlin;
    if (biomePerlin == NULL){
        biomePerlin = new Noise(19028371);
    }
    
//    biomes["plains"] = new PlainsBiome(this);
    biomes["grasslands"] = new GrasslandsBiome(this);
    biomes["desert"] = new DesertBiome(this);
    biomes["boulder field"] = new BoulderFieldBiome(this);
//    biomes["ocean"] = new OceanBiome(this);
//    biomes["lake"] = new LakeBiome(this);
    biomes["forest"] = new ForestBiome(this);
    biomes["jungle"] = new JungleBiome(this);
    biomes["mountain"] = new MountainBiome(this);
    biomes["mountain forest"] = new MountainForestBiome(this);
//    biomes["ice mountain forest"] = new IceMountainForestBiome(this);
//    biomes["ice lake"] = new IceLakeBiome(this);
//    biomes["marsh"] = new MarshBiome(this);
//    biomes["swamp"] = new SwampBiome(this);
    biomes["tundra"] = new TundraBiome(this);
//    biomes["glacier"] = new GlacierBiome(this);
//    biomes["stony beach"] = new StonyBeachBiome(this);
//    biomes["ice rocks"] = new IceRocksBiome(this);
//    biomes["mountain creek"] = new MountainCreekBiome(this);
    
    
//    features["massiveFeature"] = new TerrainFeature("note_block", Vector3(50, 10, 50), 50));
    features["tree"] = new TreeFeature();
    features["cactus"] = new CactusFeature();
    features["boulder"] = new BoulderFeature();
    features["mountainPeak"] = new MountainPeakFeature();
//    features["volcano"] = new VolcanoFeature());
//    features["testTower"] = new TestTower());
    
}

void BiomeManager::getPrimaryAndSecondaryBiomes(BiomeGenInfo &_info){
    // 0 1  2  3  4   5
    //  0 25 50 75 100
    //i 0 1  2  3  4
    
    int _primaryTempIndex = -1;
    int _secondaryTempIndex = -1;
    float _percentageToSecondaryTemp = 0;
    for (int i = 0; i < numTemperatureOptions; i++){
        if (_info.temperature < temperatureOptions[i]){
            _primaryTempIndex = i;
            
            if (i == 0){
                _secondaryTempIndex = 1;
                float _tempRange = temperatureOptions[i+1] - temperatureOptions[i];
                _percentageToSecondaryTemp = (temperatureOptions[i] - _info.temperature)/_tempRange;                
            } else {    
                float _tempRange = temperatureOptions[i] - temperatureOptions[i-1];
                float _percentageToUpper = (temperatureOptions[i] - _info.temperature)/_tempRange;
                float _percentageToLower = (_info.temperature - temperatureOptions[i-1])/_tempRange;
        
                if (_percentageToLower < _percentageToUpper){
                    _secondaryTempIndex = i-1;
                    _percentageToSecondaryTemp = _percentageToLower;
                } else {
                    _secondaryTempIndex = i+1;
                    _percentageToSecondaryTemp = _percentageToUpper;
                }
            }
            
            break; // found less than range
        } else if (i == numTemperatureOptions - 1){ // above highest number
            _primaryTempIndex = i+1;
            _secondaryTempIndex = i;
            float _tempRange = temperatureOptions[i] - temperatureOptions[i-1];
            _percentageToSecondaryTemp = (_info.temperature - temperatureOptions[i])/_tempRange;
            
            break; // found greater than range
        }
    }
    
    int _primaryHeightIndex = 1;
    int _secondaryHeightIndex = 1;
    float _percentageToSecondaryHeight = 0.5;
    for (int i = 0; i < numHeightOptions; i++){
        if (_info.height < heightOptions[i]){
            _primaryHeightIndex = i;
            
            if (i == 0){
                _secondaryHeightIndex = 1;
                float _heightRange = heightOptions[i+1] - heightOptions[i];
                _percentageToSecondaryHeight = (heightOptions[i] - _info.height)/_heightRange;                
            } else {    
                float _heightRange = heightOptions[i] - heightOptions[i-1];
                float _percentageToUpper = (heightOptions[i] - _info.height)/_heightRange;
                float _percentageToLower = (_info.height - heightOptions[i-1])/_heightRange;
                
                if (_percentageToLower < _percentageToUpper){
                    _secondaryHeightIndex = i-1;
                    _percentageToSecondaryHeight = _percentageToLower;
                } else {
                    _secondaryHeightIndex = i+1;
                    _percentageToSecondaryHeight = _percentageToUpper;
                }
            }
            
            break; // found less than range
        } else if (i == numHeightOptions - 1){ // above highest number
            _primaryHeightIndex = i+1;
            _secondaryHeightIndex = i;
            float _heightRange = heightOptions[i] - heightOptions[i-1];
            _percentageToSecondaryHeight = (_info.height - heightOptions[i])/_heightRange;
            
            break; // found greater than range
        }
    }
    
    
    int _primaryHumidityIndex = 1;
    int _secondaryHumidityIndex = 1;
    float _percentageToSecondaryHumidity = 0.5;
    for (int i = 0; i < numHumidityOptions; i++){
        if (_info.humidity < humidityOptions[i]){
            _primaryHumidityIndex = i;
            
            if (i == 0){
                _secondaryHumidityIndex = 1;
                float _humidityRange = humidityOptions[i+1] - humidityOptions[i];
                _percentageToSecondaryHumidity = (humidityOptions[i] - _info.humidity)/_humidityRange;                
            } else {    
                float _humidityRange = humidityOptions[i] - humidityOptions[i-1];
                float _percentageToUpper = (humidityOptions[i] - _info.humidity)/_humidityRange;
                float _percentageToLower = (_info.humidity - humidityOptions[i-1])/_humidityRange;
                
                if (_percentageToLower < _percentageToUpper){
                    _secondaryHumidityIndex = i-1;
                    _percentageToSecondaryHumidity = _percentageToLower;
                } else {
                    _secondaryHumidityIndex = i+1;
                    _percentageToSecondaryHumidity = _percentageToUpper;
                }
            }
            
            break; // found less than range
        } else if (i == numHumidityOptions - 1){ // above highest number
            _primaryHumidityIndex = i+1;
            _secondaryHumidityIndex = i;
            float _humidityRange = humidityOptions[i] - humidityOptions[i-1];
            _percentageToSecondaryHumidity = (_info.humidity - humidityOptions[i])/_humidityRange;
            
            break; // found greater than range
        }
    }
    
    
    string _secondaryBiomeType;
    float _percentageToSecondaryBiome;
    if (_percentageToSecondaryTemp < _percentageToSecondaryHeight && _percentageToSecondaryTemp < _percentageToSecondaryHumidity){
        _percentageToSecondaryBiome = _percentageToSecondaryTemp;
        _secondaryBiomeType = biomeGrid[_secondaryTempIndex][_primaryHeightIndex][_primaryHumidityIndex];
    } else if (_percentageToSecondaryHeight < _percentageToSecondaryHumidity && _percentageToSecondaryHeight < _percentageToSecondaryTemp){
        _percentageToSecondaryBiome = _percentageToSecondaryHeight;
        _secondaryBiomeType = biomeGrid[_primaryTempIndex][_secondaryHeightIndex][_primaryHumidityIndex];        
    } else {
        _percentageToSecondaryBiome = _percentageToSecondaryHumidity;
        _secondaryBiomeType = biomeGrid[_primaryTempIndex][_primaryHeightIndex][_secondaryHumidityIndex];        
    }
    
    string _biomeType = biomeGrid[_primaryTempIndex][_primaryHeightIndex][_primaryHumidityIndex];
    
    _info.primaryBiome = _biomeType;
    _info.secondaryBiome = _secondaryBiomeType;
    _info.percentageToSecondary = _percentageToSecondaryBiome;    
}





float BiomeManager::chooseBiomeHeight(float x, float z){
    float _stretch = 60;
    
    float xs = x/_stretch;
    float zs = z/_stretch;
    
    float _height = biomePerlin->perlin2(Vector2(xs, zs), 1)/2 + sin(xs+zs)*.5 + sin(xs-zs)*.5;
    
    _height = adjustRange(_height, -2, 2, -30, 80);
    
    return _height;
}

float BiomeManager::chooseBiomeHumidity(float x, float z){
    float _stretch = 130;
    
    float xs = x/_stretch;
    float zs = z/_stretch;
    
    float _humidity = biomePerlin->perlin2(Vector2(xs, zs), 1)/2 + sin(xs+zs)*.5 + sin(xs-zs)*.5;
    
    _humidity = adjustRange(_humidity, -2, 2, 0, 100);
    
    return _humidity;
}


// only use the general 'biome height' height modifier to influence biome choice, so that after it
// gets randomized somewhat it won't produce clearcut lines of biome change on the surface
float BiomeManager::chooseBiomeTemperature(float x, float z){ 
    
    float _stretch = 120;
    
    float xs = x/_stretch;
    float zs = z/_stretch;
    
    float _temperature = biomePerlin->perlin2(Vector2(xs, zs), 1)/2 + sin(xs+zs)*.5 + sin(xs-zs)*.5;
    
    _temperature = adjustRange(_temperature, -2.2, 2.2, -50, 140);
    
    return _temperature;
}



// Utilities

inline double BiomeManager::adjustRange(double input, double oldMin, double oldMax, double newMin, double newMax){
    double oldRange = oldMax-oldMin;
    double newRange = newMax-newMin;
    double val = newRange*((input-oldMin)/oldRange) + newMin;
    if (val < newMin) val = newMin;
    if (val > newMax) val = newMax;
    return val;
}
