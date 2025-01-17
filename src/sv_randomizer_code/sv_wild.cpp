#include "thirdparty/nlohmann/json.hpp"
#include "headers/sv_randomizer_headers/sv_wild_static.h"
#include <QDebug>
#include <fstream>
#include <QDir>

using json = nlohmann::ordered_json;
namespace fs = std::filesystem;

QList<std::string> choosenBiomes = {};

std::string pick_random_biome(){
    QList<std::string> possible_biomes = {"GRASS", "FOREST", "SWAMP", "LAKE", "TOWN", "MOUNTAIN", "BAMBOO", "MINE", "CAVE", "OLIVE",
                                          "UNDERGROUND", "RIVER", "ROCKY", "BEACH", "SNOW", "OSEAN", "RUINS", "FLOWER"};

    std::string choice = possible_biomes[std::rand()%possible_biomes.size()];

    while(choosenBiomes.contains(choice)){
        choice = possible_biomes[std::rand()%possible_biomes.size()];
    }

    return possible_biomes[std::rand()%possible_biomes.size()];
}

std::string generate_area_list(){
    std::string areaList = "\"";

    for(int i=0; i<10; i++){
        int num = 1+std::rand()%26;
        areaList = areaList + std::to_string(num);
        if(i != 9){
            areaList = areaList +",";
        }
    }

    areaList = areaList + "\"";
    return areaList;
}

bool SVWilds::randomize_paldea_wilds(std::string fileName){
    json wildPokemonInfo;
    json paldeaWildJSON;
    json wildTemplate;

    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("pokemon_mapping.json").toStdString());
    // Load the JSON file

    if(!file.is_open()){
        qDebug()<<"Error opening pokemon_mapping.json";
    }

    file >> wildPokemonInfo;
    file.close();

    paldeaWildJSON["values"] = json::array();
    /*QList<int> wildPaldeanPokemon = {};
    QList<int> wildLegendMons_Useless = {};
    getUsablePokemon(generation_paldea_wild, false, false, false, wildPaldeanPokemon, wildLegendMons_Useless);*/

    for(int i = 1; i<1026; i++){
        if(!WildPaldeaAllowed.contains(i)){
            continue;
        }

        if(paldea_ExcludeLegends == true){
            if(legends.contains(i)){
                continue;
            }
        }

        if(paldea_OnlyLegendsAndParadox){
            if(!legends_and_paradox.contains(i)){
                continue;
            }
        } else if(paldea_OnlyLegends){
            if(!legends.contains(i)){
                continue;
            }
        } else if(paldea_OnlyParadox){
            if(!paradox.contains(i)){
                continue;
            }
        }

        if(paldea_BanStage1){
            if(gen9Stage1.contains(i)){
                continue;
            }
        }

        if(paldea_BanStage2){
            if(gen9Stage2.contains(i)){
                continue;
            }
        }

        if(paldea_BanStage3){
            if(gen9Stage3.contains(i)){
                continue;
            }
        }

        if(paldea_Ban1Stage){
            if(no_evolution.contains(i)){
                continue;
            }
        }

        for(unsigned long long j =0; j<wildPokemonInfo["pokemons"][i]["forms"].size(); j++){
            if(wildPokemonInfo["pokemons"][i]["forms"][j]["is_present"] == true){

                if(i == 666 || i == 665 || i == 664){
                    if(j != 18){
                        continue;
                    }
                }
                if(let_oger_pagos_spawn == false){
                    if(types_changed == false){
                        if(i == 1017 && j != 0){
                            continue;
                        }
                        else if((i == 1017 && j != 0) || i == 1024){
                            continue;
                        }
                    }
                    else{
                        if(i == 1017 || i == 1024){
                            continue;
                        }
                    }
                }

                wildTemplate = {
                    {"devid", int(wildPokemonInfo["pokemons"][i]["devid"])},
                    {"sex", "DEFAULT"},
                    {"formno", int(j)},
                    {"minlevel", 2},
                    {"maxlevel", 99},
                    {"lotvalue", 1+std::rand()%50},
                    {"biome1", pick_random_biome()},
                    {"lotvalue1", 1+std::rand()%50},
                    {"biome2", pick_random_biome()},
                    {"lotvalue2", 1+std::rand()%50},
                    {"biome3", pick_random_biome()},
                    {"lotvalue3", 1+std::rand()%50},
                    {"biome4", pick_random_biome()},
                    {"lotvalue4", 1+std::rand()%50},
                    {"area", generate_area_list()},
                    {"locationName", ""},
                    {"minheight", 0},
                    {"maxheight", 255},
                    {"enabletable", {
                                    {"land", true},
                                    {"up_water", true},
                                    {"underwater", true},
                                    {"air1", true},
                                    {"air2", true}
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
                                   {"itemID", getPokemonItemId(wildPokemonInfo["pokemons"][i]["natdex"], int(j))},
                                  {"bringRate", getPokemonItemValue(wildPokemonInfo["pokemons"][i]["natdex"], int(j))}
                              }}
                };

                paldeaWildJSON["values"].push_back(wildTemplate);

            }
            choosenBiomes.clear();
            choosenBiomes = {};
        }
    }

    wildTemplate = {
        {"devid", 625},
        {"sex", "DEFAULT"},
        {"formno", 0},
        {"minlevel", 2},
        {"maxlevel", 99},
        {"lotvalue", 1+std::rand()%50},
        {"biome1", pick_random_biome()},
        {"lotvalue1", 1+std::rand()%50},
        {"biome2", pick_random_biome()},
        {"lotvalue2", 1+std::rand()%50},
        {"biome3", pick_random_biome()},
        {"lotvalue3", 1+std::rand()%50},
        {"biome4", pick_random_biome()},
        {"lotvalue4", 1+std::rand()%50},
        {"area", generate_area_list()},
        {"locationName", ""},
        {"minheight", 0},
        {"maxheight", 255},
        {"enabletable", {
                            {"land", true},
                            {"up_water", true},
                            {"underwater", true},
                            {"air1", true},
                            {"air2", true}
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

    paldeaWildJSON["values"].push_back(wildTemplate);

    std::ofstream fileSave(filePath+"/SV_WILDS/"+fileName);
    fileSave<<paldeaWildJSON.dump(4);
    fileSave.close();
    return true;
}

bool SVWilds::randomize_kitakami_wilds(){
    json wildPokemonInfo;
    json paldeaWildJSON;
    json wildTemplate;

    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("pokemon_mapping.json").toStdString());
    // Load the JSON file

    if(!file.is_open()){
        qDebug()<<"Error opening .json";
    }

    file >> wildPokemonInfo;
    file.close();

    paldeaWildJSON["values"] = json::array();
    QList<int> wildPaldeanPokemon = {};
    QList<int> wildLegendMons_Useless = {};
    getUsablePokemon(generation_kitakami_wild, false, false, false, wildPaldeanPokemon, wildLegendMons_Useless);

    for(int i = 1; i<1026; i++){
        if(!wildPaldeanPokemon.contains(i)){
            continue;
        }

        if(kitakami_ExcludeLegends == true){
            if(legends.contains(i)){
                continue;
            }
        }

        if(kitakami_OnlyLegendsAndParadox){
            if(!legends_and_paradox.contains(i)){
                continue;
            }
        } else if(kitakami_OnlyLegends){
            if(!legends.contains(i)){
                continue;
            }
        } else if(kitakami_OnlyParadox){
            if(!paradox.contains(i)){
                continue;
            }
        }

        if(kitakami_BanStage1){
            if(gen9Stage1.contains(i)){
                continue;
            }
        }

        if(kitakami_BanStage2){
            if(gen9Stage2.contains(i)){
                continue;
            }
        }

        if(kitakami_BanStage3){
            if(gen9Stage3.contains(i)){
                continue;
            }
        }

        if(kitakami_Ban1Stage){
            if(no_evolution.contains(i)){
                continue;
            }
        }

        for(unsigned long long j =0; j<wildPokemonInfo["pokemons"][i]["forms"].size(); j++){
            if(wildPokemonInfo["pokemons"][i]["forms"][j]["is_present"] == true){
                if(i == 666 || i == 665 || i == 664){
                    if(j != 18){
                        continue;
                    }
                }
                if(let_oger_pagos_spawn == false){
                    if(types_changed == false){
                        if(i == 1017 && j != 0){
                            continue;
                        }
                        else if((i == 1017 && j != 0) || i == 1024){
                            continue;
                        }
                    }
                    else{
                        if(i == 1017 || i == 1024){
                            continue;
                        }
                    }
                }

                wildTemplate = {
                    {"devid", int(wildPokemonInfo["pokemons"][i]["devid"])},
                    {"sex", "DEFAULT"},
                    {"formno", int(j)},
                    {"minlevel", 2},
                    {"maxlevel", 99},
                    {"lotvalue", 1+std::rand()%50},
                    {"biome1", pick_random_biome()},
                    {"lotvalue1", 1+std::rand()%50},
                    {"biome2", pick_random_biome()},
                    {"lotvalue2", 1+std::rand()%50},
                    {"biome3", pick_random_biome()},
                    {"lotvalue3", 1+std::rand()%50},
                    {"biome4", pick_random_biome()},
                    {"lotvalue4", 1+std::rand()%50},
                    {"area", generate_area_list()},
                    {"locationName", ""},
                    {"minheight", 0},
                    {"maxheight", 255},
                    {"enabletable", {
                                        {"land", true},
                                        {"up_water", true},
                                        {"underwater", true},
                                        {"air1", true},
                                        {"air2", true}
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
                                      {"itemID", getPokemonItemId(wildPokemonInfo["pokemons"][i]["natdex"], int(j))},
                                      {"bringRate", getPokemonItemValue(wildPokemonInfo["pokemons"][i]["natdex"], int(j))}
                                  }}
                };

                paldeaWildJSON["values"].push_back(wildTemplate);

            }
            choosenBiomes.clear();
            choosenBiomes = {};
        }
    }

    wildTemplate = {
        {"devid", 625},
        {"sex", "DEFAULT"},
        {"formno", 0},
        {"minlevel", 2},
        {"maxlevel", 99},
        {"lotvalue", 1+std::rand()%50},
        {"biome1", pick_random_biome()},
        {"lotvalue1", 1+std::rand()%50},
        {"biome2", pick_random_biome()},
        {"lotvalue2", 1+std::rand()%50},
        {"biome3", pick_random_biome()},
        {"lotvalue3", 1+std::rand()%50},
        {"biome4", pick_random_biome()},
        {"lotvalue4", 1+std::rand()%50},
        {"area", generate_area_list()},
        {"locationName", ""},
        {"minheight", 0},
        {"maxheight", 255},
        {"enabletable", {
                            {"land", true},
                            {"up_water", true},
                            {"underwater", true},
                            {"air1", true},
                            {"air2", true}
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

    paldeaWildJSON["values"].push_back(wildTemplate);
    std::ofstream fileSave(filePath+"/SV_WILDS/pokedata_su1_array.json");
    fileSave<<paldeaWildJSON.dump(4);
    fileSave.close();
    return true;
}

bool SVWilds::randomize_blueberry_wilds(){
    json wildPokemonInfo;
    json paldeaWildJSON;
    json wildTemplate;

    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("pokemon_mapping.json").toStdString());
    // Load the JSON file

    if(!file.is_open()){
        qDebug()<<"Error opening .json";
    }

    file >> wildPokemonInfo;
    file.close();

    paldeaWildJSON["values"] = json::array();
    QList<int> wildPaldeanPokemon = {};
    QList<int> wildLegendMons_Useless = {};
    getUsablePokemon(generation_blueberry_wild, false, false, false, wildPaldeanPokemon, wildLegendMons_Useless);

    for(int i = 1; i<1026; i++){
        if(!wildPaldeanPokemon.contains(i)){
            continue;
        }

        if(blueberry_ExcludeLegends == true){
            if(legends.contains(i)){
                continue;
            }
        }

        if(blueberry_OnlyLegendsAndParadox){
            if(!legends_and_paradox.contains(i)){
                continue;
            }
        } else if(blueberry_OnlyLegends){
            if(!legends.contains(i)){
                continue;
            }
        } else if(blueberry_OnlyParadox){
            if(!paradox.contains(i)){
                continue;
            }
        }

        if(blueberry_BanStage1){
            if(gen9Stage1.contains(i)){
                continue;
            }
        }

        if(blueberry_BanStage2){
            if(gen9Stage2.contains(i)){
                continue;
            }
        }

        if(blueberry_BanStage3){
            if(gen9Stage3.contains(i)){
                continue;
            }
        }

        if(blueberry_Ban1Stage){
            if(no_evolution.contains(i)){
                continue;
            }
        }

        for(unsigned long long j =0; j<wildPokemonInfo["pokemons"][i]["forms"].size(); j++){
            if(wildPokemonInfo["pokemons"][i]["forms"][j]["is_present"] == true){

                if(i == 666 || i == 665 || i == 664){
                    if(j != 18){
                        continue;
                    }
                }
                if(let_oger_pagos_spawn == false){
                    if(types_changed == false){
                        if(i == 1017 && j != 0){
                            continue;
                        }
                        else if((i == 1017 && j != 0) || i == 1024){
                            continue;
                        }
                    }
                    else{
                        if(i == 1017 || i == 1024){
                            continue;
                        }
                    }
                }

                wildTemplate = {
                    {"devid", int(wildPokemonInfo["pokemons"][i]["devid"])},
                    {"sex", "DEFAULT"},
                    {"formno", int(j)},
                    {"minlevel", 2},
                    {"maxlevel", 99},
                    {"lotvalue", 1+std::rand()%50},
                    {"biome1", pick_random_biome()},
                    {"lotvalue1", 1+std::rand()%50},
                    {"biome2", pick_random_biome()},
                    {"lotvalue2", 1+std::rand()%50},
                    {"biome3", pick_random_biome()},
                    {"lotvalue3", 1+std::rand()%50},
                    {"biome4", pick_random_biome()},
                    {"lotvalue4", 1+std::rand()%50},
                    {"area", generate_area_list()},
                    {"locationName", ""},
                    {"minheight", 0},
                    {"maxheight", 255},
                    {"enabletable", {
                                        {"land", true},
                                        {"up_water", true},
                                        {"underwater", true},
                                        {"air1", true},
                                        {"air2", true}
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
                                      {"itemID", getPokemonItemId(wildPokemonInfo["pokemons"][i]["natdex"], int(j))},
                                      {"bringRate", getPokemonItemValue(wildPokemonInfo["pokemons"][i]["natdex"], int(j))}
                                  }}
                };

                paldeaWildJSON["values"].push_back(wildTemplate);

            }
            choosenBiomes.clear();
            choosenBiomes = {};
        }
    }

    wildTemplate = {
        {"devid", 625},
        {"sex", "DEFAULT"},
        {"formno", 0},
        {"minlevel", 2},
        {"maxlevel", 99},
        {"lotvalue", 1+std::rand()%50},
        {"biome1", pick_random_biome()},
        {"lotvalue1", 1+std::rand()%50},
        {"biome2", pick_random_biome()},
        {"lotvalue2", 1+std::rand()%50},
        {"biome3", pick_random_biome()},
        {"lotvalue3", 1+std::rand()%50},
        {"biome4", pick_random_biome()},
        {"lotvalue4", 1+std::rand()%50},
        {"area", generate_area_list()},
        {"locationName", ""},
        {"minheight", 0},
        {"maxheight", 255},
        {"enabletable", {
                            {"land", true},
                            {"up_water", true},
                            {"underwater", true},
                            {"air1", true},
                            {"air2", true}
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

    paldeaWildJSON["values"].push_back(wildTemplate);

    std::ofstream fileSave(filePath+"/SV_WILDS/pokedata_su2_array.json");
    fileSave<<paldeaWildJSON.dump(4);
    fileSave.close();
    return true;
}

bool SVWilds::randomize_statics(){
    json wildPokemonInfo;
    json paldeaFixedJSON;

    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("pokemon_mapping.json").toStdString());
    std::ifstream fileFixed(qBaseDir.filePath("SV_WILDS/fixed_symbol_table_array_clean.json").toStdString());
    // Load the JSON file

    if(!file.is_open()){
        qDebug()<<"Error opening pokemon_mapping.json";
    }

    if(!fileFixed.is_open()){
        qDebug()<<"Error opening fixed_symbol_table_array_clean.json";
    }

    file >> wildPokemonInfo;
    file.close();
    fileFixed >> paldeaFixedJSON;
    fileFixed.close();

    for(unsigned long long i =0; i<paldeaFixedJSON["values"].size(); i++){

        int random = 1+std::rand()%1025;
        while(!FixedSymbolAllowed.contains(wildPokemonInfo["pokemons"][random]["natdex"]))
            random = 1+std::rand()%1025;

        int formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());

        if(random == 664 || random == 665 || random == 666){
            formRandom = 18;
        }

        while(wildPokemonInfo["pokemons"][random]["forms"][formRandom]["is_present"] == false){
            formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
        }

        if(getItemForPokemon(wildPokemonInfo["pokemons"][random]["natdex"], formRandom) != "ITEMID_NONE"){
            formRandom = 0;
        }

        paldeaFixedJSON["values"][i]["pokeDataSymbol"]["devId"] = wildPokemonInfo["pokemons"][random]["devName"];
        paldeaFixedJSON["values"][i]["pokeDataSymbol"]["formId"] = formRandom;
        paldeaFixedJSON["values"][i]["pokeDataSymbol"]["gemType"] = selectTeraTypes(random, formRandom);
        paldeaFixedJSON["values"][i]["pokeDataSymbol"]["wazaType"] = "DEFAULT";
        paldeaFixedJSON["values"][i]["pokeDataSymbol"]["waza1"]["wazaId"] = "WAZA_NULL";
        paldeaFixedJSON["values"][i]["pokeDataSymbol"]["waza2"]["wazaId"] = "WAZA_NULL";
        paldeaFixedJSON["values"][i]["pokeDataSymbol"]["waza3"]["wazaId"] = "WAZA_NULL";
        paldeaFixedJSON["values"][i]["pokeDataSymbol"]["waza4"]["wazaId"] = "WAZA_NULL";

    }

    std::ofstream fileSave(filePath+"/SV_WILDS/fixed_symbol_table_array.json");
    fileSave<<paldeaFixedJSON.dump(4);
    fileSave.close();
    return true;
}
