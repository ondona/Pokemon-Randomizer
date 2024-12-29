#include "thirdparty/nlohmann/json.hpp"
#include "headers/sv_randomizer_headers/sv_stats.h"
#include <QDebug>
#include <ctime>
#include <fstream>
#include <QDir>
#include <random>

using json = nlohmann::ordered_json;
namespace fs = std::filesystem;

// Global Variables
json cleanPersonalData;
json cleanMoveData;
json cleanItemMoveData;
json moveNames;
int seedStats;

SVStats::SVStats(){

}

SVStats::~SVStats(){

}

void SVStats::randomize_pokemon(){
    for(unsigned long long i=0; i<cleanPersonalData["entry"].size(); i++){
        std::srand(seedStats*(i+1));
        if(cleanPersonalData["entry"][i]["is_present"] == true){
            int species_check = cleanPersonalData["entry"][i]["species"]["species"];

            // Randomize the Pokemon's Ability
            if(randomize_abilities == true){
                int ability_choice = 1+std::rand()%310;

                if(species_check == 934){
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_hidden"] = ability_choice;
                }
                else if(species_check == 1021 && cleanPersonalData["entry"][i]["species"]["form"] != 0){
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_1"] = ability_choice;

                    ability_choice = 1+std::rand()%310;
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_2"] = ability_choice;

                    ability_choice = 1+std::rand()%310;
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_hidden"] = ability_choice;
                }
                else if(species_check == 1021 && cleanPersonalData["entry"][i]["species"]["form"] == 0){
                    continue;
                }
                else{
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_1"] = ability_choice;

                    ability_choice = 1+std::rand()%310;
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_2"] = ability_choice;

                    ability_choice = 1+std::rand()%310;
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_hidden"] = ability_choice;
                }
            }

            // Randomize the Pokemon's Type
            if(randomize_types == true){
                int type_selection_1 = std::rand()%18;
                cleanPersonalData["entry"][i]["type_1"] = type_selection_1;

                if(cleanPersonalData["entry"][i]["type_2"] == 0 && give_extra_types == true){
                    int type_selection_2 = std::rand()%18;
                    while(type_selection_2 == type_selection_1){
                        type_selection_2 = std::rand()%18;
                    }
                    cleanPersonalData["entry"][i]["type_2"] = type_selection_2;
                }else if(cleanPersonalData["entry"][i]["type_2"] != 0){
                    int type_selection_2 = std::rand()%18;
                    while(type_selection_2 == type_selection_1){
                        type_selection_2 = std::rand()%18;
                    }
                    cleanPersonalData["entry"][i]["type_2"] = type_selection_2;
                }

            }

            bool tmsDone = false;
            // Randomize Movesets
            if(randomize_moveset == true){
                QList<int> currentMoves;
                for(unsigned long long j = 0; j<cleanPersonalData["entry"][i]["levelup_moves"].size(); j++){
                    int selectedMove = std::rand()%allowed_moves.size();
                    while(currentMoves.contains(selectedMove)){
                        selectedMove = std::rand()%allowed_moves.size();
                    }
                    currentMoves.append(selectedMove);
                    cleanPersonalData["entry"][i]["levelup_moves"][j]["move"] = allowed_moves[selectedMove];
                }

                currentMoves = {};
                for(unsigned long long j = 0; j<cleanPersonalData["entry"][i]["reminder_moves"].size(); j++){
                    int selectedMove = std::rand()%allowed_moves.size();
                    while(currentMoves.contains(selectedMove)){
                        selectedMove = std::rand()%allowed_moves.size();
                    }
                    currentMoves.append(selectedMove);
                    cleanPersonalData["entry"][i]["reminder_moves"][j] = allowed_moves[selectedMove];
                }

                currentMoves = {};
                for(unsigned long long j = 0; j<cleanPersonalData["entry"][i]["egg_moves"].size(); j++){
                    int selectedMove = std::rand()%allowed_moves.size();
                    while(currentMoves.contains(selectedMove)){
                        selectedMove = std::rand()%allowed_moves.size();
                    }
                    currentMoves.append(selectedMove);
                    cleanPersonalData["entry"][i]["egg_moves"][j] = allowed_moves[selectedMove];
                }

                currentMoves = {};
                if(species_check != 151){
                    for(unsigned long long j = 0; j<cleanPersonalData["entry"][i]["tm_moves"].size(); j++){
                        if(enable_TMs == false){
                            int selectedMove = std::rand()%defaultTMs.size();
                            while(currentMoves.contains(selectedMove)){
                                selectedMove = std::rand()%defaultTMs.size();
                            }
                            currentMoves.append(selectedMove);
                            cleanPersonalData["entry"][i]["tm_moves"][j] = defaultTMs[selectedMove];
                        }
                        else{
                            int selectedMove = std::rand()%selectedTMs.size();
                            while(currentMoves.contains(selectedMove)){
                                selectedMove = std::rand()%selectedTMs.size();
                            }
                            currentMoves.append(selectedMove);
                            cleanPersonalData["entry"][i]["tm_moves"][j] = selectedTMs[selectedMove];
                        }
                    }
                }else{
                    json mewTms = json::array();
                    if(enable_TMs == false){
                        for(int j =0; j<defaultTMs.size(); j++){
                            mewTms.push_back(int(defaultTMs[j]));
                        }
                        cleanPersonalData["entry"][i]["tm_moves"] = mewTms;
                    }
                    else{
                        for(int j =0; j<selectedTMs.size(); j++){
                            int selectedMove = std::rand()%selectedTMs.size();
                            while(currentMoves.contains(selectedMove)){
                                selectedMove = std::rand()%selectedTMs.size();
                            }
                            currentMoves.append(selectedMove);
                            mewTms.push_back(int(selectedTMs[selectedMove]));
                        }
                        cleanPersonalData["entry"][i]["tm_moves"] = mewTms;
                    }
                }

                tmsDone = true;
            }

            // Force TeraBlast
            if(force_tera_blast_every_5_levels == true){
                for(int z = 1; z<100; z += 5){
                    cleanPersonalData["entry"][i]["levelup_moves"].push_back({{"move", 851}, {"level", int(z)}});
                }
            }

            // Randomize TMs if its enabled (check incase moveset is disabled)
            if(tmsDone == false && enable_TMs == true){
                QList<int> currentMoves = {};
                if(species_check != 151){
                    for(unsigned long long j = 0; j<cleanPersonalData["entry"][i]["tm_moves"].size(); j++){
                        int selectedMove = std::rand()%selectedTMs.size();
                        while(currentMoves.contains(selectedMove)){
                            selectedMove = std::rand()%selectedTMs.size();
                        }
                        currentMoves.append(selectedMove);
                        cleanPersonalData["entry"][i]["tm_moves"][j] = selectedTMs[selectedMove];
                    }
                }else{
                    json mewTms = json::array();
                    for(int j =0; j<selectedTMs.size(); j++){
                        mewTms.push_back(int(selectedTMs[j]));
                    }
                    cleanPersonalData["entry"][i]["tm_moves"] = mewTms;
                }
            }

            // Randomize BST
            if(randomize_bst == true){
                if(keep_same_bst == true){
                    int bstTotal = 0;
                    bstTotal += int(cleanPersonalData["entry"][i]["base_stats"]["hp"]);
                    bstTotal += int(cleanPersonalData["entry"][i]["base_stats"]["atk"]);
                    bstTotal += int(cleanPersonalData["entry"][i]["base_stats"]["def"]);
                    bstTotal += int(cleanPersonalData["entry"][i]["base_stats"]["spa"]);
                    bstTotal += int(cleanPersonalData["entry"][i]["base_stats"]["spd"]);
                    bstTotal += int(cleanPersonalData["entry"][i]["base_stats"]["spe"]);

                    QList<int> bstList = {15, 15, 15, 15, 15, 15};
                    bstTotal -= (15*6);

                    bool randomizeStats = true;
                    while(randomizeStats == true){
                        QList<int> statsChecked = {};
                        while(bstTotal !=0){
                            int noInfiniteLoop = 1;
                            int checkedTotal = bstTotal;
                            int checkingStat = std::rand() % 6;

                            while(statsChecked.contains(checkingStat)){
                                checkingStat = std::rand() % 6;
                                if(noInfiniteLoop == 6){
                                    break;
                                }

                                noInfiniteLoop++;
                            }

                            if(noInfiniteLoop==6){
                                break;
                            }

                            statsChecked.append(checkingStat);

                            int newBaseStat = std::rand() %241;
                            while(checkedTotal - newBaseStat < 0){
                                newBaseStat = std::rand() %(bstTotal+1);
                            }
                            while(bstList[checkingStat] + newBaseStat > 255){
                                newBaseStat = std::rand() %241;
                            }

                            bstList[checkingStat] += newBaseStat;
                            bstTotal -= newBaseStat;

                            if(bstTotal == 0){
                                randomizeStats = false;
                            }
                        };
                    };

                    cleanPersonalData["entry"][i]["base_stats"]["hp"] = bstList[0];
                    cleanPersonalData["entry"][i]["base_stats"]["atk"] = bstList[1];
                    cleanPersonalData["entry"][i]["base_stats"]["def"] = bstList[2];
                    cleanPersonalData["entry"][i]["base_stats"]["spa"] = bstList[3];
                    cleanPersonalData["entry"][i]["base_stats"]["spd"] = bstList[4];
                    cleanPersonalData["entry"][i]["base_stats"]["spe"] = bstList[5];
                }
                else{
                    cleanPersonalData["entry"][i]["base_stats"]["hp"] = 15 + std::rand()%(255 - 15);
                    cleanPersonalData["entry"][i]["base_stats"]["atk"] = 15 + std::rand()%(255 - 15);
                    cleanPersonalData["entry"][i]["base_stats"]["def"] = 15 + std::rand()%(255 - 15);
                    cleanPersonalData["entry"][i]["base_stats"]["spa"] = 15 + std::rand()%(255 - 15);
                    cleanPersonalData["entry"][i]["base_stats"]["spd"] = 15 + std::rand()%(255 - 15);
                    cleanPersonalData["entry"][i]["base_stats"]["spe"] = 15 + std::rand()%(255 - 15);
                }
            }

            // Fix Evolutions
            if(force_unobtainable_evolutions_at_night){
                if(species_check == 25){
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 26},
                                                                           {"form", 1}});
                }else if(species_check == 102){
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 103},
                                                                           {"form", 1}});
                }else if(species_check == 156){
                    cleanPersonalData["entry"][i]["evolutions"][0]["condition"] = int(32);
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 157},
                                                                           {"form", 1}});
                }
                else if(species_check == 234){
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 899},
                                                                           {"form", 0}});
                }else if(species_check == 502){
                    cleanPersonalData["entry"][i]["evolutions"][0]["condition"] = int(32);
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 503},
                                                                           {"form", 1}});
                }else if(species_check == 548){
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 549},
                                                                           {"form", 1}});
                }
                else if(species_check == 627){
                    cleanPersonalData["entry"][i]["evolutions"][0]["condition"] = int(32);
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 628},
                                                                           {"form", 1}});
                }else if(species_check == 712){
                    cleanPersonalData["entry"][i]["evolutions"][0]["condition"] = int(32);
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 713},
                                                                           {"form", 1}});
                }else if(species_check == 723){
                    cleanPersonalData["entry"][i]["evolutions"][0]["condition"] = int(32);
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 724},
                                                                           {"form", 1}});
                }
            }
        }
    }

    for(unsigned long long i =0; i<cleanPersonalData["entry"].size(); i++){
        std::srand(seedStats*(i+1));
        if(cleanPersonalData["entry"][i]["is_present"] == true){
            int species_check = cleanPersonalData["entry"][i]["species"]["species"];

            // Randomize Evolutions
            if(randomize_evolutions==true || evolve_every_5_levels == true){
                // Change var to be every level so it matches what it does
                std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
                QString QBaseAddress = QString::fromStdString(filePath);
                QDir qBaseDir(QBaseAddress);
                std::ifstream fileEvos(qBaseDir.filePath("pokemon_mapping.json").toStdString());

                if (!fileEvos.is_open()) {
                    qDebug() << qBaseDir.filePath("pokemon_mapping_evos.json");
                    qDebug() << "Error: Could not open pokemon_mapping_evos.json!";
                }

                json pokemonMaps;
                fileEvos >> pokemonMaps;
                fileEvos.close();

                if(evolve_every_5_levels == true){
                    json newEvolutions = json::array();
                    for(int z = 2; z<=100; z++){
                        int selectedPokemon = 1 + std::rand()%1025;
                        while(banned_pokemon.contains(selectedPokemon)){
                            selectedPokemon = 1 + std::rand()%1025;
                        }
                        int formRandom = std::rand()%static_cast<int>(pokemonMaps["pokemons"][selectedPokemon]["forms"].size());
                        while(pokemonMaps["pokemons"][selectedPokemon]["forms"][formRandom]["is_present"] == false){
                            formRandom = std::rand()%static_cast<int>(pokemonMaps["pokemons"][selectedPokemon]["forms"].size());
                        }

                        newEvolutions.push_back({{"level", int(z)}, {"condition", 4}, {"parameter", 0}, {"reserved3", 0},
                                                 {"reserved4", 0}, {"reserved5", 0}, {"species", int(pokemonMaps["pokemons"][selectedPokemon]["devid"])},
                                                 {"form", formRandom}});
                    }
                    cleanPersonalData["entry"][i]["evolutions"] = newEvolutions;
                }
                else{
                    for(unsigned long long z =0; z<cleanPersonalData["entry"][i]["evolutions"].size(); z++){
                        int selectedPokemon = 1 + std::rand()%1025;
                        while(banned_pokemon.contains(selectedPokemon)){
                            selectedPokemon = 1 + std::rand()%1025;
                        }

                        int formRandom = std::rand()%static_cast<int>(pokemonMaps["pokemons"][selectedPokemon]["forms"].size());
                        while(pokemonMaps["pokemons"][selectedPokemon]["forms"][formRandom]["is_present"] == false){
                            formRandom = std::rand()%static_cast<int>(pokemonMaps["pokemons"][selectedPokemon]["forms"].size());
                        }

                        cleanPersonalData["entry"][i]["evolutions"][z]["species"] = int(pokemonMaps["pokemons"][selectedPokemon]["devid"]);
                        cleanPersonalData["entry"][i]["evolutions"][z]["form"] = formRandom;
                    }
                }
            }
        }
    }
}

void SVStats::randomize_tmsFile(bool useAllMoves){
    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("SV_PERSONAL/itemdata_array_clean.json").toStdString());
    std::ifstream fileMoves(qBaseDir.filePath("SV_PERSONAL/sorted_move_list.json").toStdString());

    if (!file.is_open()) {
        qDebug() << "Error: Could not open waza_array_clean.json!";
    }
    if (!fileMoves.is_open()) {
        qDebug() << "Error: Could not open waza_array_clean.json!";
    }

    // initialize the json files and close the files for their values
    file >> cleanItemMoveData;
    file.close();

    fileMoves>>moveNames;
    fileMoves.close();

    for(unsigned long long i = 0; i<cleanItemMoveData["values"].size(); i++){
        if(cleanItemMoveData["values"][i]["ItemType"] == "ITEMTYPE_WAZA"){
            std::srand(seedStats*(i+1));
            int selectedMove = 1;
            if(useAllMoves == true){
                selectedMove = 1 + std::rand()%919;
                while(selectedTMs.contains(selectedMove)){
                    selectedMove = 1 + std::rand()%919;
                }
                cleanItemMoveData["values"][i]["MachineWaza"] = moveNames["moves"][selectedMove]["devName"];
                selectedTMs.append(int(moveNames["moves"][selectedMove]["id"]));
            }else{
                selectedMove = std::rand()%allowed_moves.size();
                while(selectedTMs.contains(allowed_moves[selectedMove])){
                    selectedMove = std::rand()%allowed_moves.size();
                }
                cleanItemMoveData["values"][i]["MachineWaza"] = moveNames["moves"][allowed_moves[selectedMove]]["devName"];
                selectedTMs.append(int(moveNames["moves"][allowed_moves[selectedMove]]["id"]));
            }
        }
    }

    std::ofstream fileSave(filePath+"/SV_PERSONAL/itemdata_array.json");
    fileSave<<cleanItemMoveData.dump(4);
    fileSave.close();
}

void SVStats::allow_all_moves(){
    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("SV_PERSONAL/waza_array_clean.json").toStdString());

    if (!file.is_open()) {
        qDebug() << "Error: Could not open waza_array_clean.json!";
    }

    // initialize the json files and close the files for their values
    file >> cleanMoveData;
    file.close();

    // Code
    for(unsigned long long i = 0; i<cleanMoveData["Table"].size(); i++){
        // if (!cleanMoveData["Table"][i].contains("MoveID")) {
        //     cleanMoveData["Table"][i]["MoveID"] = 0;
        // }

        if (!cleanMoveData["Table"][i].contains("CanUseMove")) {
            cleanMoveData["Table"][i]["CanUseMove"] = true;
        }

        if(!cleanMoveData["Table"][i].contains("Power") || cleanMoveData["Table"][i]["Power"] == 1)
            cleanMoveData["Table"][i]["Power"] = int(65);

    }

    std::ofstream fileSave(filePath+"/SV_PERSONAL/waza_array.json");
    fileSave<<cleanMoveData.dump(4);
    fileSave.close();
}

bool SVStats::randomize_stats(int passedSeed, int run){
    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("SV_PERSONAL/personal_array_clean.json").toStdString());
    // Load the JSON file

    if (!file.is_open()) {
        qDebug() << "Error: Could not open personal_array_clean.json!";
        return false;
    }

    // initialize the json files and close the files for their values
    file >> cleanPersonalData;
    file.close();

    // if there is no seed then use current time as seed
    if(passedSeed == 0){
        std::random_device rd;
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        seedStats = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() ^ rd());
    }
    else{
        seedStats = passedSeed*run;
    }

    if(ban_wonder_guard == true){
        bannedAbilities.append(25);
    }

    if(enable_TMs == true){
        if(allow_moves_without_animation == true){
            qDebug()<<"Here";
            allow_all_moves();
            randomize_tmsFile(true);
            qDebug()<<"Finished TMs Random w/All";
        }else{
            randomize_tmsFile();
        }
    }

    randomize_pokemon();

    bannedAbilities = {278, 307};
    selectedTMs = {};

    std::ofstream fileSave(filePath+"/SV_PERSONAL/personal_array.json");
    fileSave<<cleanPersonalData.dump(4);
    fileSave.close();
    return true;
}
