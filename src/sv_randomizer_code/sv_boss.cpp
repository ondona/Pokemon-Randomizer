#include <QDebug>
#include <QVector>
#include <QString>
#include <QStringListModel>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <QDir>
#include "headers/sv_randomizer_headers/sv_boss.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

QVector<int> allowedPokemonBoss;
QVector<int> allowedLegendsBoss;
json cleanBossData;
json bossMappingInfo;
json trainerInfo;
QDir BaseDir;

void SVBoss::saveIndividualPokemon(int index, std::vector<int> &dev, std::vector<int> &form, std::vector<int> &genderV, std::vector<bool> &rareV){
    dev.push_back(bossMappingInfo["pokemons_dev"][cleanBossData["values"][index]["pokeData"]["devId"]]["devid"]);
    form.push_back(cleanBossData["values"][index]["pokeData"]["formId"]);
    std::string gender = cleanBossData["values"][index]["pokeData"]["sex"];
    if(gender != "FEMALE"){
        genderV.push_back(0);
    }
    else{
        genderV.push_back(1);
    }

    std::string rare = cleanBossData["values"][index]["pokeData"]["rareType"];
    if(gender != "RARE"){
        rareV.push_back(0);
    }
    else{
        rareV.push_back(1);
    }
}

void SVBoss::savePokemonFromTrainer(int index, std::string& pokeKey, std::vector<int> &dev, std::vector<int> &form, std::vector<int> &genderV, std::vector<bool> &rareV){
    dev.push_back(bossMappingInfo["pokemons_dev"][trainerInfo["values"][index][pokeKey]["devId"].get<std::string>().c_str()]["devid"]);
    form.push_back(trainerInfo["values"][index][pokeKey]["formId"]);
    std::string gender = trainerInfo["values"][index][pokeKey]["sex"].get<std::string>().c_str();
    if(gender != "FEMALE"){
        genderV.push_back(0);
    }
    else{
        genderV.push_back(1);
    }

    std::string rare = trainerInfo["values"][index][pokeKey]["rareType"].get<std::string>().c_str();
    if(gender != "RARE"){
        rareV.push_back(0);
    }
    else{
        rareV.push_back(1);
    }
}

void SVBoss::obtainPokemonScene(int &dev, int &form, int& gender, int &rare){
    dev = 1+std::rand()%1025;
    while(!allowedPokemonBoss.contains(bossMappingInfo["pokemons"][dev]["natdex"]))
        dev = 1+std::rand()%1025;

    form = std::rand()%static_cast<int>(bossMappingInfo["pokemons"][dev]["forms"].size());
    while(bossMappingInfo["pokemons"][dev]["forms"][form]["is_present"] == false){
        form = std::rand()%static_cast<int>(bossMappingInfo["pokemons"][dev]["forms"].size());
    }

    QString form_Check = QString::fromUtf8(bossMappingInfo["pokemons"][dev]["name"].get<std::string>().c_str());
    if(maleOnlyPokemon.contains(form_Check) || genderlessPokemon.contains(form_Check)){

        if(genderlessPokemon.contains(form_Check)){
            gender = 0;
        }else{
            gender = 0;
        }
    }else if(femaleOnlyPokemon.contains(form_Check)){
        gender = 1;
    } else if(genderForms.contains(form_Check)){
        if(form == 0){

            gender = 0;
        }else{

            gender = 1;
        }
    }else{
        int rand_gender = std::rand()%50;
        if(rand_gender < 24){
            gender = 0;
        }else{

            gender = 1;
        }
    }

    int val = 1+std::rand()%100; // range is [1, shiny_starter_rate)
    if(val == 1){
        rare = 1;
    }
    else{
        rare = 0;
    }

    //set dev to devid
    dev = bossMappingInfo["pokemons"][dev]["devid"];

}

void SVBoss::changeSceneOne(std::vector<std::pair<std::string, std::string>> filePairs,
                         std::vector<int> &dev, std::vector<int> &form, std::vector<int> &gender, std::vector<bool> &rare,
                         std::string output, std::string romAddress){

    for (const auto& filePair : filePairs) {
        this->modifyPokemonScene(dev, form, gender, rare, filePair.first, filePair.second);
    }

    generateBinary(BaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                   BaseDir.filePath("SV_SCENES/"+QString::fromStdString(output)).toStdString(),
                   romAddress);
}

void SVBoss::changeScene(std::vector<std::pair<std::string, std::string>> filePairs,
                 std::vector<int> &dev, std::vector<int> &form, std::vector<int> &gender, std::vector<bool> &rare,
                         std::string output, std::string romAddress){

    for (const auto& filePair : filePairs) {
        this->modifyPokemonScene(dev, form, gender, rare, filePair.first, filePair.second);
    }

    generateBinary(BaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                   BaseDir.filePath("SV_SCENES/"+QString::fromStdString(output)+"_0.json").toStdString(),
                   romAddress);

    generateBinary(BaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                   BaseDir.filePath("SV_SCENES/"+QString::fromStdString(output)+"_1.json").toStdString(),
                   romAddress);
}

void SVBoss::patchMultiBattle(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;
    // Glimmora - 0 - 1055
    saveIndividualPokemon(0, devIdBoss, formIdBoss, genderBoss, rareBoss);

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_AZ/WayHome/common_1055_main_0_clean.json", "SV_AZ/WayHome/common_1055_main_0.json"},
        {"SV_AZ/WayHome/common_1055_main_1_clean.json", "SV_AZ/WayHome/common_1055_main_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_AZ/WayHome/common_1055_main",
                "world/scene/parts/event/event_scenario/main_scenario/common_1055_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();

    // Scream Tails - 1 - 1075
    // Iron Bundle - 2 - 1075
    saveIndividualPokemon(1, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(2, devIdBoss, formIdBoss, genderBoss, rareBoss);

    filePairs = {
        {"SV_AZ/WayHome/common_1075_main_0_clean.json", "SV_AZ/WayHome/common_1075_main_0.json"},
        {"SV_AZ/WayHome/common_1075_main_1_clean.json", "SV_AZ/WayHome/common_1075_main_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_AZ/WayHome/common_1075_main",
                "world/scene/parts/event/event_scenario/main_scenario/common_1075_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();

    // Great Tusk - 3 - 1095
    // Iron Treads - 4 - 1095
    saveIndividualPokemon(3, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(4, devIdBoss, formIdBoss, genderBoss, rareBoss);

    filePairs = {
        {"SV_AZ/WayHome/common_1095_main_0_clean.json", "SV_AZ/WayHome/common_1095_main_0.json"},
        {"SV_AZ/WayHome/common_1095_main_1_clean.json", "SV_AZ/WayHome/common_1095_main_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_AZ/WayHome/common_1095_main",
                "world/scene/parts/event/event_scenario/main_scenario/common_1095_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();


    // 6 6 5 6 6 7 6 5 - 1170
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(5, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(7, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(6, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(5, devIdBoss, formIdBoss, genderBoss, rareBoss);

    filePairs = {
        {"SV_AZ/WayHome/common_1170_always_0_clean.json", "SV_AZ/WayHome/common_1170_always_0.json"},
    };
    changeSceneOne(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_AZ/WayHome/common_1170_always_0.json",
                "world/scene/parts/event/event_scenario/main_scenario/common_1170_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();

    // 9 9 8 9 9 10 8 9 -- 1170
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(8, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(10, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(8, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(9, devIdBoss, formIdBoss, genderBoss, rareBoss);

    filePairs = {
                 {"SV_AZ/WayHome/common_1170_always_1_clean.json", "SV_AZ/WayHome/common_1170_always_1.json"},
                 };
    changeSceneOne(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                   "SV_AZ/WayHome/common_1170_always_1.json",
                   "world/scene/parts/event/event_scenario/main_scenario/common_1170_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();

}

void SVBoss::patchGimmighoul(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;
    // Gimmighoul - 11
    saveIndividualPokemon(11, devIdBoss, formIdBoss, genderBoss, rareBoss);

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"MISC/Gimmighoul/coin_symbol_walk_0_clean.json", "MISC/Gimmighoul/coin_symbol_walk_0.json"},
        {"MISC/Gimmighoul/coin_symbol_walk_1_clean.json", "MISC/Gimmighoul/coin_symbol_walk_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "MISC/Gimmighoul/coin_symbol_walk",
                "world/obj_template/parts/coin_symbol/coin_symbol_walk_/");

    filePairs.clear();

    filePairs = {
        {"MISC/Gimmighoul/coin_symbol_box_0_clean.json", "MISC/Gimmighoul/coin_symbol_box_0.json"},
        {"MISC/Gimmighoul/coin_symbol_box_1_clean.json", "MISC/Gimmighoul/coin_symbol_box_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "MISC/Gimmighoul/coin_symbol_box",
                "world/obj_template/parts/coin_symbol/coin_symbol_box_/");


    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

}

void SVBoss::patchLeChonk(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Set Lechonk
    for(int i =0; i<=4; i++){
        devIdBoss.push_back(bossMappingInfo["pokemons_dev"][cleanBossData["values"][24]["pokeData"]["devId"]]["devid"]);
        formIdBoss.push_back(cleanBossData["values"][24]["pokeData"]["formId"]);
        std::string gender = cleanBossData["values"][24]["pokeData"]["sex"];
        if(gender != "FEMALE"){
            genderBoss.push_back(0);
        }
        else{
            genderBoss.push_back(1);
        }

        std::string rare = cleanBossData["values"][24]["pokeData"]["rareType"];
        if(gender != "RARE"){
            rareBoss.push_back(0);
        }
        else{
            rareBoss.push_back(1);
        }
    }

    // Tarantoula
    int devid = 1;
    int form = 0;
    int gender = 0;
    int rare = 0;
    obtainPokemonScene(devid, form, gender, rare);

    for(int i = 5; i<=8; i++){
        devIdBoss.push_back(devid);
        formIdBoss.push_back(form);
        genderBoss.push_back(gender);
        rareBoss.push_back(rare);
    }

    //Pawmi - 9
    obtainPokemonScene(devid, form, gender, rare);
    devIdBoss.push_back(devid);
    formIdBoss.push_back(form);
    genderBoss.push_back(gender);
    rareBoss.push_back(rare);

    // Fletchling
    obtainPokemonScene(devid, form, gender, rare);
    for(int i = 10; i<=11; i++){
        devIdBoss.push_back(devid);
        formIdBoss.push_back(form);
        genderBoss.push_back(gender);
        rareBoss.push_back(rare);
    }

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_FIRST_ROUTE/common_0100_main_0_clean.json", "SV_FIRST_ROUTE/common_0100_main_0.json"},
        {"SV_FIRST_ROUTE/common_0100_main_1_clean.json", "SV_FIRST_ROUTE/common_0100_main_1.json"}
    };

    for (const auto& filePair : filePairs) {
        this->modifyPokemonScene(devIdBoss, formIdBoss, genderBoss, rareBoss, filePair.first, filePair.second);
    }

    generateBinary(BaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                   BaseDir.filePath(+"SV_SCENES/SV_FIRST_ROUTE/common_0100_main_0.json").toStdString(),
                   "world/scene/parts/event/event_scenario/main_scenario/common_0100_/");

    generateBinary(BaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                   BaseDir.filePath(+"SV_SCENES/SV_FIRST_ROUTE/common_0100_main_1.json").toStdString(),
                   "world/scene/parts/event/event_scenario/main_scenario/common_0100_/");
}

void SVBoss::patchHoundoom(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;
    // Houndoom - 25 - 1050
    saveIndividualPokemon(25, devIdBoss, formIdBoss, genderBoss, rareBoss);

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"MISC/Houndoom/common_0150_main_0_clean.json", "MISC/Houndoom/common_0150_main_0.json"},
        {"MISC/Houndoom/common_0150_main_1_clean.json", "MISC/Houndoom/common_0150_main_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "MISC/Houndoom/common_0150_main",
                "world/scene/parts/event/event_scenario/main_scenario/common_0150_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::patchSunflora(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Sunflora - 26
    for(int i =0; i<=29; i ++)
        saveIndividualPokemon(26, devIdBoss, formIdBoss, genderBoss, rareBoss);

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"MISC/Sunflora/pokes_0_clean.json", "MISC/Sunflora/pokes_0.json"},
        {"MISC/Sunflora/pokes_1_clean.json", "MISC/Sunflora/pokes_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "MISC/Sunflora/pokes",
                "world/scene/parts/event/event_scenario/main_scenario/gym_kusa_poke_finding_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();

}

void SVBoss::patchDonzoTitan(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Donzo - 33
    saveIndividualPokemon(33, devIdBoss, formIdBoss, genderBoss, rareBoss);

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_always_0_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_always_0.json"},
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_always_1_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_always_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_always",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_010_0_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_010_0.json"},
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_010_1_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_010_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_010",
                "world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_010_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_020_0_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_020_0.json"},
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_020_1_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_020_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_020",
                "world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_020_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();

    // Tatsugiri - 37
    saveIndividualPokemon(37, devIdBoss, formIdBoss, genderBoss, rareBoss);

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_010_0_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_010_0.json"},
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_010_1_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_010_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_010",
                "world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_010_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_020_0_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_020_0.json"},
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_020_1_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_020_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_020",
                "world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Dondozo_Tatsu/sub_040_pre_start_0_clean.json", "SV_TITANS/Dondozo_Tatsu/sub_040_pre_start_0.json"},
        {"SV_TITANS/Dondozo_Tatsu/sub_040_pre_start_1_clean.json", "SV_TITANS/Dondozo_Tatsu/sub_040_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Dondozo_Tatsu/sub_040_pre_start",
                "world/scene/parts/event/event_scenario/sub_scenario/sub_040_/");

    filePairs.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    for(int i =0; i<=25; i++)
        saveIndividualPokemon(37, devIdBoss, formIdBoss, genderBoss, rareBoss);

    filePairs = {
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_pre_start_0_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_pre_start_0.json"},
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_pre_start_1_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/");

    filePairs.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(33, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(37, devIdBoss, formIdBoss, genderBoss, rareBoss);
    std::string pokeKy = "poke1";
    savePokemonFromTrainer(435, pokeKy, devIdBoss, formIdBoss, genderBoss, rareBoss);
    //saveIndividualPokemon(37, devIdBoss, formIdBoss, genderBoss, rareBoss);

    filePairs = {
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_020_pre_start_0_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_020_pre_start_0.json"},
        {"SV_TITANS/Dondozo_Tatsu/nushi_dragon_020_pre_start_1_clean.json", "SV_TITANS/Dondozo_Tatsu/nushi_dragon_020_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Dondozo_Tatsu/nushi_dragon_020_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_020_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::patchOrthwormTitan(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Orthworm - 35
    saveIndividualPokemon(35, devIdBoss, formIdBoss, genderBoss, rareBoss);

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_TITANS/Orthworm/nushi_hagane_010_pre_start_0_clean.json", "SV_TITANS/Orthworm/nushi_hagane_010_pre_start_0.json"},
        {"SV_TITANS/Orthworm/nushi_hagane_010_pre_start_1_clean.json", "SV_TITANS/Orthworm/nushi_hagane_010_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Orthworm/nushi_hagane_010_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_010_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Orthworm/nushi_hagane_020_pre_start_0_clean.json", "SV_TITANS/Orthworm/nushi_hagane_020_pre_start_0.json"},
        {"SV_TITANS/Orthworm/nushi_hagane_020_pre_start_1_clean.json", "SV_TITANS/Orthworm/nushi_hagane_020_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Orthworm/nushi_hagane_020_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Orthworm/nushi_hagane_fp_1048_010_0_clean.json", "SV_TITANS/Orthworm/nushi_hagane_fp_1048_010_0.json"},
        {"SV_TITANS/Orthworm/nushi_hagane_fp_1048_010_1_clean.json", "SV_TITANS/Orthworm/nushi_hagane_fp_1048_010_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Orthworm/nushi_hagane_fp_1048_010",
                "world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_010_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Orthworm/nushi_hagane_fp_1048_020_0_clean.json", "SV_TITANS/Orthworm/nushi_hagane_fp_1048_020_0.json"},
        {"SV_TITANS/Orthworm/nushi_hagane_fp_1048_020_1_clean.json", "SV_TITANS/Orthworm/nushi_hagane_fp_1048_020_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Orthworm/nushi_hagane_fp_1048_020",
                "world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Orthworm/sub_039_pre_start_0_clean.json", "SV_TITANS/Orthworm/sub_039_pre_start_0.json"},
        {"SV_TITANS/Orthworm/sub_039_pre_start_1_clean.json", "SV_TITANS/Orthworm/sub_039_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Orthworm/sub_039_pre_start",
                "world/scene/parts/event/event_scenario/sub_scenario/sub_039_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::patchBombariderTitan(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Bombardier - 41
    saveIndividualPokemon(41, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(41, devIdBoss, formIdBoss, genderBoss, rareBoss);

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_TITANS/Bombirdier/HikoNushi_0_clean.json", "SV_TITANS/Bombirdier/HikoNushi_0.json"},
        {"SV_TITANS/Bombirdier/HikoNushi_1_clean.json", "SV_TITANS/Bombirdier/HikoNushi_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Bombirdier/HikoNushi",
                "world/scene/parts/field/field_contents/nushi/hiko/HikoNushi_/");

    filePairs.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(41, devIdBoss, formIdBoss, genderBoss, rareBoss);

    filePairs = {
        {"SV_TITANS/Bombirdier/nushi_hikou_010_pre_start_0_clean.json", "SV_TITANS/Bombirdier/nushi_hikou_010_pre_start_0.json"},
        {"SV_TITANS/Bombirdier/nushi_hikou_010_pre_start_1_clean.json", "SV_TITANS/Bombirdier/nushi_hikou_010_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Bombirdier/nushi_hikou_010_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_010_/");

    filePairs.clear();

    filePairs = {
        {"SV_TITANS/Bombirdier/nushi_hikou_020_pre_start_0_clean.json", "SV_TITANS/Bombirdier/nushi_hikou_020_pre_start_0.json"},
        {"SV_TITANS/Bombirdier/nushi_hikou_020_pre_start_1_clean.json", "SV_TITANS/Bombirdier/nushi_hikou_020_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Bombirdier/nushi_hikou_020_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Bombirdier/nushi_hikou_020_main_0_clean.json", "SV_TITANS/Bombirdier/nushi_hikou_020_main_0.json"},
        {"SV_TITANS/Bombirdier/nushi_hikou_020_main_1_clean.json", "SV_TITANS/Bombirdier/nushi_hikou_020_main_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Bombirdier/nushi_hikou_020_main",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Bombirdier/nushi_hiko_fp_1063_010_0_clean.json", "SV_TITANS/Bombirdier/nushi_hiko_fp_1063_010_0.json"},
        {"SV_TITANS/Bombirdier/nushi_hiko_fp_1063_010_1_clean.json", "SV_TITANS/Bombirdier/nushi_hiko_fp_1063_010_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Bombirdier/nushi_hiko_fp_1063_010",
                "world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_010_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Bombirdier/nushi_hiko_fp_1063_020_0_clean.json", "SV_TITANS/Bombirdier/nushi_hiko_fp_1063_020_0.json"},
        {"SV_TITANS/Bombirdier/nushi_hiko_fp_1063_020_1_clean.json", "SV_TITANS/Bombirdier/nushi_hiko_fp_1063_020_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Bombirdier/nushi_hiko_fp_1063_020",
                "world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Bombirdier/sub_038_pre_start_0_clean.json", "SV_TITANS/Bombirdier/sub_038_pre_start_0.json"},
        {"SV_TITANS/Bombirdier/sub_038_pre_start_1_clean.json", "SV_TITANS/Bombirdier/sub_038_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Bombirdier/sub_038_pre_start",
                "world/scene/parts/event/event_scenario/sub_scenario/sub_038_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::patchKlawfTitan(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Bombardier - 41
    saveIndividualPokemon(43, devIdBoss, formIdBoss, genderBoss, rareBoss);

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_TITANS/Klawf/nushi_iwa_010_pre_start_0_clean.json", "SV_TITANS/Klawf/nushi_iwa_010_pre_start_0.json"},
        {"SV_TITANS/Klawf/nushi_iwa_010_pre_start_1_clean.json", "SV_TITANS/Klawf/nushi_iwa_010_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Klawf/nushi_iwa_010_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_010_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Klawf/nushi_iwa_020_pre_start_0_clean.json", "SV_TITANS/Klawf/nushi_iwa_020_pre_start_0.json"},
        {"SV_TITANS/Klawf/nushi_iwa_020_pre_start_1_clean.json", "SV_TITANS/Klawf/nushi_iwa_020_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Klawf/nushi_iwa_020_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Klawf/nushi_iwa_fp_1066_0_clean.json", "SV_TITANS/Klawf/nushi_iwa_fp_1066_0.json"},
        {"SV_TITANS/Klawf/nushi_iwa_fp_1066_1_clean.json", "SV_TITANS/Klawf/nushi_iwa_fp_1066_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Klawf/nushi_iwa_fp_1066",
                "world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_/");
    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Klawf/nushi_iwa_fp_1066_020_0_clean.json", "SV_TITANS/Klawf/nushi_iwa_fp_1066_020_0.json"},
        {"SV_TITANS/Klawf/nushi_iwa_fp_1066_020_1_clean.json", "SV_TITANS/Klawf/nushi_iwa_fp_1066_020_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Klawf/nushi_iwa_fp_1066_020",
                "world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Klawf/sub_037_pre_start_0_clean.json", "SV_TITANS/Klawf/sub_037_pre_start_0.json"},
        {"SV_TITANS/Klawf/sub_037_pre_start_1_clean.json", "SV_TITANS/Klawf/sub_037_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Klawf/sub_037_pre_start",
                "world/scene/parts/event/event_scenario/sub_scenario/sub_037_/");


    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::patchGreatIronTitan(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Great Tusk - 45
    // Iron Treads - 47
    saveIndividualPokemon(47, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(45, devIdBoss, formIdBoss, genderBoss, rareBoss);

    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_TITANS/Great_Iron/nushi_jimen_010_pre_start_0_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_010_pre_start_0.json"},
        {"SV_TITANS/Great_Iron/nushi_jimen_010_pre_start_1_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_010_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Great_Iron/nushi_jimen_010_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_010_/");

    filePairs.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    saveIndividualPokemon(45, devIdBoss, formIdBoss, genderBoss, rareBoss);
    saveIndividualPokemon(47, devIdBoss, formIdBoss, genderBoss, rareBoss);

    filePairs = {
        {"SV_TITANS/Great_Iron/nushi_jimen_020_pre_start_0_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_020_pre_start_0.json"},
        {"SV_TITANS/Great_Iron/nushi_jimen_020_pre_start_1_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_020_pre_start_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Great_Iron/nushi_jimen_020_pre_start",
                "world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_020_/");

    filePairs.clear();
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    // Great Tusk Only
    saveIndividualPokemon(45, devIdBoss, formIdBoss, genderBoss, rareBoss);
    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Great_Iron/nushi_jimen_fp_1082_010_0_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_fp_1082_010_0.json"},
        {"SV_TITANS/Great_Iron/nushi_jimen_fp_1082_010_1_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_fp_1082_010_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Great_Iron/nushi_jimen_fp_1082_010",
                "world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_010_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Great_Iron/nushi_jimen_fp_1082_020_0_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_fp_1082_020_0.json"},
        {"SV_TITANS/Great_Iron/nushi_jimen_fp_1082_020_1_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_fp_1082_020_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Great_Iron/nushi_jimen_fp_1082_020",
                "world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Great_Iron/sub_041_pre_start_0_clean.json", "SV_TITANS/Great_Iron/sub_041_pre_start_0.json"}
    };
    changeSceneOne(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                   "SV_TITANS/Great_Iron/sub_041_pre_start_0.json",
                   "world/scene/parts/event/event_scenario/sub_scenario/sub_041_/");

    // Iron Treads Only
    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();

    saveIndividualPokemon(47, devIdBoss, formIdBoss, genderBoss, rareBoss);
    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Great_Iron/nushi_jimen_fp_1090_010_0_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_fp_1090_010_0.json"},
        {"SV_TITANS/Great_Iron/nushi_jimen_fp_1090_010_1_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_fp_1090_010_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Great_Iron/nushi_jimen_fp_1090_010",
                "world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_010_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Great_Iron/nushi_jimen_fp_1090_020_0_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_fp_1090_020_0.json"},
        {"SV_TITANS/Great_Iron/nushi_jimen_fp_1090_020_1_clean.json", "SV_TITANS/Great_Iron/nushi_jimen_fp_1090_020_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Great_Iron/nushi_jimen_fp_1090_020",
                "world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_020_/");

    filePairs.clear();
    filePairs = {
        {"SV_TITANS/Great_Iron/sub_041_pre_start_1_clean.json", "SV_TITANS/Great_Iron/sub_041_pre_start_1.json"}
    };
    changeSceneOne(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_TITANS/Great_Iron/sub_041_pre_start_1.json",
                "world/scene/parts/event/event_scenario/sub_scenario/sub_041_/");

    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::patchTingLu(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Bombardier - 49
    saveIndividualPokemon(49, devIdBoss, formIdBoss, genderBoss, rareBoss);
    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_LEGENDS/Treasures/sub_014_main_0_clean.json", "SV_LEGENDS/Treasures/sub_014_main_0.json"},
        {"SV_LEGENDS/Treasures/sub_014_main_1_clean.json", "SV_LEGENDS/Treasures/sub_014_main_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_LEGENDS/Treasures/sub_014_main",
                "world/scene/parts/event/event_scenario/sub_scenario/sub_014_/");


    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::patchChienPao(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Bombardier - 49
    saveIndividualPokemon(50, devIdBoss, formIdBoss, genderBoss, rareBoss);
    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_LEGENDS/Treasures/sub_015_main_0_clean.json", "SV_LEGENDS/Treasures/sub_015_main_0.json"},
        {"SV_LEGENDS/Treasures/sub_015_main_1_clean.json", "SV_LEGENDS/Treasures/sub_015_main_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_LEGENDS/Treasures/sub_015_main",
                "world/scene/parts/event/event_scenario/sub_scenario/sub_015_/");


    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::patchWoChien(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Bombardier - 49
    saveIndividualPokemon(51, devIdBoss, formIdBoss, genderBoss, rareBoss);
    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_LEGENDS/Treasures/sub_016_main_0_clean.json", "SV_LEGENDS/Treasures/sub_016_main_0.json"},
        {"SV_LEGENDS/Treasures/sub_016_main_1_clean.json", "SV_LEGENDS/Treasures/sub_016_main_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_LEGENDS/Treasures/sub_016_main",
                "world/scene/parts/event/event_scenario/sub_scenario/sub_016_/");


    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::patchChiYu(){
    std::vector<int> devIdBoss;
    std::vector<int> formIdBoss;
    std::vector<int> genderBoss;
    std::vector<bool> rareBoss;

    // Bombardier - 49
    saveIndividualPokemon(52, devIdBoss, formIdBoss, genderBoss, rareBoss);
    std::vector<std::pair<std::string, std::string>> filePairs = {
        {"SV_LEGENDS/Treasures/sub_017_main_0_clean.json", "SV_LEGENDS/Treasures/sub_017_main_0.json"},
        {"SV_LEGENDS/Treasures/sub_017_main_1_clean.json", "SV_LEGENDS/Treasures/sub_017_main_1.json"}
    };
    changeScene(filePairs, devIdBoss, formIdBoss, genderBoss, rareBoss,
                "SV_LEGENDS/Treasures/sub_017_main",
                "world/scene/parts/event/event_scenario/sub_scenario/sub_017_/");


    devIdBoss.clear();
    formIdBoss.clear();
    genderBoss.clear();
    rareBoss.clear();
    filePairs.clear();
}

void SVBoss::copyFight(unsigned long long indexSet, unsigned long long indexCopy){
    cleanBossData["values"][indexSet]["pokeData"]["devId"] = cleanBossData["values"][indexCopy]["pokeData"]["devId"];
    cleanBossData["values"][indexSet]["pokeData"]["formId"] = cleanBossData["values"][indexCopy]["pokeData"]["formId"];
    cleanBossData["values"][indexSet]["pokeData"]["sex"] = cleanBossData["values"][indexCopy]["pokeData"]["sex"];
    cleanBossData["values"][indexSet]["pokeData"]["rareType"] = cleanBossData["values"][indexCopy]["pokeData"]["rareType"];
    cleanBossData["values"][indexSet]["pokeData"]["item"] = cleanBossData["values"][indexCopy]["pokeData"]["item"];
    cleanBossData["values"][indexSet]["pokeData"]["wazaType"] = "DEFAULT";
    for(int i = 1; i<=4; i++){
        std::string key = "waza"+std::to_string(i);
        cleanBossData["values"][indexSet]["pokeData"][key]["wazaId"] = "WAZA_NULL";
    }
    cleanBossData["values"][indexSet]["pokeData"]["gemType"] = cleanBossData["values"][indexCopy]["pokeData"]["gemType"];
}

void SVBoss::randomizeFight(unsigned long long index){
    int random = 1+std::rand()%1025;
    while(!allowedPokemonBoss.contains(bossMappingInfo["pokemons"][random]["natdex"]))
        random = 1+std::rand()%1025;

    int formRandom = std::rand()%static_cast<int>(bossMappingInfo["pokemons"][random]["forms"].size());
    while(bossMappingInfo["pokemons"][random]["forms"][formRandom]["is_present"] == false){
        formRandom = std::rand()%static_cast<int>(bossMappingInfo["pokemons"][random]["forms"].size());
    }

    cleanBossData["values"][index]["pokeData"]["devId"] = bossMappingInfo["pokemons"][random]["devName"];

    // Set Starter Form
    cleanBossData["values"][index]["pokeData"]["formId"] = formRandom;

    std::string genderStd = "";

    QString form_Check = QString::fromUtf8(bossMappingInfo["pokemons"][random]["name"].get<std::string>().c_str());
    if(maleOnlyPokemon.contains(form_Check) || genderlessPokemon.contains(form_Check)){

        if(genderlessPokemon.contains(form_Check)){
            genderStd = "DEFAULT";
        }else{
            genderStd = "MALE";
        }
    }else if(femaleOnlyPokemon.contains(form_Check)){
        genderStd = "FEMALE";
    } else if(genderForms.contains(form_Check)){
        if(formRandom == 0){

            genderStd = "MALE";
        }else{

            genderStd = "FEMALE";
        }
    }else{
        int rand_gender = std::rand()%50;
        if(rand_gender < 24){
            genderStd = "MALE";
        }else{

            genderStd = "FEMALE";
        }
    }

    cleanBossData["values"][index]["pokeData"]["sex"] = genderStd;
    int val = 1+std::rand()%100; // range is [1, shiny_starter_rate)
    if(val == 1){
        cleanBossData["values"][index]["pokeData"]["rareType"] = "RARE";
    }
    else{
        cleanBossData["values"][index]["pokeData"]["rareType"] = "NO_RARE";
    }
    std::string itemForPokemon = getPokemonItemId(bossMappingInfo["pokemons"][random]["natdex"], formRandom);
    cleanBossData["values"][index]["pokeData"]["item"] = itemForPokemon;

    cleanBossData["values"][index]["pokeData"]["wazaType"] = "DEFAULT";
    for(int i = 1; i<=4; i++){
        std::string key = "waza"+std::to_string(i);
        cleanBossData["values"][index]["pokeData"][key]["wazaId"] = "WAZA_NULL";
    }

    cleanBossData["values"][index]["pokeData"]["gemType"] = selectTeraTypes(random, formRandom);

}

void SVBoss::randomizeBosses(QDir baseDir){
    getUsablePokemon(boss_generation, boss_settings[0], boss_settings[1], boss_settings[2], allowedPokemonBoss, allowedLegendsBoss);
    getBannedPokemon(boss_settings[3],boss_settings[4],boss_settings[5],boss_settings[6], allowedPokemonBoss);

    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream fileInfo(qBaseDir.filePath("pokemon_mapping.json").toStdString());
    std::ifstream fileBoss(qBaseDir.filePath("SV_SCENES/eventBattlePokemon_array_clean.json").toStdString());
    std::ifstream fileTrainer(qBaseDir.filePath("SV_TRAINERS/trdata_array.json").toStdString());

    if(!fileTrainer.is_open()){
        fileTrainer.open(qBaseDir.filePath("SV_TRAINERS/trdata_array_clean.json").toStdString());
        if(!fileTrainer.is_open()){
            qFatal()<<"Couldn't open any of the trdata_array jsons";
        }
    }

    BaseDir = baseDir;

    if(!fileBoss.is_open()){
        qFatal()<<"eventBattlePokemon_array_clean.json did not open!";
    }

    if(!fileInfo.is_open()){
        qFatal()<<"pokemon_mapping.json did not open!";
    }

    fileInfo >> bossMappingInfo;
    fileInfo.close();
    fileBoss >>cleanBossData;
    fileBoss.close();
    fileTrainer >> trainerInfo;
    fileTrainer.close();

    for(unsigned long long i =0; i<cleanBossData["values"].size(); i++){
        if(i > 52){
            continue;
        }

        else if(i == 31 || i == 32){
            continue;
        }

        else if(i >= 58 && i<= 65){
            continue;
        }

        else if(i == 87 || i == 88){
            continue;
        }

        else if (i >= 12 && i <= 23) {
            copyFight(i, 11);
        }

        else if (i >= 27 && i <= 30) {
            copyFight(i, 26);
        }

        else if (i >= 33 && i <= 48) {
            switch(i){
            case 34:
                copyFight(i, 33);
                break;
            case 36:
                copyFight(i, 35);
                break;
            case 38:
            case 39:
            case 40:
                copyFight(i, 37);
                break;
            case 42:
                copyFight(i, 41);
                break;
            case 44:
                copyFight(i, 43);
                break;
            case 46:
                copyFight(i, 45);
                break;
            case 48:
                copyFight(i, 47);
                break;
            default:
                randomizeFight(i);
            }
        }
        else if(i == 67){
            copyFight(i, 66);
        }
        else{
            randomizeFight(i);
        }
    }
    patchMultiBattle();
    patchGimmighoul();
    patchLeChonk();
    patchHoundoom();
    patchSunflora();
    patchDonzoTitan();
    patchOrthwormTitan();
    patchBombariderTitan();
    patchKlawfTitan();
    patchGreatIronTitan();
    patchTingLu();
    patchChienPao();
    patchWoChien();
    patchChiYu();

    std::ofstream fileSave((qBaseDir.filePath("SV_SCENES/eventBattlePokemon_array.json").toStdString()));
    if(!fileSave.is_open()){
        qFatal()<<"eventBattlePokemon_array.json did not open!";
    }
    fileSave<<cleanBossData.dump(4);
    fileSave.close();
}
