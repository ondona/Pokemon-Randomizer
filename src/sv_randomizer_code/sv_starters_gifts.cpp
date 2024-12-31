#include "headers/sv_randomizer_headers/sv_starters_gifts.h"
#include <QString>
#include <cstdlib>
#include <string>
#include <QMap>
#include <QDir>
#include <QDebug>
#include <fstream>
#include "thirdparty/nlohmann/json.hpp"

// Look into QSharedData in the future
using json = nlohmann::json;
namespace fs = std::filesystem;

// Global Variables
json cleanStarterData;
json cleanGiftData;
json pokemonMaps;
std::vector<int> devId  = {};
std::vector<int> formId  = {};
std::vector<int> gender  = {};
std::vector<bool> rare  = {};

SVStarters::SVStarters()
{}


SVStarters::~SVStarters()
{}

int SVStarters::getTeraTypeInt(std::string teraType){
    for(int i =0; i<teraTypes.length(); i++){
        if(teraTypes[i].toUpper().toStdString()==teraType){
            return i;
        }
    }

    return 0;
}

void printJsonValues(const json& jsonData) {
    if (jsonData.is_object()) {
        for (auto it = jsonData.begin(); it != jsonData.end(); ++it) {
            QString key = QString::fromStdString(it.key());
            qDebug() << "Key:" << key;

            if (it->is_object()) {
                qDebug() << "Value: Object";
                printJsonValues(*it); // Recursive call for nested objects
            } else if (it->is_array()) {
                qDebug() << "Value: Array";
                for (const auto& item : *it) {
                    QString value = QString::fromStdString(item.dump());
                    qDebug() << " -" << value;
                }
            } else {
                QString value = QString::fromStdString(it->dump());
                qDebug() << "Value:" << value;
            }
            qDebug() << "--------------";
        }
    } else if (jsonData.is_array()) {
        qDebug() << "Value: Array";
        for (const auto& item : jsonData) {
            QString value = QString::fromStdString(item.dump());
            qDebug() << " -" << value << '\n';
        }
    } else {
        QString value = QString::fromStdString(jsonData.dump());
        qDebug() << "Value:" << value;
    }
}

void SVStarters::get_selected_starter(int index, QString starterName, int form, QString gender, bool shiny, QString ball){

    std::string starterNameStd = starterName.toStdString();
    std::string genderStd = gender.toStdString();

    int genderNum = 0;
    int rareNum = 0;

    if(!pokemonMaps["pokemons_name"].contains(starterName)){
        int random = 1+std::rand()%totalPokemon;
        while(!allowedPokemon.contains(pokemonMaps["pokemons"][random]["natdex"]))
            random = 1+std::rand()%totalPokemon;

        int formRandom = std::rand()%static_cast<int>(pokemonMaps["pokemons"][random]["forms"].size());
        while(pokemonMaps["pokemons"][random]["forms"][formRandom]["is_present"] == false){
            formRandom = std::rand()%static_cast<int>(pokemonMaps["pokemons"][random]["forms"].size());
        }

        devId.push_back(int(pokemonMaps["pokemons"][random]["devid"]));
        formId.push_back(formRandom);

        cleanStarterData["values"][index]["pokeData"]["devId"] = pokemonMaps["pokemons"][random]["devName"];

        // Set Starter Form
        cleanStarterData["values"][index]["pokeData"]["formId"] = formRandom;

        // Set Starter Gender
        QString form_Check = QString::fromStdString(pokemonMaps["pokemons"][random]["name"]);
        if(maleOnlyPokemon.contains(form_Check) || genderlessPokemon.contains(form_Check)){
            ::gender.push_back(0);
            genderNum = 0;
            if(genderlessPokemon.contains(form_Check)){
                genderStd = "DEFAULT";
            }else{
                genderStd = "MALE";
            }
        }else if(femaleOnlyPokemon.contains(form_Check)){
            ::gender.push_back(1);
            genderNum = 1;
            genderStd = "FEMALE";
        } else if(genderForms.contains(form_Check)){
            if(form == 0){
                ::gender.push_back(0);
                genderNum = 0;
                genderStd = "MALE";
            }else{
                ::gender.push_back(1);
                genderNum = 1;
                genderStd = "FEMALE";
            }
        }else{
            qDebug()<<"How Here";
        }
        cleanStarterData["values"][index]["pokeData"]["sex"] = genderStd;


        // Set Starter Shiny Status
        if(shiny == true){
            cleanStarterData["values"][index]["pokeData"]["rareType"] = "RARE";
            rare.push_back(shiny);
        }else{
            if(all_starters_shiny == true){
                cleanStarterData["values"][index]["pokeData"]["rareType"] = "RARE";
                rare.push_back(true);
                rareNum = 1;
            }
            else{
                int val = 1+std::rand()%shiny_starter_rate; // range is [1, shiny_starter_rate)
                if(val == 1){
                    cleanStarterData["values"][index]["pokeData"]["rareType"] = "RARE";
                    rare.push_back(true);
                    rareNum = 1;
                }
                else{
                    cleanStarterData["values"][index]["pokeData"]["rareType"] = "NO_RARE";
                    rare.push_back(false);
                    rareNum = 0;
                }
            }
        }
        cleanStarterData["values"][index]["pokeData"]["ballId"] = pokeballDevNames[ball][0].toStdString();

        if(randomize_starters_tera_types == true){
            cleanStarterData["values"][index]["pokeData"]["gemType"] = selectTeraTypes(random, formRandom);
        }

        if(set_tera_type_pokemon.contains(random))
            cleanStarterData["values"][index]["pokeData"]["gemType"] = selectTeraTypes(random, formRandom);

        QString keyVal = "";
        if(index == 0){
            keyVal = "hono";
        }else if(index == 1){
            keyVal = "kusa";
        }else if(index == 2){
            keyVal = "mizu";
        }

        cleanStarterData["values"][index]["pokeData"]["item"] = getPokemonItemId(pokemonMaps["pokemons"][random]["natdex"], formRandom);
        currectlySelectedStarters[keyVal]["id"] = pokemonMaps["pokemons"][random]["devid"];
        currectlySelectedStarters[keyVal]["form"] = formRandom;
        currectlySelectedStarters[keyVal]["gender"] = genderNum;
        currectlySelectedStarters[keyVal]["rare"] = rareNum;
        currectlySelectedStarters[keyVal]["gem"] = getTeraTypeInt(cleanStarterData["values"][index]["pokeData"]["gemType"]);

    }else{
        cleanStarterData["values"][index]["pokeData"]["devId"] = pokemonMaps["pokemons_name"][starterNameStd]["devName"];

        cleanStarterData["values"][index]["pokeData"]["formId"] = form;

        devId.push_back(int(pokemonMaps["pokemons_name"][starterNameStd]["devid"]));
        formId.push_back(form);

        // Set Starter Gender
        if(genderStd == "MALE" || genderStd == "GENDERLESS"){
            ::gender.push_back(0);
            genderNum = 0;
            if(genderStd == "GENDERLESS")
                genderStd = "DEFAULT";
        }else if(genderStd == "FEMALE"){
            ::gender.push_back(1);
            genderNum = 1;
        } else if(genderStd == "DEFAULT"){
            if(form == 0){
                ::gender.push_back(0);
                genderNum = 0;
                genderStd = "MALE";
            }else{
                ::gender.push_back(1);
                genderNum = 1;
                genderStd = "FEMALE";
            }
        }else{
            qDebug()<<"How Here - 2";
        }

        cleanStarterData["values"][index]["pokeData"]["sex"] = genderStd;

        // Stet Starter Shiny Status
        if(shiny == true){
            cleanStarterData["values"][index]["pokeData"]["rareType"] = "RARE";
            rare.push_back(true);
            rareNum = 1;
        }else{
            if(all_starters_shiny == true){
                cleanStarterData["values"][index]["pokeData"]["rareType"] = "RARE";
                rare.push_back(true);
                rareNum = 1;
            }
            else{
                int val = 1+std::rand()%shiny_starter_rate; // range is [1, shiny_starter_rate)
                if(val == 1){
                    cleanStarterData["values"][index]["pokeData"]["rareType"] = "RARE";
                    rare.push_back(true);
                    rareNum = 1;
                }
                else{
                    cleanStarterData["values"][index]["pokeData"]["rareType"] = "NO_RARE";
                    rare.push_back(false);
                    rareNum = 0;
                }
            }
        }

        cleanStarterData["values"][index]["pokeData"]["ballId"] = pokeballDevNames[ball][0].toStdString();
        if(randomize_starters_tera_types == true){
            cleanStarterData["values"][index]["pokeData"]["gemType"] = selectTeraTypes(pokemonMaps["pokemons_name"][starterNameStd]["natdex"], form);
        }

        if(set_tera_type_pokemon.contains(pokemonMaps["pokemons_name"][starterNameStd]["natdex"]))
            cleanStarterData["values"][index]["pokeData"]["gemType"] = selectTeraTypes(pokemonMaps["pokemons_name"][starterNameStd]["natdex"], form);

        QString keyVal = "";
        if(index == 0){
            keyVal = "hono";
        }else if(index == 1){
            keyVal = "kusa";
        }else if(index == 2){
            keyVal = "mizu";
        }
        cleanStarterData["values"][index]["pokeData"]["item"] = getPokemonItemId(pokemonMaps["pokemons_name"][starterNameStd]["natdex"], form);
        currectlySelectedStarters[keyVal]["id"] = pokemonMaps["pokemons_name"][starterNameStd]["devid"];
        currectlySelectedStarters[keyVal]["form"] = form;
        currectlySelectedStarters[keyVal]["gender"] = genderNum;
        currectlySelectedStarters[keyVal]["rare"] = rareNum;
        currectlySelectedStarters[keyVal]["gem"] = getTeraTypeInt(cleanStarterData["values"][index]["pokeData"]["gemType"]);

    }
}

bool SVStarters::randomize_starters(){
    // Logic to Setup combinations of settings for pokemon starters

    getUsablePokemon(generations_starters, only_legendary_pokemon_starters, only_paradox_pokemon_starters, only_legends_and_paradox_starters, allowedPokemon, allowedLegends);
    getBannedPokemon(ban_stage_1_pokemon_starters, ban_stage_2_pokemon_starters, ban_stage_3_pokemon_starters, ban_single_stage_pokemon_starters, allowedPokemon);

    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("SV_STARTERS_FLATBUFFERS/eventAddPokemon_array_clean.json").toStdString());
    std::ifstream fileInfo(qBaseDir.filePath("pokemon_mapping.json").toStdString());

    // Load the JSON file
    if (!file.is_open()) {
        qDebug() << "Error: Could not open data_clean.json!";
        return false;
    }
    if (!fileInfo.is_open()) {
        qDebug() << "Error: Could not open pokemon_mapping.json!";
        return false;
    }

    // initialize the json files and close the files for their values
    fileInfo >> pokemonMaps;
    file >> cleanStarterData;
    fileInfo.close();
    file.close();

    if(force_shiny_starter == true){
        int rare_Force = std::rand()%3;
        if(starters_shiny[rare_Force] == false){
            starters_shiny[rare_Force]=true;
        }
    }
    devId.clear();
    formId.clear();
    gender.clear();
    rare.clear();

    // Fuecoco
    get_selected_starter(0, starters[1], pokemonFormsIntsInGame[starters[1]][starters_forms[1]], starters_gender[1], starters_shiny[1], starters_pokeball[1]);

    // Quaxly
    get_selected_starter(2, starters[2], pokemonFormsIntsInGame[starters[2]][starters_forms[2]], starters_gender[2], starters_shiny[2], starters_pokeball[2]);

    // Sprigattito
    get_selected_starter(1, starters[0], pokemonFormsIntsInGame[starters[0]][starters_forms[0]], starters_gender[0], starters_shiny[0], starters_pokeball[0]);

    if(show_starters_in_overworld == true){
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0060_always_0_clean.trsog", "SV_STARTERS_SCENES/common_0060_always_0.trsog");
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0060_always_1_clean.trsog", "SV_STARTERS_SCENES/common_0060_always_1.trsog");
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0060_main_0_clean.trsog", "SV_STARTERS_SCENES/common_0060_main_0.trsog");
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0060_main_1_clean.trsog", "SV_STARTERS_SCENES/common_0060_main_1.trsog");
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0070_always_0_clean.trsog", "SV_STARTERS_SCENES/common_0070_always_0.trsog");
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0070_always_1_clean.trsog", "SV_STARTERS_SCENES/common_0070_always_1.trsog");
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0088_always_0_clean.trsog", "SV_STARTERS_SCENES/common_0088_always_0.trsog");
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0088_always_1_clean.trsog", "SV_STARTERS_SCENES/common_0088_always_1.trsog");
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0090_main_0_clean.trsog", "SV_STARTERS_SCENES/common_0090_main_0.trsog");
        this->modifyPokemonScene(devId, formId, gender, rare, "SV_STARTERS_SCENES/common_0090_main_1_clean.trsog", "SV_STARTERS_SCENES/common_0090_main_1.trsog");
    }

    devId.clear();
    formId.clear();
    gender.clear();
    rare.clear();

    qDebug() << allowedPokemon;

    std::ofstream fileSave(filePath+"/SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.json");
    fileSave<<cleanStarterData.dump(4);
    fileSave.close();
    return true;
}

bool SVStarters::randomize_gifts(){
    // Logic to Setup combinations of settings for pokemon starters

    getUsablePokemon(generation_gifts, false, false, false, allowedPokemon_gifts, allowedLegends_gifts);
    //getBannedPokemon(ban_stage_1_pokemon_starters, ban_stage_2_pokemon_starters, ban_stage_3_pokemon_starters, ban_single_stage_pokemon_starters, allowedPokemon_gifts);

    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file;
    file.open(qBaseDir.filePath("SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.json").toStdString());
    if(!file.is_open()){
        file.open(qBaseDir.filePath("SV_STARTERS_FLATBUFFERS/eventAddPokemon_array_clean.json").toStdString());
    }
    std::ifstream fileInfo(qBaseDir.filePath("pokemon_mapping.json").toStdString());

    // Load the JSON file
    if (!file.is_open()) {
        qDebug() << "Error: Could not open data_clean.json!";
        return false;
    }
    if (!fileInfo.is_open()) {
        qDebug() << "Error: Could not open pokemon_mapping.json!";
        return false;
    }

    // initialize the json files and close the files for their values
    fileInfo >> pokemonMaps;
    file >> cleanGiftData;
    fileInfo.close();
    file.close();

    for(unsigned long long i = 3; i<cleanGiftData["values"].size(); i++){
        int random = 1+std::rand()%totalPokemon;
        while(!allowedPokemon_gifts.contains(random))
            random = 1+std::rand()%totalPokemon;

        int formRandom = std::rand()%static_cast<int>(pokemonMaps["pokemons"][random]["forms"].size());
        while(pokemonMaps["pokemons"][random]["forms"][formRandom]["is_present"] == false){
            formRandom = std::rand()%static_cast<int>(pokemonMaps["pokemons"][random]["forms"].size());
        }

        devId.push_back(random);
        formId.push_back(formRandom);

        cleanGiftData["values"][i]["pokeData"]["devId"] = pokemonMaps["pokemons"][random]["devName"];

        // Set Starter Form
        cleanGiftData["values"][i]["pokeData"]["formId"] = formRandom;

        // Set Starter Shiny Status
        if(cleanGiftData["values"][i]["pokeData"]["rareType"] != "RARE"){
            int val = 1+std::rand()%shiny_static_rate; // range is [1, shiny_starter_rate)
            if(val == 1){
                cleanGiftData["values"][i]["pokeData"]["rareType"] = "RARE";
            }
            else{
                cleanGiftData["values"][i]["pokeData"]["rareType"] = "NO_RARE";
            }
        }

        cleanGiftData["values"][i]["pokeData"]["item"] = getPokemonItemId(pokemonMaps["pokemons"][random]["natdex"], formRandom);

        if(randomize_tera_types == true){
            cleanGiftData["values"][i]["pokeData"]["gemType"] = selectTeraTypes(random, formRandom);
        }

        if(set_tera_type_pokemon.contains(random))
            cleanGiftData["values"][i]["pokeData"]["gemType"] = selectTeraTypes(random, formRandom);

    }

    std::ofstream fileSave(filePath+"/SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.json");
    fileSave<<cleanGiftData.dump(4);
    fileSave.close();
    return true;
}
