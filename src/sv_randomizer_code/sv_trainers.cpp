#include "headers/sv_randomizer_headers/sv_trainers.h"
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

json wildPokemonInfo;
json cleanTrainerInfo;

int getMaxNumberOfChanges(json pokemonFile, int index, bool NULLS = false){
    int totalChanges = 0;

    for(int i = 1; i<=6; i++){
        std::string key = "poke"+std::to_string(i);
        if(NULLS == true){
            if(pokemonFile["values"][index][key]["devId"] == "DEV_NULL"){
                totalChanges++;
            }
        }else{
            if(pokemonFile["values"][index][key]["devId"] != "DEV_NULL"){
                totalChanges++;
            }
        }
    }

    return totalChanges;
}

void SVTrainers::randomizeTrainerFights(QList<int> FightLists, QVector<bool> settings, QVector<bool> globalSettings, QList<int> AllowedMons){
    for(int i =0; i<FightLists.size(); i++){
        if(!randomize_arven_titan.contains(FightLists[i])){
            if(globalSettings[0] == true || settings[2] == true){
                cleanTrainerInfo["values"][FightLists[i]]["changeGem"] = true;
            }
        }

        int maxNum = getMaxNumberOfChanges(cleanTrainerInfo, FightLists[i]);
        int ogMax = maxNum;

        if(!raid_npc_index.contains(FightLists[i]) == true && !multi_battles_index.contains(FightLists[i]) == true){
            if(globalSettings[3] == true){
                if(1+std::rand()%2 == 2){
                    cleanTrainerInfo["values"][FightLists[i]]["aiDouble"] = true;
                    cleanTrainerInfo["values"][FightLists[i]]["battleType"] = "_2vs2";
                    if(maxNum < 2){
                        maxNum = 2 ;
                    }
                }
            }
            if(globalSettings[4] == true){
                cleanTrainerInfo["values"][FightLists[i]]["aiDouble"] = true;
                cleanTrainerInfo["values"][FightLists[i]]["battleType"] = "_2vs2";
                if(maxNum < 2){
                    maxNum = 2 ;
                }
            }
        }
        if(cleanTrainerInfo["values"][FightLists[i]]["trid"] == "sister_01_01" || cleanTrainerInfo["values"][FightLists[i]]["trid"] == "sister_01_01_strong"){
            cleanTrainerInfo["values"][FightLists[i]]["aiDouble"] = false;
            cleanTrainerInfo["values"][FightLists[i]]["battleType"] = "_1vs1";
        }
        if(FightLists[i] >= 585 && FightLists[i] <= 594){
            cleanTrainerInfo["values"][FightLists[i]]["aiDouble"] = false;
            cleanTrainerInfo["values"][FightLists[i]]["battleType"] = "_1vs1";
        }

        // ----------Start Changing Pokemon Settings
        if(settings[0] == true){
            maxNum = 6;
            if(cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_aku_boss_01" ||
                cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_doku_boss_01" ||
                cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_fairy_boss_01" ||
                cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_hono_boss_01" ||
                cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_kakutou_boss_01"){
                maxNum = 5;
                cleanTrainerInfo["values"][FightLists[i]]["aiDouble"] = false;
                cleanTrainerInfo["values"][FightLists[i]]["battleType"] = "_1vs1";
            }
        }else if(settings[1] == true){
            if(maxNum < 6){
                maxNum = maxNum + std::rand()%(7-maxNum);
            }

            // protective case
            if(maxNum>6){
                maxNum = 6;
                if(cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_aku_boss_01" ||
                    cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_doku_boss_01" ||
                    cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_fairy_boss_01" ||
                    cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_hono_boss_01" ||
                    cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_kakutou_boss_01"){
                    maxNum = 5;
                    cleanTrainerInfo["values"][FightLists[i]]["aiDouble"] = false;
                    cleanTrainerInfo["values"][FightLists[i]]["battleType"] = "_1vs1";
                }
            }

            if(maxNum == 6){
                if(cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_aku_boss_01" ||
                    cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_doku_boss_01" ||
                    cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_fairy_boss_01" ||
                    cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_hono_boss_01" ||
                    cleanTrainerInfo["values"][FightLists[i]]["trid"] == "dan_kakutou_boss_01" || FightLists[i] == 681){
                    maxNum = 5;
                    cleanTrainerInfo["values"][FightLists[i]]["aiDouble"] = false;
                    cleanTrainerInfo["values"][FightLists[i]]["battleType"] = "_1vs1";
                }
            }
        }



        if(cleanTrainerInfo["values"][FightLists[i]]["trid"] == "rival_01_hono" ||
           cleanTrainerInfo["values"][FightLists[i]]["trid"] == "rival_01_kusa" ||
            cleanTrainerInfo["values"][FightLists[i]]["trid"] == "rival_01_mizu" || FightLists[i] == 711){
            cleanTrainerInfo["values"][FightLists[i]]["aiDouble"] = false;
            cleanTrainerInfo["values"][FightLists[i]]["battleType"] = "_1vs1";
            cleanTrainerInfo["values"][FightLists[i]]["changeGem"] = false;
            maxNum = 1;
        }

        if(settings[3] == true){
            for(int j = 1; j<=maxNum; j++){
                std::string key = "poke"+std::to_string(j);
                cleanTrainerInfo["values"][FightLists[i]][key]["talentType"] = "VALUE";
                cleanTrainerInfo["values"][FightLists[i]][key]["talentValue"]["hp"] = 31;
                cleanTrainerInfo["values"][FightLists[i]][key]["talentValue"]["atk"] = 31;
                cleanTrainerInfo["values"][FightLists[i]][key]["talentValue"]["def"] = 31;
                cleanTrainerInfo["values"][FightLists[i]][key]["talentValue"]["spAtk"] = 31;
                cleanTrainerInfo["values"][FightLists[i]][key]["talentValue"]["spDef"] = 31;
                cleanTrainerInfo["values"][FightLists[i]][key]["talentValue"]["agi"] = 31;
            }
        }

        if(settings[4] == true){
            cleanTrainerInfo["values"][FightLists[i]]["aiExpert"] = true;
            cleanTrainerInfo["values"][FightLists[i]]["aiItem"] = true;
            cleanTrainerInfo["values"][FightLists[i]]["aiChange"] = true;
        }

        // Change Pokemons

        for(int j =1; j<=maxNum; j++){
            std::string key = "poke"+std::to_string(j);

            int random = 1+std::rand()%1025;
            while(!AllowedMons.contains(wildPokemonInfo["pokemons"][random]["natdex"]))
                random = 1+std::rand()%1025;

            int formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
            while(wildPokemonInfo["pokemons"][random]["forms"][formRandom]["is_present"] == false){
                formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
            }

            cleanTrainerInfo["values"][FightLists[i]][key]["devId"] = wildPokemonInfo["pokemons"][random]["devName"];
            cleanTrainerInfo["values"][FightLists[i]][key]["formId"] = formRandom;
            cleanTrainerInfo["values"][FightLists[i]][key]["item"] = getPokemonItemId(wildPokemonInfo["pokemons"][random]["natdex"], formRandom);
            if(cleanTrainerInfo["values"][FightLists[i]][key]["level"] == 0){
                std::string stringMaxKey = "poke" + std::to_string(ogMax);
                cleanTrainerInfo["values"][FightLists[i]][key]["level"] = int(cleanTrainerInfo["values"][FightLists[i]][stringMaxKey]["level"]) + std::rand()%3;
            }

            cleanTrainerInfo["values"][FightLists[i]][key]["wazaType"] = "DEFAULT";
            for(int k = 1; k<=4; k++){
                std::string moveKey = "waza"+std::to_string(k);
                cleanTrainerInfo["values"][FightLists[i]][key][moveKey]["wazaId"] = "WAZA_NULL";
            }

            if(globalSettings[2] == true){
                int selection = 1+std::rand()%50;
                if(selection == 1)
                    cleanTrainerInfo["values"][FightLists[i]][key]["rareType"] = "RARE";
            }

            if(globalSettings[1] == true){
                cleanTrainerInfo["values"][FightLists[i]][key]["gemType"] = selectTeraTypes(random, formRandom);
            }
        }
    }
}

void SVTrainers::openFiles(){
    qDebug()<<"Opening Files";
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

    std::ifstream raidFile(qBaseDir.filePath("SV_TRAINERS/trdata_array_clean.json").toStdString());
    raidFile >> cleanTrainerInfo;
    raidFile.close();

    obtainCleanRatios();
}

void SVTrainers::closeFiles(){
    qDebug()<<"Here - Closing";
    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    std::ofstream fileSave(filePath+"/SV_TRAINERS/trdata_array.json");
    fileSave<<cleanTrainerInfo.dump(4);
    fileSave.close();
}

void SVTrainers::noSoftlockParadise(QList<int> AllowedMons){
    QList<int> protocol = {437, 439};
    for(int i = 0; i< protocol.length(); i++){
        QMap<QString, int> checkDict = {{"id", 0}, {"form", 0}};

        while(wonderGuardPokemon.contains(checkDict)){
            int random = 1+std::rand()%1025;

            while(!AllowedMons.contains(int(wildPokemonInfo["pokemons"][random]["natdex"])))
                random = 1+std::rand()%1025;

            int formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
            while(wildPokemonInfo["pokemons"][random]["forms"][formRandom]["is_present"] == false){
                formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
            }

            checkDict={{"id", int(random)}, {"form",int(formRandom)}};
        }

        cleanTrainerInfo["values"][protocol[i]]["poke1"]["devId"] = wildPokemonInfo["pokemons"][int(checkDict["id"])]["devName"];
        cleanTrainerInfo["values"][protocol[i]]["poke1"]["formId"] = int(checkDict["form"]);
    }
}

void SVTrainers::noSoftlockTerapagos(QList<int> AllowedMons){
    QList<int> protocol = {680};
    for(int i = 0; i< protocol.length(); i++){
        QMap<QString, int> checkDict = {{"id", 0}, {"form", 0}};

        while(wonderGuardPokemon.contains(checkDict)){
            int random = 1+std::rand()%1025;

            while(!AllowedMons.contains(int(wildPokemonInfo["pokemons"][random]["natdex"])))
                random = 1+std::rand()%1025;

            int formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
            while(wildPokemonInfo["pokemons"][random]["forms"][formRandom]["is_present"] == false){
                formRandom = std::rand()%static_cast<int>(wildPokemonInfo["pokemons"][random]["forms"].size());
            }

            checkDict={{"id", int(random)}, {"form",int(formRandom)}};
        }

        cleanTrainerInfo["values"][protocol[i]]["poke1"]["devId"] = wildPokemonInfo["pokemons"][int(checkDict["id"])]["devName"];
        cleanTrainerInfo["values"][protocol[i]]["poke1"]["formId"] = int(checkDict["form"]);
    }
}

/*
 *
 * # trid == rival_01_hono
# ...._kusa
# ...._mizu
# "trid": "raid_assist_NPC_1->52"
# "trid" : botan - penny
#   - "botan_01" penny fight 1
#   - "botan_02" penny ace_tournament Rematch
#   - "botan_multi" penny AZ
#   - "botan_02_01" Epilogue Multi (Max 3 [Try 6])
#   - "botan_schoolwars" Ace Star - Indigo Disk
# - : chairperson - geeta
#   - "chairperson_01" Geeta - League
#   - "chairperson_02" Geeta - Ace Star
#   - "chairperson_03" Getta - Ace Star Rematch
# - : clavel - clavell
#   - "clavel_01" Clavell - ??? (Unsure)
#   - "clavel_01_hono" Clavell - He has Quaxly
#   - "clavel_01_kusa" Clavell - He has Fuecoco
#   - "clavel_01_mizu" Clavell - He has Sprigatito
#   - "clavel_02_hono" Clavell - He has Quaxly Ace Star
#   - "clavel_02_kusa" Clavell - He has Fuecoco Ace Star
#   - "clavel_02_mizu" Clavell - He has Sprigatito Ace Star
# - : dan_aku_ - Dark Team Star
#   - "dan_aku_01" Dark TS - Outside Fight
#   - "dan_aku_boss_01" Dark TS - Boss (Max is 5)
#   - "dan_aku_boss_02" Dark TS - Boss (Max is 6)
# - : dan_doku_ -  Poison
#   - "dan_doku_01" Poison TS - Outside Fight
#   - "dan_doku_boss_01" (Max is 5)
#   - "dan_doku_boss_02" (Max is 6)
# - : dan_fairy_ - Fairy
#   - "dan_fairy_butler_01" Fairy TS - Outside Fight
#   - "dan_fairy_boss_01" (Max is 5)
#   - "dan_fairy_boss_02" (Max is 6)
# - : dan_hono_ - Fire
#   - "dan_hono_01" Fire TS - Outside Fight
#   - "dan_hono_boss_01" (Max is 5)
#   - "dan_hono_boss_02" (Max is 6)
# - : dan_kakutou - Fighting
#   - "dan_kakutou_01" Fire TS - Outside Fight
#   - "dan_kakutou_boss_01" (Max is 5)
#   - "dan_kakutou_boss_02" (Max is 6)
# - : dan_tr - Tutorial TS
#   - "dan_tr_01" Tutorial TS - Fight 1
#   - "dan_tr_02" Tutorial TS - Fight 2
# - : e4_dragon
#   - "e4_dragon_01" E4 - Dragon - First
#   - "e4_dragon_02" E4 - Dragon - Ace Star
# - : e4_hagane
#   - "e4_hagane_01" E4 - Steel - First
# - : e4_hikou
#   - "e4_hikou_01" E4 - Flying - First
# - : e4_jimen
#   - "e4_jimen_01" E4 - Ground - First
# - : gym_denki
#   - "gym_denki_02" - Electric Trainer
#   - "gym_denki_03" - Electric Trainer
#   - "gym_denki_04" - Electric Trainer
#   - "gym_denki_leader_01" - Leader
#   - "gym_denki_leader_02" - Leader Rematch
# - : gym_esper
#   - "gym_esper_01" - Psychic Trainer
#   - "gym_esper_02" - Psychic Trainer
#   - "gym_esper_leader_01" - Leader
#   - "gym_esper_leader_02" - Leader Rematch
# - : gym_ghost
#   - "gym_ghost_01" - Ghost Trainer
#   - "gym_ghost_02" - Ghost Trainer
#   - "gym_ghost_03" - Ghost Trainer
#   - "gym_ghost_leader_01" - Leader
#   - "gym_ghost_leader_02" - Leader Rematch
# - : gym_koori (Ice)
#   - "gym_koori_leader_01" - Leader
#   - "gym_koori_leader_02" - Leader Rematch
# - : gym_kusa (Grass)
#   - "gym_kusa_leader_01" - Leader
#   - "gym_kusa_leader_02" - Leader Rematch
# - : gym_mizu (Water)
#   - "gym_mizu_01" - Water Trainer
#   - "gym_mizu_leader_01" - Leader
#   - "gym_mizu_leader_02" - Leader Rematch
# - : gym_mushi (Bug)
#   - "gym_mushi_01" - Bug Trainer
#   - "gym_mushi_02" - Bug Trainer
#   - "gym_mushi_03" - Bug Trainer
#   - "gym_mushi_leader_01" - Leader
#   - "gym_mushi_leader_02" - Leader Rematch
# - : gym_normal
#   - "gym_normal_01" - Normal Trainer
#   - "gym_normal_02" - Normal Trainer
#   - "gym_normal_03" - Normal Trainer
#   - "gym_normal_leader_01" - Leader
#   - "gym_normal_leader_02" - Leader Rematch
# - : kihada (Dendra)
#   - "kihada_01" - Ace Star
#   - "kihada_02" - Ace Star Rematch
# - : mimoza (Miriam)
#   - "mimoza_01" - Ace Star
# - : pepper - Arven
#   - "pepper_00" - Lighthouse
#   - "pepper_01" - Lighthouse Final
#   - "pepper_02" - Ace Star
#   - "pepper_03" - Ace Star Rematch
#   - "pepper_multi" - AZ
#   - "pepper_nusi_01" - Titan (No Gem Change)
#   - "pepper_nusi_02" - Titan (No Gem Change)
#   - "pepper_nusi_03" - Titan (No Gem Change)
#   - "pepper_nusi_04" - Titan (No Gem Change)
#   - "pepper_nusi_05" - Titan (No Gem Change)
#   - "pepper_02_01" - Epilogue Multi
#   - "pepper_schoolwars" Ace Star - Indigo Disk
# - : professor_A_01 - Sada
#   - "professor_A_01" - Fight
#   - "professor_A_02" - Koraidon Fight
# - : professor_B_01 - Turo
#   - "professor_B_01" - Fight
#   - "professor_B_02" - Miraidon Fight
# - : rehoru (Raifort)
#   - "rehoru_01" - Ace Star Rematch
# - : richf - O'Nare
#   - "richf_01" - Not Fightable Ignore (O'Nare)
# - : rival - Nemona
    # - : rival_01 (Nemona - cutscene)
    # - : rival_02 (Nemona - cutscene)
    # - : rival_03 (Nemona - cutscene)
    # - : rival_05 (Nemona - cutscene)
    # - : rival_06 (Nemona - cutscene)
    # - : rival_0X_hono (Nemona w/Sprigattito) X [1->8; first 6 are gym, then champion, then Ace Tournament Rematch]
    # - : rival_0X_kusa (Nemona w/Quaxly)
    # - : rival_0X_mizu (Nemona w/ Fuecoco)
    # - : rival_multi_hono (Nemona A0 - Sprigatito)
    # - : rival_multi_kusa (Nemona A0 - Quaxly)
    # - : rival_multi_mizu (Nemona A0 - Fuecoco)
    # - : rival_02_01hono (Nemona Epilogue - Sprigatito)
    # - : rival_02_01kusa (Nemona Epilogue - Quaxly)
    # - : rival_02_01mizu (Nemona Epilogue - Fuecoco)
    # - : rival_schoolwars_hono (Nemona Ace - Epilogue - Sprigatito)
    # - : rival_schoolwars_kusa (Nemona Ace - Epilogue - Quaxly)
    # - : rival_schoolwars_mizu (Nemona Ace - Epilogue - Fuecoco)
# - : sawaro (Saguaro)
#   - "sawaro_01" - Ace Star Rematch
# - : seizi (Salvatore)
#   - "seizi_01" - Ace Star Rematch
# - : strong_01 (Garchomp - Ignore)
# ---------------DLC 1 Starts here---------------
# - : Brother (Kieran - SU1)
#   - "brother_01_01" - First Battle - Not Complete
#   - "brother_01_01_strong" - First Battle - Complete
#   - "brother_01_02" - Second Battle - Not Complete
#   - "brother_01_02_strong" - Second Battle - Complete
#   - "brother_01_03" - Third Battle - Not Complete
#   - "brother_01_03_strong" - Third Battle - Complete
#   - "brother_01_04" - Fourth Battle - Not Complete - Loyalty Plaza
#   - "brother_01_04_strong" - Fourth Battle - Complete - Loyalty Plaza
#   - "brother_01_05" - Fourth Battle - Not Complete - Fight For Ogerpon
#   - "brother_01_05_strong" - Fourth Battle - Complete - Fight For Ogerpon
#   - "brother_02_01" - DLC Champion Fight
#   - "brother_02_02" - Terapagos Fight (Ignore)
#   - "brother_kodaigame" - Multi Terapagos
#   - "s2_side_brother" - Multi Epilogue
# - : Camera (Perrin)
#   - "camera_01_01" - Fight
# - : serebu (O'Nare)
#   - "serebu_01" - First Fight
#   - "serebu_02" - Second Fight
# - : O'Nare Wife
#   - "serevy_03" - First Fight
# - : sister (Carmine)
#   - "sister_01_01" - First Fight - Not Complete
#   - "sister_01_01_strong" - First Fight - Complete
#   - "sister_01_02" - Second Fight - Not Complete
#   - "sister_01_02_strong" - Second Fight - Complete
#   - "sister_01_03" - Third Fight - Not Complete
#   - "sister_01_03_strong" - Third Fight - Complete
#   - "sister_muruchi_01" - Multi (Milotic) - Not Complete
#   - "sister_muruchi_01_strong" - Multi (Milotic) - Complete
#   - "sister_onitaizi" - Multi (Titan Legend) - Not Complete
#   - "sister_onitaizi_strong" - Multi (Titan Legend) - Complete
#   - "sister_02_01" - Aquarium Fight
#   - "sister_02_02" - Terapagos Multi
# - : sp_trainer (Ogre Clan)
#   - "sp_trainer_0X" - Ogre Member (X = 1->7)
#   - "sp_trainer_boss" - Ogre Boss
# ---------------DLC 2 Starts here---------------
# - : dragon4 (BB4)
#   - "dragon4_02_01" - BB Dragon Fight
# - : dragonchallenge
#   - "dragonchallenge_01" - BB Dragon Challenge
#   - "dragonchallenge_02" - BB Dragon Challenge
#   - "dragonchallenge_03" - BB Dragon Challenge
# - : fairy4
#   - "fairy4_02_01" - School Yard Fight
#   - "fairy4_02_01" - BB Fairy Fight
# - : fairychallenge
#   - "fairychallenge_0X" - BB Fairy Challenge [X = 1->5]
# - : s2_side_grandfather - Epilogue
# - : s2_side_grandmother - Epilogue
# - : hagane4
#   - "hagane4_02_01" - BB Steel Fight
# - : hono4
#   - "hono4_02_01" - BB Fire Fight
# - : honochallenge
#   - "honochallenge_01" - BB Fire Challenge
#   - "honochallenge_02" - BB Fire Challenge
#   - "honochallenge_03" - BB Fire Challenge
# - : shiano (Citrano)
#   - "shiano" - BBLeauge Fight
# - : su2_bukatu (bbleauge)
#   - "su2_bukatu_akamatu" - Crispin
#   - "su2_bukatu_botan" - Penny (Also Ace Star)
#   - "su2_bukatu_claver_honoo" - Clavell with Quaxly
#   - "su2_bukatu_claver_kusa" - Clavell with Fuecoco
#   - "su2_bukatu_claver_mizu" - Clavell with Sprigatito
#   - "su2_bukatu_denki" - Electric Leader
#   - "su2_bukatu_doragon" - Dragon E4
#   - "su2_bukatu_esper" - Psychic Leader
#   - "su2_bukatu_ghost" - Ghost Leader
#   - "su2_bukatu_hagane" - Steel E4
#   - "su2_bukatu_hikou" - Normal Leader
#   - "su2_bukatu_kakitubata" - Dragon BBE4
#   - "su2_bukatu_kihada" - Dendra
#   - "su2_bukatu_Koori" - Grusha
#   - "su2_bukatu_kusa" - Braissius
#   - "su2_bukatu_mimoza" - Miriam
#   - "su2_bukatu_mizu" - Kofu
#   - "su2_bukatu_mushi" - Katy
#   - "su2_bukatu_nemo_honoo" - Nemona w/Sprigatito
#   - "su2_bukatu_nemo_kusa"  - Nemona w/Quaxly
#   - "su2_bukatu_nemo_mizu"  - Nemona w/Fuecoco
#   - "su2_bukatu_nerine" - Steel BBE4
#   - "su2_bukatu_omodaka" - Geeta
#   - "su2_bukatu_pepa" - Arven
#   - "su2_bukatu_rehool" - Raifort
#   - "su2_bukatu_sawaro" - Saguaro
#   - "su2_bukatu_seizi" - Salvatore
#   - "su2_bukatu_suguri" - Kieran
#   - "su2_bukatu_taro" - Lacey
#   - "su2_bukatu_time" - Tyme
#   - "su2_bukatu_zeiyu" - Carmine
#   - "su2_bukatu_zimen" - Rika
#   - "su2_bukatu_zinia" - Jacq
# - : s2_side_villager01
# - : s2_side_villager02
# - : taimu (Ryme)
#   - "taimu_01" - Star Ace Rematch
# - : zinia (Bio Teacher)
#   - "zinia_01" - Star Ace
#   - "zinia_02" - Star Ace Rematch
# botan_01 has index [356]
# botan_02 has index [357]
# botan_multi has index [358]
# botan_02_01 has index [677]
# botan_schoolwars has index [678]
# chairperson_01 has index [359]
# chairperson_02 has index [360]
# chairperson_03 has index [361]
# clavel_01 has index [362]
# clavel_01_hono has index [363]
# clavel_01_kusa has index [364]
# clavel_01_mizu has index [365]
# clavel_02_hono has index [366]
# clavel_02_kusa has index [367]
# clavel_02_mizu has index [368]
# dan_aku_01 has index [369]
# dan_aku_boss_01 has index [370]
# dan_aku_boss_02 has index [371]
# dan_doku_01 has index [372]
# dan_doku_boss_01 has index [373]
# dan_doku_boss_02 has index [374]
# dan_fairy_butler_01 has index [377]
# dan_fairy_boss_01 has index [375]
# dan_fairy_boss_02 has index [376]
# dan_hono_01 has index [378]
# dan_hono_boss_01 has index [379]
# dan_hono_boss_02 has index [380]
# dan_kakutou_01 has index [381]
# dan_kakutou_boss_01 has index [382]
# dan_kakutou_boss_02 has index [383]
# dan_tr_01 has index [384]
# dan_tr_02 has index [385]
# e4_dragon_01 has index [386]
# e4_dragon_02 has index [387]
# e4_hagane_01 has index [388]
# e4_hikou_01 has index [389]
# e4_jimen_01 has index [390]
# gym_denki_01 has index [391]
# gym_denki_02 has index [392]
# gym_denki_03 has index [393]
# gym_denki_04 has index [394]
# gym_denki_leader_01 has index [395]
# gym_denki_leader_02 has index [396]
# gym_esper_01 has index [397]
# gym_esper_02 has index [398]
# gym_esper_leader_01 has index [399]
# gym_esper_leader_02 has index [400]
# gym_ghost_01 has index [401]
# gym_ghost_02 has index [402]
# gym_ghost_03 has index [403]
# gym_ghost_leader_01 has index [404]
# gym_ghost_leader_02 has index [405]
# gym_koori_leader_01 has index [406]
# gym_koori_leader_02 has index [407]
# gym_kusa_leader_01 has index [408]
# gym_kusa_leader_02 has index [409]
# gym_mizu_01 has index [410]
# gym_mizu_leader_01 has index [411]
# gym_mizu_leader_02 has index [412]
# gym_mushi_01 has index [413]
# gym_mushi_02 has index [414]
# gym_mushi_03 has index [415]
# gym_mushi_leader_01 has index [416]
# gym_mushi_leader_02 has index [417]
# gym_normal_01 has index [418]
# gym_normal_02 has index [419]
# gym_normal_03 has index [420]
# gym_normal_leader_01 has index [421]
# gym_normal_leader_02 has index [422]
# kihada_01 has index [423]
# kihada_02 has index [424]
# mimoza_01 has index [425]
# pepper_00 has index [426]
# pepper_01 has index [427]
# pepper_02 has index [428]
# pepper_03 has index [429]
# pepper_multi has index [430]
# pepper_nusi_01 has index [431]
# pepper_nusi_02 has index [432]
# pepper_nusi_03 has index [433]
# pepper_nusi_04 has index [434]
# pepper_nusi_05 has index [435]
# pepper_02_01 has index [701]
# pepper_schoolwars has index [702]
# professor_A_01 has index [436]
# professor_A_02 has index [437]
# professor_B_01 has index [438]
# professor_B_02 has index [439]
# rehoru_01 has index [492]
# richf_01 has index [493] - Not fightable
# rival_01 has index [494]
# rival_02 has index [498]
# rival_03 has index [502]
# rival_05 has index [509]
# rival_06 has index [513]
# rival_01_hono has index [495]
# rival_02_hono has index [499]
# rival_03_hono has index [503]
# rival_04_hono has index [506]
# rival_05_hono has index [510]
# rival_06_hono has index [514]
# rival_07_hono has index [517]
# rival_08_hono has index [520]
# rival_01_kusa has index [496]
# rival_02_kusa has index [500]
# rival_03_kusa has index [504]
# rival_04_kusa has index [507]
# rival_05_kusa has index [511]
# rival_06_kusa has index [515]
# rival_07_kusa has index [518]
# rival_08_kusa has index [521]
# rival_01_mizu has index [497]
# rival_02_mizu has index [501]
# rival_03_mizu has index [505]
# rival_04_mizu has index [508]
# rival_05_mizu has index [512]
# rival_06_mizu has index [516]
# rival_07_mizu has index [519]
# rival_08_mizu has index [522]
# rival_multi_hono has index [523]
# rival_multi_kusa has index [524]
# rival_multi_mizu has index [525]
# rival_02_01hono has index [703]
# rival_02_01kusa has index [704]
# rival_02_01mizu has index [705]
# rival_schoolwars_hono has index [706]
# rival_schoolwars_kusa has index [707]
# rival_schoolwars_mizu has index [708]
# sawaro_01 has index [526]
# seizi_01 has index [527]
# strong_01 has index [528] - Not fightable
# brother_01_01 has index [585]
# brother_01_02 has index [587]
# brother_01_03 has index [589]
# brother_01_04 has index [591]
# brother_01_05 has index [593]
# brother_01_01_strong has index [586]
# brother_01_02_strong has index [588]
# brother_01_03_strong has index [590]
# brother_01_04_strong has index [592]
# brother_01_05_strong has index [594]
# brother_02_01 has index [679]
# brother_02_02 has index [680]
# brother_kodaigame has index [681]
# s2_side_brother has index [682]
# camera_01_01 has index [595]
# serebu_01 has index [596]
# serebu_02 has index [597]
# serevy_03 has index [598]
# sister_01_01 has index [599]
# sister_01_02 has index [601]
# sister_01_03 has index [603]
# sister_01_01_strong has index [600]
# sister_01_02_strong has index [602]
# sister_01_03_strong has index [604]
# sister_muruchi_01 has index [605]
# sister_muruchi_01_strong has index [606]
# sister_onitaizi has index [607]
# sister_onitaizi_strong has index [608]
# sister_02_01 has index [710]
# sister_02_02 has index [711]
# sp_trainer_01 has index [609]
# sp_trainer_02 has index [610]
# sp_trainer_03 has index [611]
# sp_trainer_04 has index [612]
# sp_trainer_05 has index [613]
# sp_trainer_06 has index [614]
# sp_trainer_07 has index [615]
# sp_trainer_boss has index [616]
# dragon4_02_01 has index [683]
# dragonchallenge_01 has index [684]
# dragonchallenge_02 has index [685]
# dragonchallenge_03 has index [686]
# fairy4_02_01 has index [687]
# fairy4_02_02 has index [688]
# fairychallenge_01 has index [689]
# fairychallenge_02 has index [690]
# fairychallenge_03 has index [691]
# fairychallenge_04 has index [692]
# fairychallenge_05 has index [693]
# s2_side_grandfather has index [694]
# s2_side_grandmother has index [695]
# hagane4_02_01 has index [696]
# hono4_02_01 has index [697]
# honochallenge_01 has index [698]
# honochallenge_02 has index [699]
# honochallenge_03 has index [700]
# shiano has index [709]
# su2_bukatu_akamatu has index [712]
# su2_bukatu_botan has index [713]
# su2_bukatu_claver_honoo has index [714]
# su2_bukatu_claver_kusa has index [715]
# su2_bukatu_claver_mizu has index [716]
# su2_bukatu_denki has index [717]
# su2_bukatu_doragon has index [718]
# su2_bukatu_esper has index [719]
# su2_bukatu_ghost has index [720]
# su2_bukatu_hagane has index [721]
# su2_bukatu_hikou has index [722]
# su2_bukatu_kakitubata has index [723]
# su2_bukatu_kihada has index [724]
# su2_bukatu_Koori has index [725]
# su2_bukatu_kusa has index [726]
# su2_bukatu_mimoza has index [727]
# su2_bukatu_mizu has index [728]
# su2_bukatu_mushi has index [729]
# su2_bukatu_nemo_honoo has index [730]
# su2_bukatu_nemo_kusa has index [731]
# su2_bukatu_nemo_mizu has index [732]
# su2_bukatu_nerine has index [733]
# su2_bukatu_omodaka has index [734]
# su2_bukatu_pepa has index [735]
# su2_bukatu_rehool has index [736]
# su2_bukatu_sawaro has index [737]
# su2_bukatu_seizi has index [738]
# su2_bukatu_suguri has index [739]
# su2_bukatu_taro has index [740]
# su2_bukatu_time has index [741]
# su2_bukatu_zeiyu has index [742]
# su2_bukatu_zimen has index [743]
# su2_bukatu_zinia has index [744]
# s2_side_villager01 has index [745]
# s2_side_villager02 has index [746]
# taimu_01 has index [747]
# zinia_01 has index [754]
# zinia_02 has index [755]
# NOTE: Ace Tournament and BBLeague Share Pokemon Teams
 *
 */
