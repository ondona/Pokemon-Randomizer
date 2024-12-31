#include "headers/sv_randomizer_headers/sv_raids.h"
#include <QString>
#include <string>
#include <QMap>
#include <QDir>
#include <QDebug>
#include <fstream>
#include "thirdparty/nlohmann/json.hpp"

// Look into QSharedData in the future
using json = nlohmann::json;
namespace fs = std::filesystem;

int getBannedLength(QList<bool> generations){
    int gen1bannedlength = 52;
    int gen2bannedlength = 18;
    int gen3bannedlength = 48;
    int gen4bannedlength = 22;
    int gen5bannedlength = 70;
    int gen6bannedlength = 25;
    int gen7bannedlength = 25;
    int gen8bannedlength = 31;
    int gen9bannedlength = 0;

    int totalGens = 0;
    int totalBanned = 0;
    for(int i=0; i<9; i++){
        if(generations[i] == true){
            // Logic
            switch (i){
            case 0:
                totalBanned += gen1bannedlength;
                break;
            case 1:
                totalBanned += gen2bannedlength;
                break;
            case 2:
                totalBanned += gen3bannedlength;
                break;
            case 3:
                totalBanned += gen4bannedlength;
                break;
            case 4:
                totalBanned += gen5bannedlength;
                break;
            case 5:
                totalBanned += gen6bannedlength;
                break;
            case 6:
                totalBanned += gen7bannedlength;
                break;
            case 7:
                totalBanned += gen8bannedlength;
                break;
            case 8:
                totalBanned += gen9bannedlength;
                break;
            };
            totalGens++;
        }
    };

    if(totalGens == 0){
        totalBanned += gen1bannedlength;
        totalBanned += gen2bannedlength;
        totalBanned += gen3bannedlength;
        totalBanned += gen4bannedlength;
        totalBanned += gen5bannedlength;
        totalBanned += gen6bannedlength;
        totalBanned += gen7bannedlength;
        totalBanned += gen9bannedlength;
    }

    return totalBanned;
}

bool SVRaids::randomize_paldean_raids(std::string fileName){
    json wildPokemonInfo;
    json paldeaRaidJSON;

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


    QList<int> wildPaldeanPokemon = {};
    QList<int> wildLegendMons_Useless = {};
    getUsablePokemon(praidsgeneratio, praids_onlyLegends, praids_onlyParadox, praids_onlyLegendsandParadox, wildPaldeanPokemon, wildLegendMons_Useless);
    getBannedPokemon(praids_BanStage1, praids_BanStage2, praids_BanStage3, praids_Ban1Stage, wildPaldeanPokemon);
    int banned = getBannedLength(praidsgeneratio);
    std::string ogName = fileName;

    for(int i = 1; i<=6; i++){
        fileName = ogName;
        fileName = fileName + std::to_string(i) + "_array_clean.json";
        std::ifstream raidFile(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(fileName)).toStdString());

        if(!raidFile.is_open()){
            qDebug()<<"Issue in Opening: " + QString::fromStdString(fileName);
        }

        qDebug()<<"Checking: " + QString::fromStdString(fileName);

        raidFile >> paldeaRaidJSON;
        raidFile.close();

        json newRaids;
        newRaids["values"] = json::array();
        unsigned long long counter = 0;
        QList<int> usedPokemon;
        QList<QMap<QString, int>> usedForms = {{{"id", 0}, {"form", 0}}};
        wildPaldeanPokemon.removeOne(1024);
        QSet<int> indexesChanged;

        for(unsigned long long j =0; j<paldeaRaidJSON["values"].size(); j++){
            if(usedPokemon.length() - wildPaldeanPokemon.length() == 0){
                break;
            }
            else if(usedPokemon.length() - wildPaldeanPokemon.length() == banned){
                break;
            }
            QMap<QString, int> checkDict;
            int random;
            int formRandom;
            bool check_loop = true;
            do{
                random = 1+std::rand()%1025;
                while(!wildPaldeanPokemon.contains(wildPokemonInfo["pokemons"][random]["natdex"]))
                    random = 1+std::rand()%1025;

                formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
                while(wildPokemonInfo["pokemons"][random]["forms"][formRandom]["is_present"] == false){
                    formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
                }

                checkDict = {{"id", int(random)}, {"form", int(formRandom)}};
                if(!usedForms.contains(checkDict)){
                    usedForms.append(checkDict);
                    check_loop = false;
                }
            } while (check_loop);

            counter++;

            int check = 0;
            for(int k =0; k<usedForms.size(); k++){
                if(usedForms[k]["id"] == checkDict["id"]){
                    check++;
                }
            }

            int formTotal = 0;
            for(unsigned long long k =0; k < wildPokemonInfo["pokemons"][random]["forms"].size(); k++){
                if(wildPokemonInfo["pokemons"][random]["forms"][k]["is_present"] == true){
                    formTotal++;
                }
            }

            if(check == formTotal){
                usedPokemon.append(random);
            }

            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["romVer"] = "BOTH";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["devId"] = wildPokemonInfo["pokemons"][random]["devName"];
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["formId"] = formRandom;
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["item"] = getItemForPokemon(random, formRandom);
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["wazaType"] = "DEFAULT";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza1"]["wazaId"] = "WAZA_NULL";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza2"]["wazaId"] = "WAZA_NULL";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza3"]["wazaId"] = "WAZA_NULL";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza4"]["wazaId"] = "WAZA_NULL";
            if(random == 1017){
                if(formRandom == 0){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "KUSA";
                }
                if(formRandom == 1){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "MIZU";
                }
                if(formRandom == 2){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "HONOO";
                }
                if(formRandom == 3){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "IWA";
                }
            }

            if(praids_force_shiny == true){
                paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["rareType"] = "RARE";
            }else{
                int shiny_num = 1+std::rand()%praids_shiny_chance;
                if(shiny_num == 1){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["rareType"] = "RARE";
                }
            }

            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["timming"] = "HP";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["action"] = "WAZA";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["value"] = int(85);
            if(random != 1024){
                paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["wazano"] = "WAZA_TERABAASUTO";
            }else{
                paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["wazano"] = "WAZA_TERAKURASUTAA";
            }

            indexesChanged.insert(i);
        }

        if(paldeaRaidJSON["values"].size() - counter > 0){
            for(int j =0; j<indexesChanged.size(); j++){
                newRaids["values"].push_back(paldeaRaidJSON["values"][i]);
            }
        }else if(paldeaRaidJSON["values"].size() - counter < 0){
            for(int j =0; j<indexesChanged.size(); j++){
                newRaids["values"].push_back(paldeaRaidJSON["values"][i]);
            }
        }else{
            newRaids["values"] = paldeaRaidJSON["values"];
        }

        std::string outputName = ogName + std::to_string(i) +"_array.json";
        std::ofstream fileSave(filePath+"/SV_RAIDS/"+outputName);
        fileSave<<paldeaRaidJSON.dump(4);
        fileSave.close();
    }

    // Loop through the file
    return true;
}

bool SVRaids::randomize_kitakami_raids(std::string fileName){
    json wildPokemonInfo;
    json paldeaRaidJSON;

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


    QList<int> wildPaldeanPokemon = {};
    QList<int> wildLegendMons_Useless = {};
    getUsablePokemon(kraidsgeneration, kraids_onlyLegends, kraids_onlyParadox, kraids_onlyLegendsandParadox, wildPaldeanPokemon, wildLegendMons_Useless);
    getBannedPokemon(kraids_BanStage1, kraids_BanStage2, kraids_BanStage3, kraids_Ban1Stage, wildPaldeanPokemon);
    int banned = getBannedLength(kraidsgeneration);
    std::string ogName = fileName;

    for(int i = 1; i<=6; i++){
        fileName = ogName;
        fileName = fileName + std::to_string(i) + "_array_clean.json";
        std::ifstream raidFile(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(fileName)).toStdString());

        if(!raidFile.is_open()){
            qDebug()<<"Issue in Opening: " + QString::fromStdString(fileName);
        }

        qDebug()<<"Checking: " + QString::fromStdString(fileName);

        raidFile >> paldeaRaidJSON;
        raidFile.close();

        json newRaids;
        newRaids["values"] = json::array();
        unsigned long long counter = 0;
        QList<int> usedPokemon;
        QList<QMap<QString, int>> usedForms = {{{"id", 0}, {"form", 0}}};
        wildPaldeanPokemon.removeOne(1024);
        QSet<int> indexesChanged;

        for(unsigned long long j =0; j<paldeaRaidJSON["values"].size(); j++){
            if(usedPokemon.length() - wildPaldeanPokemon.length() == 0){
                break;
            }
            else if(usedPokemon.length() - wildPaldeanPokemon.length() == banned){
                break;
            }
            QMap<QString, int> checkDict;
            int random;
            int formRandom;
            bool check_loop = true;
            do{
                random = 1+std::rand()%1025;
                while(!wildPaldeanPokemon.contains(wildPokemonInfo["pokemons"][random]["natdex"]))
                    random = 1+std::rand()%1025;

                formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
                while(wildPokemonInfo["pokemons"][random]["forms"][formRandom]["is_present"] == false){
                    formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
                }

                checkDict = {{"id", int(random)}, {"form", int(formRandom)}};
                if(!usedForms.contains(checkDict)){
                    usedForms.append(checkDict);
                    check_loop = false;
                }
            } while (check_loop);

            counter++;

            int check = 0;
            for(int k =0; k<usedForms.size(); k++){
                if(usedForms[k]["id"] == checkDict["id"]){
                    check++;
                }
            }

            int formTotal = 0;
            for(unsigned long long k =0; k < wildPokemonInfo["pokemons"][random]["forms"].size(); k++){
                if(wildPokemonInfo["pokemons"][random]["forms"][k]["is_present"] == true){
                    formTotal++;
                }
            }

            if(check == formTotal){
                usedPokemon.append(random);
            }

            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["romVer"] = "BOTH";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["devId"] = wildPokemonInfo["pokemons"][random]["devName"];
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["formId"] = formRandom;
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["item"] = getItemForPokemon(random, formRandom);
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["wazaType"] = "DEFAULT";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza1"]["wazaId"] = "WAZA_NULL";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza2"]["wazaId"] = "WAZA_NULL";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza3"]["wazaId"] = "WAZA_NULL";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza4"]["wazaId"] = "WAZA_NULL";
            if(random == 1017){
                if(formRandom == 0){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "KUSA";
                }
                if(formRandom == 1){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "MIZU";
                }
                if(formRandom == 2){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "HONOO";
                }
                if(formRandom == 3){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "IWA";
                }
            }

            if(kraids_force_shiny == true){
                paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["rareType"] = "RARE";
            }else{
                int shiny_num = 1+std::rand()%kraids_shiny_chance;
                if(shiny_num == 1){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["rareType"] = "RARE";
                }
            }

            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["timming"] = "HP";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["action"] = "WAZA";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["value"] = int(85);
            if(random != 1024){
                paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["wazano"] = "WAZA_TERABAASUTO";
            }else{
                paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["wazano"] = "WAZA_TERAKURASUTAA";
            }

            indexesChanged.insert(i);
        }

        if(paldeaRaidJSON["values"].size() - counter > 0){
            for(int j =0; j<indexesChanged.size(); j++){
                newRaids["values"].push_back(paldeaRaidJSON["values"][i]);
            }
        }else if(paldeaRaidJSON["values"].size() - counter < 0){
            for(int j =0; j<indexesChanged.size(); j++){
                newRaids["values"].push_back(paldeaRaidJSON["values"][i]);
            }
        }else{
            newRaids["values"] = paldeaRaidJSON["values"];
        }

        std::string outputName = ogName + std::to_string(i) +"_array.json";
        std::ofstream fileSave(filePath+"/SV_RAIDS/"+outputName);
        fileSave<<paldeaRaidJSON.dump(4);
        fileSave.close();
    }

    // Loop through the file
    return true;
}

bool SVRaids::randomize_blueberry_raids(std::string fileName){
    json wildPokemonInfo;
    json paldeaRaidJSON;

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

    QList<int> wildPaldeanPokemon = {};
    QList<int> wildLegendMons_Useless = {};
    getUsablePokemon(braidsgeneration, braids_onlyLegends, braids_onlyParadox, braids_onlyLegendsandParadox, wildPaldeanPokemon, wildLegendMons_Useless);
    getBannedPokemon(braids_BanStage1, braids_BanStage2, braids_BanStage3, braids_Ban1Stage, wildPaldeanPokemon);
    int banned = getBannedLength(braidsgeneration);
    std::string ogName = fileName;

    for(int i = 1; i<=6; i++){
        fileName = ogName;
        fileName = fileName + std::to_string(i) + "_array_clean.json";
        std::ifstream raidFile(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(fileName)).toStdString());

        if(!raidFile.is_open()){
            qDebug()<<"Issue in Opening: " + QString::fromStdString(fileName);
        }

        qDebug()<<"Checking: " + QString::fromStdString(fileName);

        raidFile >> paldeaRaidJSON;
        raidFile.close();

        json newRaids;
        newRaids["values"] = json::array();
        unsigned long long counter = 0;
        QList<int> usedPokemon;
        QList<QMap<QString, int>> usedForms = {{{"id", 0}, {"form", 0}}};
        wildPaldeanPokemon.removeOne(1024);
        QSet<int> indexesChanged;

        for(unsigned long long j =0; j<paldeaRaidJSON["values"].size(); j++){
            if(usedPokemon.length() - wildPaldeanPokemon.length() == 0){
                break;
            }
            else if(usedPokemon.length() - wildPaldeanPokemon.length() == banned){
                break;
            }
            QMap<QString, int> checkDict;
            int random;
            int formRandom;
            bool check_loop = true;
            do{
                random = 1+std::rand()%1025;
                while(!wildPaldeanPokemon.contains(wildPokemonInfo["pokemons"][random]["natdex"]))
                    random = 1+std::rand()%1025;

                formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
                while(wildPokemonInfo["pokemons"][random]["forms"][formRandom]["is_present"] == false){
                    formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
                }

                checkDict = {{"id", int(random)}, {"form", int(formRandom)}};
                if(!usedForms.contains(checkDict)){
                    usedForms.append(checkDict);
                    check_loop = false;
                }
            } while (check_loop);

            counter++;

            int check = 0;
            for(int k =0; k<usedForms.size(); k++){
                if(usedForms[k]["id"] == checkDict["id"]){
                    check++;
                }
            }

            int formTotal = 0;
            for(unsigned long long k =0; k < wildPokemonInfo["pokemons"][random]["forms"].size(); k++){
                if(wildPokemonInfo["pokemons"][random]["forms"][k]["is_present"] == true){
                    formTotal++;
                }
            }

            if(check == formTotal){
                usedPokemon.append(random);
            }

            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["romVer"] = "BOTH";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["devId"] = wildPokemonInfo["pokemons"][random]["devName"];
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["formId"] = formRandom;
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["item"] = getItemForPokemon(random, formRandom);
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["wazaType"] = "DEFAULT";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza1"]["wazaId"] = "WAZA_NULL";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza2"]["wazaId"] = "WAZA_NULL";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza3"]["wazaId"] = "WAZA_NULL";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["waza4"]["wazaId"] = "WAZA_NULL";
            if(random == 1017){
                if(formRandom == 0){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "KUSA";
                }
                if(formRandom == 1){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "MIZU";
                }
                if(formRandom == 2){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "HONOO";
                }
                if(formRandom == 3){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["gemType"] = "IWA";
                }
            }

            if(braids_force_shiny == true){
                paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["rareType"] = "RARE";
            }else{
                int shiny_num = 1+std::rand()%braids_shiny_chance;
                if(shiny_num == 1){
                    paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossPokePara"]["rareType"] = "RARE";
                }
            }

            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["timming"] = "HP";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["action"] = "WAZA";
            paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["value"] = int(85);
            if(random != 1024){
                paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["wazano"] = "WAZA_TERABAASUTO";
            }else{
                paldeaRaidJSON["values"][j]["raidEnemyInfo"]["bossDesc"]["extraAction1"]["wazano"] = "WAZA_TERAKURASUTAA";
            }

            indexesChanged.insert(i);
        }

        if(paldeaRaidJSON["values"].size() - counter > 0){
            for(int j =0; j<indexesChanged.size(); j++){
                newRaids["values"].push_back(paldeaRaidJSON["values"][i]);
            }
        }else if(paldeaRaidJSON["values"].size() - counter < 0){
            for(int j =0; j<indexesChanged.size(); j++){
                newRaids["values"].push_back(paldeaRaidJSON["values"][i]);
            }
        }else{
            newRaids["values"] = paldeaRaidJSON["values"];
        }

        std::string outputName = ogName + std::to_string(i) +"_array.json";
        std::ofstream fileSave(filePath+"/SV_RAIDS/"+outputName);
        fileSave<<paldeaRaidJSON.dump(4);
        fileSave.close();
    }

    // Loop through the file
    return true;
}
