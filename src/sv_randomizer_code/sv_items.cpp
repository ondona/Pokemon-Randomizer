#include "thirdparty/nlohmann/json.hpp"
#include "headers/sv_randomizer_headers/sv_items.h"
#include <QDebug>
#include <ctime>
#include <fstream>
#include <QDir>

using json = nlohmann::ordered_json;
namespace fs = std::filesystem;

json itemsDev;

SVItems::SVItems(){

}

SVItems::~SVItems(){

}

void SVItems::randomizePokemonDrops(){
    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("SV_ITEMS/dropitemdata_array_clean.json").toStdString());
    // Load the JSON file

    if(!file.is_open()){
        qDebug()<<"Error opening dropitemdata_array_clean.json";
    }

    json syncrhoItems;
    file >> syncrhoItems;
    file.close();

    for(unsigned long long i =0; i<syncrhoItems["values"].size(); i++){
        int itemChoice = 1+std::rand()%1090;
        while(itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_MATERIAL" ||
               itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_EVENT" ||
               itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_BATTLE" ||
               itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_POCKET" ||
               bannedItems.contains(itemsDev["items"][itemChoice]["id"])){
            itemChoice = 1+std::rand()%1090;
        }

        std::string itemName = itemsDev["items"][itemChoice]["devName"];
        syncrhoItems["values"][i]["item1"]["itemid"] = itemName;
    }
    std::ofstream fileSave(filePath+"/SV_ITEMS/dropitemdata_array.json");
    fileSave<<syncrhoItems.dump(4);
    fileSave.close();
}

void SVItems::randomizeLetsGoItems(){
    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("SV_ITEMS/rummagingItemDataTable_array_clean.json").toStdString());
    // Load the JSON file

    if(!file.is_open()){
        qDebug()<<"Error opening rummagingItemDataTable_array_clean.json";
    }

    json pickUpItems;
    file >> pickUpItems;
    file.close();

    for(unsigned long long i =0; i<pickUpItems["values"].size(); i++){
        for(int j = 0; j<=4; j++){
            int itemChoice = 1+std::rand()%1090;
            while(itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_MATERIAL" ||
                   itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_EVENT" ||
                   itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_BATTLE" ||
                   itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_POCKET" ||
                   bannedItems.contains(itemsDev["items"][itemChoice]["id"])){
                itemChoice = 1+std::rand()%1090;
            }

            std::string itemName = itemsDev["items"][itemChoice]["devName"];
            std::string itemKey;
            if(j < 10){
                itemKey = "Item0" + std::to_string(j);
            }else{
                itemKey = "Item" + std::to_string(j);
            }
            pickUpItems["values"][i][itemKey] = itemName;
        }
    }

    std::ofstream fileSave(filePath+"/SV_ITEMS/rummagingItemDataTable_array.json");
    fileSave<<pickUpItems.dump(4);
    fileSave.close();
}

void SVItems::randomizePickUpItems(){
    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("SV_ITEMS/monohiroiItemData_array_clean.json").toStdString());
    // Load the JSON file

    if(!file.is_open()){
        qDebug()<<"Error opening monohiroiItemData_array_clean.json";
    }

    json pickUpItems;
    file >> pickUpItems;
    file.close();

    for(unsigned long long i =0; i<pickUpItems["values"].size(); i++){
        for(int j = 1; j<=30; j++){
            int itemChoice = 1+std::rand()%1090;
            while(itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_MATERIAL" ||
                   itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_EVENT" ||
                   itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_BATTLE" ||
                   itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_POCKET" ||
                   bannedItems.contains(itemsDev["items"][itemChoice]["id"])){
                itemChoice = 1+std::rand()%1090;
            }

            std::string itemName = itemsDev["items"][itemChoice]["devName"];
            std::string itemKey;
            if(j < 10){
                itemKey = "item_0" + std::to_string(j);
            }else{
                itemKey = "item_" + std::to_string(j);
            }
            pickUpItems["values"][i][itemKey]["itemId"] = itemName;
        }
    }

    std::ofstream fileSave(filePath+"/SV_ITEMS/monohiroiItemData_array.json");
    fileSave<<pickUpItems.dump(4);
    fileSave.close();
}

void SVItems::randomizeHiddenItems(std::string fileName, std::string saveFile){
    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("SV_ITEMS/" + QString::fromStdString(fileName)).toStdString());
    // Load the JSON file

    if(!file.is_open()){
        qDebug()<<"Error opening " + QString::fromStdString(fileName);
    }

    json paldeaItems;
    file >> paldeaItems;
    file.close();

    for(unsigned long long i =0; i<paldeaItems["values"].size(); i++){
        for(int j = 1; j<=10; j++){
            int itemChoice = 1+std::rand()%1090;
            while(itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_MATERIAL" ||
                   itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_EVENT" ||
                   itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_BATTLE" ||
                   itemsDev["items"][itemChoice]["ItemType"] == "ITEMTYPE_POCKET" ||
                   bannedItems.contains(itemsDev["items"][itemChoice]["id"])){
                itemChoice = 1+std::rand()%1090;
            }

            std::string itemName = itemsDev["items"][itemChoice]["devName"];
            std::string itemKey = "item_" + std::to_string(j);
            paldeaItems["values"][i][itemKey]["itemId"] = itemName;
            paldeaItems["values"][i][itemKey]["emergePercent"] = 100 + std::rand()%901;
            paldeaItems["values"][i][itemKey]["dropCount"] = 1 + std::rand()%20;
        }
    }

    std::ofstream fileSave(filePath+"/SV_ITEMS/"+saveFile);
    fileSave<<paldeaItems.dump(4);
    fileSave.close();
}

bool SVItems::randomize_items(){
    std::string filePath = fs::absolute("SV_FLATBUFFERS").string();
    QString QBaseAddress = QString::fromStdString(filePath);
    QDir qBaseDir(QBaseAddress);
    std::ifstream file(qBaseDir.filePath("SV_ITEMS/pokemon_items_dev.json").toStdString());
    // Load the JSON file

    if(!file.is_open()){
        qDebug()<<"Error opening pokemon_items_dev.json";
    }

    file >> itemsDev;
    file.close();

    if(randomize_hidden_items == true){
        randomizeHiddenItems("hiddenItemDataTable_lc_array_clean.json", "hiddenItemDataTable_lc_array.json");
        randomizeHiddenItems("hiddenItemDataTable_su2_array_clean.json", "hiddenItemDataTable_su2_array.json");
        randomizeHiddenItems("hiddenItemDataTable_su1_array_clean.json", "hiddenItemDataTable_su1_array.json");
        randomizeHiddenItems("hiddenItemDataTable_array_clean.json", "hiddenItemDataTable_array.json");
    }

    if(randomize_pickup_items == true){
        randomizePickUpItems();
    }

    if(randomize_synchro_items == true){
        randomizeLetsGoItems();
    }

    if(randomize_pokemon_drops == true){
        randomizePokemonDrops();
    }

    return true;
}
