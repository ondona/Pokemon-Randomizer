#include "headers/sv_randomizer_headers/sv_wilds.h"

QList<QString> choosenBiomes = {};

QString svWilds::pickRandomBiome(){
    QList<QString> possible_biomes = {"GRASS", "FOREST", "SWAMP", "LAKE", "TOWN", "MOUNTAIN", "BAMBOO", "MINE", "CAVE", "OLIVE",
                                          "UNDERGROUND", "RIVER", "ROCKY", "BEACH", "SNOW", "OSEAN", "RUINS", "FLOWER"};

    QString choice = possible_biomes[randNum.bounded(0, possible_biomes.size())];

    while(choosenBiomes.contains(choice)){
        choice = possible_biomes[randNum.bounded(0, possible_biomes.size())];
    }

    return choice;
}

QString svWilds::generateAreaList(){
    QString areaList = "\"";

    for(int i=0; i<10; i++){
        int num = randNum.bounded(1, 27);
        areaList = areaList + QString::number(num);
        if(i != 9){
            areaList = areaList +",";
        }
    }

    areaList = areaList + "\"";
    return areaList;
}

void svWilds::randomizeWilds(QMap<int, QList<int>> allowedPokemon, bool ogerponTerapagos, bool bstBalance){
    json wildsGeneration;
    json temp;

    if(bstBalance == true){
        //Do Something
    }

    wildsGeneration["values"] = json::array();
    for(unsigned long long i=1; i< 1026; i++){
        if(allowedPokemon.contains(i)){
            for(unsigned long long j =0; j<pokemonMapping["pokemons"][i]["forms"].size(); j++){
                if(i == 666 || i == 665 || i==664){
                    if(j!=18){
                        continue;
                    }
                }

                if(ogerponTerapagos == false){
                    if(typesChanged == false){
                        if(i == 1017 && j != 0){
                            continue;
                        }else if((i == 1017 && j != 0) || i == 1024){
                            continue;
                        }
                    }else{
                        if(i == 1017 || i == 1024){
                            continue;
                        }
                    }
                }
                bool air = false;
                if(pokemonFly.contains(int(pokemonMapping["pokemons"][i]["devid"]))){
                    if(pokemonFly[int(pokemonMapping["pokemons"][i]["devid"])].contains(int(j))){
                        air = true;
                    }
                }

                bool water = false;
                if(pokemonSwim.contains(int(pokemonMapping["pokemons"][i]["devid"]))){
                    if(pokemonSwim[int(pokemonMapping["pokemons"][i]["devid"])].contains(int(j))){
                        air = true;
                    }
                }

                temp = {
                    {"devid", int(pokemonMapping["pokemons"][i]["devid"])},
                    {"sex", "DEFAULT"},
                    {"formno", int(j)},
                    {"minlevel", 2},
                    {"maxlevel", 99},
                    {"lotvalue", randNum.bounded(1, 51)},
                    {"biome1", pickRandomBiome().toStdString()},
                    {"lotvalue1", randNum.bounded(1, 51)},
                    {"biome2", pickRandomBiome().toStdString()},
                    {"lotvalue2", randNum.bounded(1, 51)},
                    {"biome3", pickRandomBiome().toStdString()},
                    {"lotvalue3", randNum.bounded(1, 51)},
                    {"biome4", pickRandomBiome().toStdString()},
                    {"lotvalue4", randNum.bounded(1, 51)},
                    {"area", generateAreaList().toStdString()},
                    {"locationName", ""},
                    {"minheight", 0},
                    {"maxheight", 255},
                    {"enabletable", {
                                        {"land", true},
                                        {"up_water", water},
                                        {"underwater", water},
                                        {"air1", air},
                                        {"air2", air}
                                    }},
                    {"timetable", {
                                      {"morning", true},
                                      {"noon", true},
                                      {"evening", true},
                                      {"night", true}
                                  }},
                    {"flagName", ""},
                    {"bandrate", 0},
                    {"bandtype", "NONE"},
                    {"bandpoke", "DEV_NULL"},
                    {"bandSex", "DEFAULT"},
                    {"bandFormno", 0},
                    {"outbreakLotvalue", 10},
                    {"pokeVoiceClassification", "ANIMAL_LITTLE"},
                    {"versiontable", {
                                         {"A", true},
                                         {"B", true}
                                     }},
                    {"bringItem", {
                                      {"itemID", getPokemonItemId(pokemonMapping["pokemons"][i]["natdex"], int(j))},
                                      {"bringRate", getPokemonItemValue(pokemonMapping["pokemons"][i]["natdex"], int(j))}
                                  }}
                };
                wildsGeneration["values"].push_back(temp);
            }
            choosenBiomes.clear();
            choosenBiomes = {};
        }
    }

    temp = {
        {"devid", 625},
        {"sex", "DEFAULT"},
        {"formno", 0},
        {"minlevel", 2},
        {"maxlevel", 99},
        {"lotvalue", 1+std::rand()%50},
        {"lotvalue", randNum.bounded(1, 51)},
        {"biome1", pickRandomBiome().toStdString()},
        {"lotvalue1", randNum.bounded(1, 51)},
        {"biome2", pickRandomBiome().toStdString()},
        {"lotvalue2", randNum.bounded(1, 51)},
        {"biome3", pickRandomBiome().toStdString()},
        {"lotvalue3", randNum.bounded(1, 51)},
        {"biome4", pickRandomBiome().toStdString()},
        {"lotvalue4", randNum.bounded(1, 51)},
        {"area", generateAreaList().toStdString()},
        {"locationName", ""},
        {"minheight", 0},
        {"maxheight", 255},
        {"enabletable", {
                            {"land", true},
                            {"up_water", false},
                            {"underwater", false},
                            {"air1", false},
                            {"air2", false}
                        }},
        {"timetable", {
                          {"morning", true},
                          {"noon", true},
                          {"evening", true},
                          {"night", true}
                      }},
        {"flagName", ""},
        {"bandrate", 100},
        {"bandtype", "BOSS"},
        {"bandpoke", "DEV_KOMATANA"},
        {"bandSex", "DEFAULT"},
        {"bandFormno", 0},
        {"outbreakLotvalue", 10},
        {"pokeVoiceClassification", "ANIMAL_LITTLE"},
        {"versiontable", {
                             {"A", true},
                             {"B", true}
                         }},
        {"bringItem", {
                          {"itemID", getPokemonItemId(625, 0)},
                          {"bringRate", getPokemonItemValue(625, 0)}
                      }}
    };

    wildsGeneration["values"].push_back(temp);

    // No parsing the wild paths - you'll have to put a different function here

    closeFileAndDelete(path,
                       schema,
                       output,
                       wildsGeneration, true);
}

void svWilds::randomize(){
    if(randomizePaldeaWild == true){
        qDebug()<<"Randomizing Paldea Wilds";
        path = "SV_WILDS/pokedata_array.json";
        schema = "SV_WILDS/pokedata_array.bfbs";
        output = "world/data/encount/pokedata/pokedata/";

        bool sizeCheck = getAllowedPokemon(paldeaWilds, usableWildPokemon, "Paldea Wilds");
        if(sizeCheck == false){
            qFatal()<<"Not Enough usable Pokemon for Paldea Wilds";
        }

        randomizeWilds(usableWildPokemon, ogerponTerapagosPaldea);
    }
    if(paldeaForAll == true){
        qDebug()<<"Using Paldea for all - Randomizing Kitakami Wilds";
        path = "SV_WILDS/pokedata_su1_array.json";
        schema = "SV_WILDS/pokedata_su1_array.bfbs";
        output = "world/data/encount/pokedata/pokedata_su1/";
        bool sizeCheck = getAllowedPokemon(paldeaWilds, usableWildPokemon, "Kitakami Wilds");
        if(sizeCheck == false){
            qFatal()<<"Not Enough usable Pokemon for Kitakami Wilds";
        }
        randomizeWilds(usableWildPokemon, ogerponTerapagosPaldea);

        qDebug()<<"Using Paldea for all - Randomizing Blueberry Wilds";
        path = "SV_WILDS/pokedata_su2_array.json";
        schema = "SV_WILDS/pokedata_su2_array.bfbs";
        output = "world/data/encount/pokedata/pokedata_su2/";
        sizeCheck = getAllowedPokemon(paldeaWilds, usableWildPokemon, "Blueberry Wilds");
        if(sizeCheck == false){
            qFatal()<<"Not Enough usable Pokemon for Blueberry Wilds";
        }
        randomizeWilds(usableWildPokemon, ogerponTerapagosPaldea);
    }else{
        if(randomizeKitakamiWild == true){
            qDebug()<<"Randomizing Kitakami Wilds";
            path = "SV_WILDS/pokedata_su1_array.json";
            schema = "SV_WILDS/pokedata_su1_array.bfbs";
            output = "world/data/encount/pokedata/pokedata_su1/";
            bool sizeCheck = getAllowedPokemon(kitakamiWilds, usableWildPokemon, "Kitakami Wilds");
            if(sizeCheck == false){
                qFatal()<<"Not Enough usable Pokemon for Kitakami Wilds";
            }
            randomizeWilds(usableWildPokemon, ogerponTerapagosKitakami);
        }

        if(randomizeBlueberryWild == true){
            qDebug()<<"Randomizing Blueberry Wilds";
            path = "SV_WILDS/pokedata_su2_array.json";
            schema = "SV_WILDS/pokedata_su2_array.bfbs";
            output = "world/data/encount/pokedata/pokedata_su2/";
            bool sizeCheck = getAllowedPokemon(blueberrWilds, usableWildPokemon, "Blueberry Wilds");
            if(sizeCheck == false){
                qFatal()<<"Not Enough usable Pokemon for Blueberry Wilds";
            }
            randomizeWilds(usableWildPokemon, ogerponTearapagosBlueberry);
        }
    }
}
