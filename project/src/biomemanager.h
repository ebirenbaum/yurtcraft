#ifndef BIOMEMANAGER_H
#define BIOMEMANAGER_H

#include "vector.h"
#include "map"

class Noise;
class Biome;
class TerrainFeature;

struct BiomeGenInfo {
    float temperature, height, humidity;
    string primaryBiome;
    float primaryBlockHeight;
    
    string secondaryBiome;
    float percentageToSecondary;
    
    string secondaryBlockChoice;
    float secondaryBlockHeight;
    
    BiomeGenInfo(){
    }
};
    

class BiomeManager {

public:
    BiomeManager(Noise *_perlin = NULL);

    map<string, Biome*> biomes;
    
    map<string, TerrainFeature*> features;
    
    void getPrimaryAndSecondaryBiomes(BiomeGenInfo &_info);
    
    float chooseBiomeHeight(float x, float z);
    float chooseBiomeHumidity(float x, float z);
    float chooseBiomeTemperature(float x, float z);
    
    inline double adjustRange(double input, double oldMin, double oldMax, double newMin, double newMax);
    
    Noise *biomePerlin;
    
    // ////// //
    // BIOMES //
    // ////// //
    
// original ranges    
//    float temperatureOptions[5] = 	{0, 25, 50, 75, 100};
//    float heightOptions[5]      =	{-30, 0, 15, 30, 45};
//    float humidityOptions[2]    = 	{30, 60};
    
    float temperatureOptions[5] = 	{0, 25, 50, 75, 100};
    float heightOptions[5]      =	{-30, 0, 15, 35, 55};
    float humidityOptions[2]    = 	{25, 50};
    
    static const int numTemperatureOptions = 5;
    static const int numHeightOptions = 5;
    static const int numHumidityOptions = 2;
    
    // at some point, consider switching the order of these args so that the fewest option'd (and least likely to change) dimensions are the major ones - will this help caching at all?
    
    string biomeGrid[numTemperatureOptions + 1][numHeightOptions + 1][numHumidityOptions + 1] = {
        {                               
            /* temp < 0: */
            /*					        hum < 25            25 < hum < 50           50 < hum          */
            
            /* height < -30 */   {      "tundra",          "tundra",          "tundra"      },
            /* height <     0 */ {      "desert",        "desert",        "boulder field"    },
            /* height <   30 */  {      "desert",        "tundra",           "mountain",          },
            /* height <   60 */  {      "desert",        "tundra",           "mountain",          },
            /* height <   90 */  {      "mountain",     "mountain",          "mountain",          },
            /* height > 90+ */   {      "mountain",     "mountain",     "mountain",       },
        },
        
        {                               
            /* 0 < temp < 25: */
            /*					        hum < 25            25 < hum < 50           50 < hum          */
            
            /* height < -30 */   {      "desert",           "desert",           "tundra",            },
            /* height <     0 */ {      "desert",            "tundra",           "tundra",      },
            /* height <   30 */  {      "desert",           "tundra",           "forest",           },
            /* height <   60 */  {      "boulder field",    "tundra",           "forest",           },      
            /* height <   90 */  {      "boulder field",    "mountain",         "mountain forest",  },
            /* height > 90+ */   {      "mountain",         "mountain",         "mountain",         },
        },
        
        {                               
            /* 25 < temp < 50: */
            /*					        hum < 25            25 < hum < 50           50 < hum          */
            
            /* height < -30 */   {      "tundra",           "tundra",           "tundra",          },
            /* height <     0 */ {      "desert",           "desert",           "desert",          },
            /* height <   30 */  {      "desert",            "grasslands",       "boulder field",         },
            /* height <   60 */  {      "grasslands",       "grasslands",       "boulder field",         },
            /* height <   90 */  {      "mountain",         "mountain",         "mountain forest",         },      
            /* height > 90+ */   {      "mountain",         "mountain",         "mountain",         },
        },
        
        {                               
            /* 50 < temp < 75: */
            /*					        hum < 25            25 < hum < 50           50 < hum          */
            
            /* height < -30 */   {      "boulder field",    "boulder field",    "desert",         },
            /* height <     0 */ {      "desert",            "desert",            "desert",         },
            /* height <   30 */  {      "desert",           "grasslands",           "grasslands",         },
            /* height <   60 */  {      "grasslands",           "grasslands",       "grasslands",         },
            /* height <   90 */  {      "mountain",         "mountain",         "mountain forest",         },      
            /* height > 90+ */   {      "mountain",         "mountain",         "mountain",         },  
        },
        
        {                               
            /* 75 < temp < 100: */
            /*					        hum < 25            25 < hum < 50           50 < hum          */
            
            /* height < -30 */   {      "desert",           "boulder field",    "tundra",         },
            /* height <     0 */ {      "desert",            "desert",            "grasslands",         },
            /* height <   30 */  {      "desert",           "grasslands",           "jungle",         },
            /* height <   60 */  {      "desert",           "jungle",           "jungle",         },      
            /* height <   90 */  {      "desert",          "mountain",         "mountain forest",         },
            /* height > 90+ */   {      "mountain",         "mountain forest",  "mountain",         },
        },
        
        {                               
            /* 100 < temp: */
            /*					        hum < 25            25 < hum < 50           50 < hum          */
            
            /* height < -30 */   {      "tundra",           "boulder field",    "boulder field",         },
            /* height <     0 */ {      "desert",            "desert",            "desert",         },
            /* height <   30 */  {      "desert",      "desert",           "desert",         },
            /* height <   60 */  {      "desert",       "desert",           "jungle",         },
            /* height <   90 */  {      "desert",       "mountain",         "mountain forest",         },
            /* height > 90+ */   {      "mountain",         "mountain forest",  "mountain",         },
        },
    };

};



#endif // BIOMEMANAGER_H
