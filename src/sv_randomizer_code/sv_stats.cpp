#include "thirdparty/nlohmann/json.hpp"
#include "headers/sv_randomizer_headers/sv_stats.h"
#include <QDebug>
#include <fstream>
#include <QDir>

using json = nlohmann::ordered_json;
namespace fs = std::filesystem;

// Global Variables
json cleanPersonalData;
json cleanMoveData;
json cleanItemMoveData;
json moveNames;

SVStats::SVStats(){

}

SVStats::~SVStats(){

}

bool allowedToLearnMove(int move, int pokemon, int form){
    switch(move){
    case 464:
        if(pokemon != 491){
            return false;
        }
        break;
    case 621:
        if(pokemon == 720){
            if(form !=1){
                return false;
            }
        }else{
            return false;
        }
        break;
    case 783:
        if(pokemon != 877){
            return false;
        }
        break;
    }

    return true;
}

bool allowedTMMove(int move){
    switch(move){
    case 464:
        return false;
    case 621:
        return false;
    case 783:
        return false;
    }

    return true;
}

void SVStats::randomize_pokemon(){
    for(unsigned long long i=0; i<cleanPersonalData["entry"].size(); i++){
        if(cleanPersonalData["entry"][i]["is_present"] == true){
            int species_check = cleanPersonalData["entry"][i]["species"]["species"];
            int form_check = cleanPersonalData["entry"][i]["species"]["form"];

            // Randomize the Pokemon's Ability
            if(randomize_abilities == true){
                int ability_choice = 1+std::rand()%310;

                if(species_check == 934){
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }

                    cleanPersonalData["entry"][i]["ability_hidden"] = ability_choice;
                    if(ability_choice == 193 || ability_choice == 194){
                        exitAbilitiesPokemon.append({{"id", cleanPersonalData["entry"][i]["species"]["model"]}, {"form", form_check}});
                    }
                }
                else if(species_check == 1021 && cleanPersonalData["entry"][i]["species"]["form"] != 0){
                    bool a1 = false;
                    bool a2 = false;
                    bool a3 = false;
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_1"] = ability_choice;
                    if(ability_choice == 193 || ability_choice == 194){
                        a1 = true;
                    }

                    ability_choice = 1+std::rand()%310;
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_2"] = ability_choice;
                    if(ability_choice == 193 || ability_choice == 194){
                        a2 = true;
                    }

                    ability_choice = 1+std::rand()%310;
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_hidden"] = ability_choice;
                    if(ability_choice == 193 || ability_choice == 194){
                        a3 = true;
                    }

                    if(a1 == true || a2 == true || a3 == true){
                        exitAbilitiesPokemon.append({{"id", cleanPersonalData["entry"][i]["species"]["model"]}, {"form", form_check}});
                    }
                }
                else if(species_check == 1021 && cleanPersonalData["entry"][i]["species"]["form"] == 0){
                    // Do not randomize abilities but change the rest
                }
                else{
                    bool a1 = false;
                    bool a2 = false;
                    bool a3 = false;

                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_1"] = ability_choice;
                    if(ability_choice == 193 || ability_choice == 194){
                        a1 = true;
                    }

                    ability_choice = 1+std::rand()%310;
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_2"] = ability_choice;
                    if(ability_choice == 193 || ability_choice == 194){
                        a2 = true;
                    }

                    ability_choice = 1+std::rand()%310;
                    while(bannedAbilities.contains(ability_choice)){
                        ability_choice = 1+std::rand()%310;
                    }
                    cleanPersonalData["entry"][i]["ability_hidden"] = ability_choice;
                    if(ability_choice == 193 || ability_choice == 194){
                        a3 = true;
                    }

                    if(a1 == true || a2 == true || a3 == true){
                        exitAbilitiesPokemon.append({{"id", cleanPersonalData["entry"][i]["species"]["model"]}, {"form", form_check}});
                    }
                }
            }

            // Randomize the Pokemon's Type
            if(randomize_types == true){
                int type_selection_1 = std::rand()%18;
                int oldType = cleanPersonalData["entry"][i]["type_1"];
                cleanPersonalData["entry"][i]["type_1"] = type_selection_1;

                if(cleanPersonalData["entry"][i]["type_2"] == oldType && give_extra_types == true){
                    int newType = std::rand()%100;
                    if(newType < 60){
                        int type_selection_2 = std::rand()%18;
                        while(type_selection_2 == type_selection_1){
                            type_selection_2 = std::rand()%18;
                        }
                        cleanPersonalData["entry"][i]["type_2"] = type_selection_2;
                    }
                }else if(cleanPersonalData["entry"][i]["type_2"] != oldType){
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
                    if(allow_moves_without_animation == true){
                        int selectedMove = 1+std::rand()%919;
                        while(currentMoves.contains(selectedMove) || !allowedToLearnMove(selectedMove, species_check, form_check)){
                            selectedMove = 1+std::rand()%919;;
                        }
                        currentMoves.append(selectedMove);
                        cleanPersonalData["entry"][i]["levelup_moves"][j]["move"] = selectedMove;
                    }else{
                        int selectedMove = std::rand()%allowed_moves.size();
                        while(currentMoves.contains(selectedMove) || !allowedToLearnMove(selectedMove, species_check, form_check)){
                            selectedMove = std::rand()%allowed_moves.size();
                        }
                        currentMoves.append(selectedMove);
                        cleanPersonalData["entry"][i]["levelup_moves"][j]["move"] = allowed_moves[selectedMove];
                    }
                }

                currentMoves = {};
                for(unsigned long long j = 0; j<cleanPersonalData["entry"][i]["reminder_moves"].size(); j++){
                    if(allow_moves_without_animation == true){
                        int selectedMove = 1+std::rand()%919;
                        while(currentMoves.contains(selectedMove) || !allowedToLearnMove(selectedMove, species_check, form_check)){
                            selectedMove = 1+std::rand()%919;;
                        }
                        currentMoves.append(selectedMove);
                        cleanPersonalData["entry"][i]["reminder_moves"][j] = selectedMove;
                    }
                    else{
                        int selectedMove = std::rand()%allowed_moves.size();
                        while(currentMoves.contains(selectedMove) || !allowedToLearnMove(selectedMove, species_check, form_check)){
                            selectedMove = std::rand()%allowed_moves.size();
                        }
                        currentMoves.append(selectedMove);
                        cleanPersonalData["entry"][i]["reminder_moves"][j] = allowed_moves[selectedMove];
                    }
                }
                if(species_check == 57){
                    cleanPersonalData["entry"][i]["reminder_moves"].push_back(889);
                }else if(species_check == 190){
                    cleanPersonalData["entry"][i]["reminder_moves"].push_back(458);
                }else if(species_check == 193){
                    cleanPersonalData["entry"][i]["reminder_moves"].push_back(246);
                }else if(species_check == 203){
                    cleanPersonalData["entry"][i]["reminder_moves"].push_back(888);
                }else if(species_check == 206){
                    cleanPersonalData["entry"][i]["reminder_moves"].push_back(887);
                }else if(species_check == 211 && cleanPersonalData["entry"][i]["species"]["form"] == 1){
                    cleanPersonalData["entry"][i]["reminder_moves"].push_back(839);
                }else if(species_check == 221){
                    cleanPersonalData["entry"][i]["reminder_moves"].push_back(246);
                }else if(species_check == 438){
                    cleanPersonalData["entry"][i]["reminder_moves"].push_back(102);
                }else if(species_check == 762){
                    cleanPersonalData["entry"][i]["reminder_moves"].push_back(23);
                }

                currentMoves = {};
                for(unsigned long long j = 0; j<cleanPersonalData["entry"][i]["egg_moves"].size(); j++){
                    if(allow_moves_without_animation == true){
                        int selectedMove = 1+std::rand()%919;
                        while(currentMoves.contains(selectedMove) || !allowedToLearnMove(selectedMove, species_check, form_check)){
                            selectedMove = 1+std::rand()%919;;
                        }
                        currentMoves.append(selectedMove);
                        cleanPersonalData["entry"][i]["egg_moves"][j] = selectedMove;
                    }else{
                        int selectedMove = std::rand()%allowed_moves.size();
                        while(currentMoves.contains(selectedMove) || !allowedToLearnMove(selectedMove, species_check, form_check)){
                            selectedMove = std::rand()%allowed_moves.size();
                        }
                        currentMoves.append(selectedMove);
                        cleanPersonalData["entry"][i]["egg_moves"][j] = allowed_moves[selectedMove];
                    }
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
            // Force TeraBlast as reminder move
            cleanPersonalData["entry"][i]["reminder_moves"].push_back(851);
            if(species_check == 1021){
                cleanPersonalData["entry"][i]["reminder_moves"].push_back(906);
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
                }else if(species_check == 109){
                    cleanPersonalData["entry"][i]["evolutions"][0]["condition"] = int(32);
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 35}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 110},
                                                                           {"form", 1}});
                }
                else if(species_check == 217){
                    cleanPersonalData["entry"][i]["evolutions"][0]["level"] = int(36);
                    cleanPersonalData["entry"][i]["evolutions"][0]["condition"] = int(32);
                    cleanPersonalData["entry"][i]["evolutions"][0]["parameter"] = int(0);
                    cleanPersonalData["entry"][i]["evolutions"][0]["reserved3"] = int(0);
                    cleanPersonalData["entry"][i]["evolutions"][0]["reserved4"] = int(0);
                    cleanPersonalData["entry"][i]["evolutions"][0]["reserved5"] = int(0);
                    cleanPersonalData["entry"][i]["evolutions"][0]["species"] = int(901);
                    cleanPersonalData["entry"][i]["evolutions"][0]["form"] = int(0);
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 901},
                                                                           {"form", 1}});
                }
                else if(species_check == 156){
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
                else if(species_check == 704){
                    cleanPersonalData["entry"][i]["evolutions"][0]["condition"] = int(32);
                    cleanPersonalData["entry"][i]["evolutions"].push_back({{"level", 36}, {"condition", 33}, {"parameter", 0}, {"reserved3", 0},
                                                                           {"reserved4", 0}, {"reserved5", 0}, {"species", 705},
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
        if(cleanPersonalData["entry"][i]["is_present"] == true){
            // Randomize Evolutions
            if(randomize_evolutions==true || evolve_every_5_levels == true){
                // Change var to be every level so it matches what it does
                std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
                QString QBaseAddress = QString::fromStdString(filePath);
                QDir qBaseDir(QBaseAddress);
                std::ifstream fileEvos(qBaseDir.filePath("pokemon_mapping.json").toStdString());

                if (!fileEvos.is_open()) {
                    qDebug() << qBaseDir.filePath("pokemon_mapping.json");
                    qDebug() << "Error: Could not open pokemon_mapping.json!";
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
            int selectedMove = 1;
            if(useAllMoves == true){
                selectedMove = 1 + std::rand()%919;
                while(selectedTMs.contains(selectedMove) || !allowedTMMove(selectedMove)){
                    selectedMove = 1 + std::rand()%919;
                }
                cleanItemMoveData["values"][i]["MachineWaza"] = moveNames["moves"][selectedMove]["devName"];
                selectedTMs.append(int(moveNames["moves"][selectedMove]["id"]));
            }else{
                selectedMove = std::rand()%allowed_moves.size();
                while(selectedTMs.contains(allowed_moves[selectedMove]) || !allowedTMMove(allowed_moves[selectedMove])){
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

bool SVStats::randomize_stats(){
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
