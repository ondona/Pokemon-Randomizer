#include <QDebug>
#include <QVector>
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
QDir BaseDir;

void checkAndDeleteFile(std::string filePath) {
    try {
        // Check if the file exists
        if (fs::exists(filePath)) {
            // Delete the file
            if (fs::remove(filePath)) {
                qDebug() << "File deleted successfully: " << filePath;
            } else {
                qDebug() << "Failed to delete the file: " << filePath;
            }
        } else {
            qDebug()  << "File does not exist: " << filePath;
        }
    } catch (const fs::filesystem_error& e) {
        // Handle filesystem-related exceptions
        qDebug() << "Filesystem error: " << e.what();
    } catch (const std::exception& e) {
        // Handle other exceptions
        qDebug() << "Error: " << e.what();
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

void SVBoss::patchMultiBattle(){

}

void SVBoss::patchGimmighoul(){

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
            genderBoss.push_back(0);
        }
        else{
            genderBoss.push_back(1);
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

}

void SVBoss::patchSunflora(){

}

void SVBoss::patchDonzoTitan(){

}

void SVBoss::patchOrthwormTitan(){

}

void SVBoss::patchBombariderTitan(){

}

void SVBoss::patchKlawfTitan(){

}

void SVBoss::patchGreatIronTitan(){

}

void SVBoss::patchTingLu(){

}

void SVBoss::patchChienPao(){

}

void SVBoss::patchWoChien(){

}

void SVBoss::patchChiYu(){

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

    patchLeChonk();

    std::ofstream fileSave(filePath+"SV_SCENES/eventBattlePokemon_array.json");
    fileSave<<cleanBossData.dump(4);
    fileSave.close();
}
