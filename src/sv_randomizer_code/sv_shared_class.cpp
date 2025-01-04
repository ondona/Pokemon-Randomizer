#include <nlohmann/json.hpp>
#include "headers/sv_randomizer_headers/sv_shared_class.h"
#include <QMap>
#include <QVector>
#include <QString>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <string>
#include <stdexcept>
#include <QSet>
#include <QDebug>
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/verifier.h>
#include <TrinitySceneObject_generated.h>
#include <QProcess>
#include <QStringListModel>

namespace fs = std::filesystem;
using json = nlohmann::json;

json cleanSceneJSON;

std::string SVShared::getPokemonItemId(int index, int form){
    if (paradox.contains(index) && index != 1007 && index != 1008 && index != 1024) {
        return "ITEMID_BUUSUTOENAJII";
    }

    switch (index) {
        case 25: return "ITEMID_DENKIDAMA";
        case 113:
        case 242: return "ITEMID_MANMARUISI";
        case 283:
        case 415: return "ITEMID_AMAIMITU";
        case 285:
        case 590:
        case 948: return "ITEMID_TIISANAKINOKO";
        case 286:
        case 591:
        case 949: return "ITEMID_TIISANAKINOKO";
        case 316: return "ITEMID_ORENNOMI";
        case 317: return "ITEMID_OBONNOMI";
        case 440: return "ITEMID_MANMARUISI";
        case 625: return "ITEMID_KASIRANOAKASI";
        case 734: return "ITEMID_MOMONNOMI";
        case 739: return "ITEMID_NANASINOMI";
        case 740: return "ITEMID_KURABONOMI";
        case 741: return "ITEMID_YAMABUKINOMITU";
        case 778: return "ITEMID_KAGONOMI";
        case 819: return "ITEMID_ORENNOMI";
        case 483:
            if (form == 1) return "ITEMID_DAIKONGOUDAMA";
            break;
        case 484:
            if (form == 1) return "ITEMID_DAISIRATAMA";
            break;
        case 487:
            if (form == 1) return "ITEMID_DAIHAKKINDAMA";
            break;
        case 493:
            switch (form) {
            case 1: return "ITEMID_KOBUSINOPUREETO";
            case 2: return "ITEMID_AOZORAPUREETO";
            case 3: return "ITEMID_MOUDOKUPUREETO";
            case 4: return "ITEMID_DAITINOPUREETO";
            case 5: return "ITEMID_GANSEKIPUREETO";
            case 6: return "ITEMID_TAMAMUSIPUREETO";
            case 7: return "ITEMID_MONONOKEPUREETO";
            case 8: return "ITEMID_KOUTETUPUREETO";
            case 9: return "ITEMID_HINOTAMAPUREETO";
            case 10: return "ITEMID_SIZUKUPUREETO";
            case 11: return "ITEMID_MIDORINOPUREETO";
            case 12: return "ITEMID_IKAZUTIPUREETO";
            case 13: return "ITEMID_HUSIGINOPUREETO";
            case 14: return "ITEMID_TURARANOPUREETO";
            case 15: return "ITEMID_RYUUNOPUREETO";
            case 16: return "ITEMID_KOWAMOTEPUREETO";
            case 17: return "ITEMID_SEIREIPUREETO";
            }
            break;
        case 888:
            if (form == 1) return "ITEMID_KUTITATURUGI";
            break;
        case 889:
            if (form == 1) return "ITEMID_KUTITATATE";
            break;
        case 1017:
            switch (form) {
                case 1: return "ITEMID_IDONOMEN";
                case 2: return "ITEMID_KAMADONOMEN";
                case 3: return "ITEMID_ISHIDUENOMEN";
            }
        break;
    }
    return "ITEMID_NONE";
}

int SVShared::getPokemonItemValue(int index, int form){
    if (paradox.contains(index) && index != 1007 && index != 1008 && index != 1024) {
        return 25;
    }

    switch (index) {
    case 25: return 5;
    case 113:
    case 242: return 30;
    case 283:
    case 415: return 5;
    case 285:
    case 590:
    case 948: return 5;
    case 286:
    case 591:
    case 949: return 30;
    case 316: return 30;
    case 317: return 5;
    case 440: return 5;
    case 625: return 100;
    case 734: return 5;
    case 739: return 5;
    case 740: return 5;
    case 741: return 5;
    case 778: return 5;
    case 819: return 5;
    case 483:
        if (form == 1) return 100;
        break;
    case 484:
        if (form == 1) return 100;
        break;
    case 487:
        if (form == 1) return 100;
        break;
    case 493:
        switch (form) {
        case 1: case 2: case 3: case 4: case 5:
        case 6: case 7: case 8: case 9: case 10:
        case 11: case 12: case 13: case 14: case 15:
        case 16: case 17: return 100;
        }
        break;
    case 888:
        if (form == 1) return 100;
        break;
    case 889:
        if (form == 1) return 100;
        break;
    case 1017:
        switch (form) {
        case 1: case 2: case 3: return 100;
        }
        break;
    }
    return 0;
}

std::string SVShared::getItemForPokemon(int pokemon, int form){
    switch(pokemon){
        case 483:
            if(form == 1){
                return "ITEMID_DAIKONGOUDAMA";
            }
            break;
        case 484:
            if(form == 1){
                return "ITEMID_DAISIRATAMA";
            }
            break;
        case 487:
            if(form == 1){
                return "ITEMID_DAIHAKKINDAMA";
            }
            break;
        case 493:
            switch(form){
                case 1:  // Fightning
                    return "ITEMID_KOBUSINOPUREETO";
                    break;
                case 2:  // Flying
                    return "ITEMID_AOZORAPUREETO";
                    break;
                case 3:  // poison
                    return "ITEMID_MOUDOKUPUREETO";
                    break;
                case 4:  // ground
                    return "ITEMID_DAITINOPUREETO";
                    break;
                case 5:  // rock
                    return "ITEMID_GANSEKIPUREETO";
                    break;
                case 6:  // bug
                    return "ITEMID_TAMAMUSIPUREETO";
                    break;
                case 7:  // ghost
                    return "ITEMID_MONONOKEPUREETO";
                    break;
                case 8:  // steel
                    return "ITEMID_KOUTETUPUREETO";
                    break;
                case 9:  // fire
                    return "ITEMID_HINOTAMAPUREETO";
                    break;
                case 10:  // water
                    return "ITEMID_SIZUKUPUREETO";
                    break;
                case 11:  // grass
                    return "ITEMID_MIDORINOPUREETO";
                    break;
                case 12:  // electric
                    return "ITEMID_IKAZUTIPUREETO";
                    break;
                case 13:  // psychic
                    return "ITEMID_HUSIGINOPUREETO";
                    break;
                case 14:  // ice
                    return "ITEMID_TURARANOPUREETO";
                    break;
                case 15:  // dragon
                    return "ITEMID_RYUUNOPUREETO";
                    break;
                case 16:  // dark
                    return "ITEMID_KOWAMOTEPUREETO";
                    break;
                case 17:  // Fairy
                    return "ITEMID_SEIREIPUREETO";
                    break;
            };
            break;
        case 888:
            if(form == 1){
                return "ITEMID_KUTITATURUGI";
            }
            break;
        case 889:
            if(form == 1){
                return "ITEMID_KUTITATATE";
            }
            break;
        case 1017:
            switch(form){
                case 1:
                    return "ITEMID_IDONOMEN";
                    break;
                case 2:
                    return "ITEMID_KAMADONOMEN";
                    break;
                case 3:
                    return "ITEMID_ISHIDUENOMEN";
                    break;
            };
            break;
        default:
            return "ITEMID_NONE";
    };

    return "ITEMID_NONE";
}

std::string SVShared::selectTeraTypes(int pokemon, int formId){

    switch(pokemon){
        case 1017:
            switch(formId){
                case 0:
                    return "KUSA";
                case 1:
                    return "MIZU";
                case 2:
                    return "HONOO";
                case 3:
                    return "IWA";
            }
                break;
        case 1024:
            return "NIJI";
        default:
            break;
    }

    return teraTypes[std::rand()%teraTypes.length()].toUpper().toStdString();
}

void SVShared::getUsablePokemon(QVector<bool> generations, bool legend, bool paradoxs, bool legends_paradox, QList<int>& allowedPokemon, QList<int>& allowedLegends){
    // Logic for generation limiter
    int totalGens = 0;
    for(int i=0; i<9; i++){
        if(generations[i] == true){
            // Logic
            switch (i){
            case 0:
                if(legend == true){
                    allowedPokemon += gen1_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen1;
                    allowedLegends += gen1_legends;
                }
                break;
            case 1:
                if(legend == true){
                    allowedPokemon += gen2_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen2;
                    allowedLegends += gen2_legends;
                }
                break;
            case 2:
                if(legend == true){
                    allowedPokemon += gen3_legends;
                }
                else{
                    allowedPokemon += pokemonDevIds_Gen3;
                    allowedLegends += gen3_legends;
                }
                break;
            case 3:
                if(legend == true){
                    allowedPokemon += gen4_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen4;
                    allowedLegends += gen4_legends;
                }
                break;
            case 4:
                if(legend == true){
                    allowedPokemon += gen5_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen5;
                    allowedLegends += gen5_legends;
                }
                break;
            case 5:
                if(legend == true){
                    allowedPokemon += gen6_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen6;
                    allowedLegends += gen6_legends;
                }
                break;
            case 6:
                if(legend == true){
                    allowedPokemon += gen7_legends;
                    allowedPokemon += UB;
                }
                else{
                    allowedPokemon += pokemonDevIds_Gen7;
                    allowedLegends += gen7_legends;
                    allowedLegends += UB;
                }
                break;
            case 7:
                if(legend == true){
                    allowedPokemon += gen8_legends;
                }else{
                    allowedPokemon += pokemonDevIds_Gen8;
                    allowedLegends += gen8_legends;
                }
                break;
            case 8:
                if(legends_paradox == true){
                    allowedPokemon += gen9_legends;
                    allowedPokemon += paradox;
                }else if(legend == true){
                    allowedPokemon += gen9_legends;
                }else if(paradoxs == true){
                    allowedPokemon += paradox;
                }
                else{
                    allowedPokemon += pokemonDevIds_Gen9;
                    allowedLegends += gen9_legends;
                    allowedLegends += paradox;
                }
                break;
            };
            totalGens++;
        }
    }
    if(totalGens == 0){
        allowedPokemon = {};
        allowedPokemon += pokemonDevIds_Gen1;
        allowedLegends += gen1_legends;
        allowedPokemon += pokemonDevIds_Gen2;
        allowedLegends += gen2_legends;
        allowedPokemon += pokemonDevIds_Gen3;
        allowedLegends += gen3_legends;
        allowedPokemon += pokemonDevIds_Gen4;
        allowedLegends += gen4_legends;
        allowedPokemon += pokemonDevIds_Gen5;
        allowedLegends += gen5_legends;
        allowedPokemon += pokemonDevIds_Gen6;
        allowedLegends += gen6_legends;
        allowedPokemon += pokemonDevIds_Gen7;
        allowedLegends += gen7_legends;
        allowedLegends += UB;
        allowedPokemon += pokemonDevIds_Gen8;
        allowedLegends += gen8_legends;
        allowedPokemon += pokemonDevIds_Gen9;
        allowedLegends += gen9_legends;
        allowedLegends += paradox;
    }

    // Legends and Paradox > Legends > Paradox
    if(legends_paradox == true){
        allowedPokemon = {};
        allowedPokemon = legends_and_paradox;
    }else if(legend == true){
        allowedPokemon = {};
        allowedPokemon = legends;
    }else if(paradoxs == true){
        allowedPokemon = {};
        allowedPokemon = paradox;
    }
}

void SVShared::getBannedPokemon(bool stage1, bool stage2, bool stage3, bool singlestage, QList<int>& allowedPokemon){
    QSet<int> allowedMons(allowedPokemon.begin(), allowedPokemon.end());

    if(stage1 == true){
        QSet<int> stage1Set(gen9Stage1.begin(), gen9Stage1.end());
        allowedMons.subtract(stage1Set);
    }

    if(stage2 == true){
        QSet<int> stage2Set(gen9Stage2.begin(), gen9Stage2.end());
        allowedMons.subtract(stage2Set);
    }

    if(stage3 == true){
        QSet<int> stage3Set(gen9Stage3.begin(), gen9Stage3.end());
        allowedMons.subtract(stage3Set);
    }

    if(singlestage == true){
        QSet<int> singleStageSet(no_evolution.begin(), no_evolution.end());
        allowedMons.subtract(singleStageSet);
    }

    QSet<int> bannedSet(banned_pokemon.begin(), banned_pokemon.end());
    allowedMons.subtract(bannedSet);

    allowedPokemon = QList<int>(allowedMons.begin(), allowedMons.end());
}

void SVShared::createFolderHierarchy(const std::string& folder) {
    try {
        // Convert to absolute path
        fs::path absolutePath = fs::absolute(folder);
        qDebug() << "Absolute path: " << QString::fromUtf8(absolutePath.string());

        // Create directories
        fs::create_directories(absolutePath);
        qDebug() << "Created path: " << QString::fromUtf8(absolutePath.string());

        // Define where to start (example folder)
        std::string startFolder = "output";
        bool startSettingPermissions = false;
        fs::path currentPath;

        for (auto it = absolutePath.begin(); it != absolutePath.end(); ++it) {
            if (*it == startFolder) {
                startSettingPermissions = true;
            }

            if (startSettingPermissions) {
                currentPath /= *it; // Incrementally build the path
                if (!fs::exists(currentPath)) {
                    fs::create_directory(currentPath); // Create directory if it doesn't exist
                }
                fs::permissions(currentPath, fs::perms::all, fs::perm_options::add);
                qDebug() << "Set permissions for: " << QString::fromUtf8(currentPath.string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        qCritical() << "Error creating folder hierarchy: " << e.what();
    }
}

int SVShared::generateBinary(const std::string& schema, const std::string& jsonFile, const std::string& path, bool debug) {
    // Determine the platform and architecture
    std::string platform;
    std::string architecture;

    #ifdef _WIN32
        platform = "Windows";
    #ifdef _WIN64
        architecture = "Win64";
    #else
        architecture = "Win32";
    #endif
    #elif __APPLE__
        platform = "macOS";
    #ifdef __aarch64__
        architecture = "ARM";
    #else
        architecture = "Intel";
    #endif
    #elif __linux__
        platform = "Linux";
    #ifdef __clang__
        architecture = "Clang";
    #else
        architecture = "GCC";
    #endif
    #else
        platform = "Unknown";
        architecture = "Unknown";
    #endif

    // Print detected platform and architecture if debug is enabled
    qDebug()<< QString("Platform %1 with Architecture %2").arg(QString::fromStdString(platform), QString::fromStdString(architecture));

    // Determine the binary path based on platform and architecture
    std::string flatc;
    if (platform == "Windows") {
        flatc = fs::absolute("thirdparty/flatbuffers/windows/flatc.exe").string();
    } else if (platform == "macOS") {
        flatc = (architecture == "ARM")
        ? fs::absolute("thirdparty/flatbuffers/mac_ant/flatc").string()
        : fs::absolute("thirdparty/flatbuffers/mac_intel/flatc").string();
    } else if (platform == "Linux") {
        flatc = (architecture == "Clang")
        ? fs::absolute("thirdparty/flatbuffers/linux_clang/flatc").string()
        : fs::absolute("thirdparty/flatbuffers/linux_gcc/flatc").string();
    } else {
        throw std::runtime_error("Unsupported platform or architecture");
    }

    // Create the folder hierarchy
    createFolderHierarchy("output/romfs/" + path + "/");
    std::string outpath = fs::absolute("output/romfs/" + path + "/").string();

    // Use QProcess to execute the command
    QProcess process;
    QString program = QString::fromStdString(flatc);

    // Prepare the arguments
    QStringList arguments;
    arguments << "-b"
              << "-o" << QString::fromStdString(outpath)
              << QString::fromStdString(fs::absolute(schema).string())
              << QString::fromStdString(fs::absolute(jsonFile).string())
              << "--no-warnings";

    // Debug the command
    qDebug()<< QString("Executing: %1 with Arguments: %2").arg(program, arguments.join(' '));

    // Start the process
    process.start(program, arguments);
    process.waitForFinished();

    // Check the process result
    int returnCode = process.exitCode();
    if (debug) {
        if (returnCode != 0) {
            qCritical()<< "Error executing command. Return code: " << returnCode;
            qCritical() << "Error output: " << process.readAllStandardError().toStdString();
        } else {
            qDebug()<< "Program Ended Successfully";
        }
    }

    return returnCode;
}

uint64_t SVShared::fnv1a_64(const std::string& str, uint64_t basis) {
    uint64_t hash = basis;
    for (char c : str) {
        hash ^= static_cast<uint64_t>(c);
        hash *= 0x100000001B3;
    }
    return hash;
}

void SVShared::patchFileDescriptor() {
    using json = nlohmann::json;
    namespace fs = std::filesystem;

    // Load the JSON file
    std::ifstream file(fs::absolute("SV_FLATBUFFERS/SV_DATA_FLATBUFFERS/data_clean.json").string());
    if (!file.is_open()) {
        qFatal()<< "Error: Could not open data_clean.json!";
        return;
    }

    json data;
    file >> data;
    file.close();

    // Define the prefix and modPath
    std::string prefix = fs::current_path().string() + "/output/romfs/";
    fs::path modPath = fs::current_path() / "output/romfs";

    // Iterate through the files in modPath
    for (const auto& item : fs::recursive_directory_iterator(modPath)) {
        if (fs::is_regular_file(item)) {
            std::string name = item.path().string();
            std::replace(name.begin(), name.end(), '\\', '/'); // Replace backslashes with forward slashes
            name = name.substr(prefix.length()); // Remove the prefix

            // Compute the hash
            uint64_t hash = fnv1a_64(name);

            // Check if the hash exists in data["file_hashes"]
            auto& file_hashes = data["file_hashes"];
            auto it = std::find(file_hashes.begin(), file_hashes.end(), hash);
            if (it != file_hashes.end()) {
                size_t index = std::distance(file_hashes.begin(), it);
                file_hashes.erase(it); // Remove hash
                data["files"].erase(data["files"].begin() + index); // Remove corresponding file entry
            }
        }
    }

    // Write the modified data back to a JSON file
    std::ofstream outfile(fs::absolute("SV_FLATBUFFERS/SV_DATA_FLATBUFFERS/data.json").string());
    if (!outfile.is_open()) {
        qFatal()<< "Error: Could not open data.json for writing!";
        return;
    }

    outfile << data.dump(2);
    outfile.close();

    qDebug() << "Patched trpfd!";
}


int modelCount = 0;
int fieldCount = 0;
void SVShared::recursiveFindOfPokemonSceneTable(nlohmann::json& sceneObject, std::vector<int> devId, std::vector<int> formId,std::vector<int> gender, std::vector<bool> rare){
    if (!sceneObject.is_array()) {
        return;
    }
    for(unsigned long long i =0; i<sceneObject.size(); i++){
        if(sceneObject[i]["Type"] == "ti_PokemonModelComponent" || sceneObject[i]["Type"] == "ti_FieldPokemonComponent"){
            qDebug()<<"Type: "<< QString::fromUtf8(sceneObject[i]["Type"].get<std::string>().c_str());

            const auto& dataArray = sceneObject[i]["Data"];
            std::vector<uint8_t> data;
            for (const auto& byteValue : dataArray) {
                if (byteValue.is_number_unsigned()) {
                    data.push_back(static_cast<uint8_t>(byteValue.get<uint32_t>()));
                } else {
                    qFatal() << "Invalid byte value in Data array.\n";
                }
            }

            if(sceneObject[i]["Type"] == "ti_PokemonModelComponent"){
                auto originalTable = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
                    data.data());

                 qDebug() << "TIPokemonModelComponent:";
                 qDebug() << "  DevId: " << originalTable->DevId();
                 qDebug() << "  FormId: " << originalTable->FormId();
                 qDebug() << "  Sex: " << static_cast<int>(originalTable->Sex());
                 qDebug() << "  Rare: " << static_cast<int>(originalTable->Rare());

                flatbuffers::FlatBufferBuilder builder;
                uint16_t dev_id = devId[modelCount];
                uint16_t form_id = formId[modelCount];
                int8_t sex_num = gender[modelCount];
                int8_t rare_bool = rare[modelCount];
                modelCount++;

                // Serialize the new table with updated values
                auto updatedTable = pkNX::Structures::FlatBuffers::SV::Trinity::CreateTIPokemonModelComponent(
                    builder,
                    dev_id,
                    form_id,
                    sex_num,
                    rare_bool,
                    originalTable->Field_04(),
                    originalTable->Field_05(),
                    originalTable->Field_06(),
                    originalTable->Field_07(),
                    originalTable->Field_08()
                    );

                // Finish the FlatBuffer
                builder.Finish(updatedTable);

                // Replace the data vector with the new FlatBuffer content
                data.assign(builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize());
                json newTablePokemon = json::array();
                for(uint8_t byte: data){
                    newTablePokemon.push_back(byte);
                }

                sceneObject[i]["Data"] = newTablePokemon;


                auto modifiedTale = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
                    data.data());

                qDebug() << "TIPokemonModelComponent (Updated)";
                qDebug() << "  DevId: " << modifiedTale->DevId();
                qDebug() << "  FormId: " << modifiedTale->FormId();
                qDebug() << "  Sex: " << static_cast<int>(modifiedTale->Sex());
                qDebug() << "  Rare: " << static_cast<int>(modifiedTale->Rare());


            }else if(sceneObject[i]["Type"] == "ti_FieldPokemonComponent"){

                auto originalTable = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                    data.data());

                qDebug() << "TIFieldPokemonComponent:\n";
                qDebug() << "  DevId: " << originalTable->DevId();
                qDebug() << "  FormId: " << originalTable->FormId();
                qDebug() << "  Sex: " << static_cast<int>(originalTable->Sex());
                qDebug() << "  Rare: " << static_cast<int>(originalTable->Rare());

                flatbuffers::FlatBufferBuilder builder;
                uint16_t dev_id = devId[fieldCount];
                uint16_t form_id = formId[fieldCount];
                int8_t sex_num = gender[fieldCount];
                int8_t rare_bool = rare[fieldCount];
                fieldCount++;

                // Serialize the new table with updated values
                auto updatedTable = pkNX::Structures::FlatBuffers::SV::Trinity::CreateTIFieldPokemonComponent(
                    builder,
                    dev_id,
                    form_id,
                    sex_num,
                    rare_bool,
                    originalTable->Field_04(),
                    originalTable->Field_05(),
                    originalTable->Field_06(),
                    originalTable->Field_07(),
                    originalTable->Field_08()
                    );

                // Finish the FlatBuffer
                builder.Finish(updatedTable);

                // Replace the data vector with the new FlatBuffer content
                data.assign(builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize());
                json newTablePokemon = json::array();
                for(uint8_t byte: data){
                    newTablePokemon.push_back(byte);
                }

                sceneObject[i]["Data"] = newTablePokemon;


                auto modifiedTale = flatbuffers::GetRoot<
                    pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                    data.data());

                qDebug() << "TIFieldPokemonComponent (Updated):";
                qDebug() << "  DevId: " << modifiedTale->DevId();
                qDebug() << "  FormId: " << modifiedTale->FormId();
                qDebug() << "  Sex: " << static_cast<int>(modifiedTale->Sex());
                qDebug() << "  Rare: " << static_cast<int>(modifiedTale->Rare());

            }
        }
        recursiveFindOfPokemonSceneTable(sceneObject[i]["SubObjects"], devId, formId, gender, rare);
}
}
void SVShared::modifyPokemonScene(std::vector<int> devId, std::vector<int> formId,std::vector<int> gender, std::vector<bool> rare, std::string input, std::string output){

    std::string filePath = fs::absolute("SV_FLATBUFFERS/SV_SCENES").string();
    const std::string binaryFilePath = filePath+"/"+input;
    const std::string outputFilePath = filePath+"/"+output;

    modelCount = 0;
    fieldCount = 0;

    std::ifstream cleanSceneData(binaryFilePath);
    if(!cleanSceneData.is_open()){
        qFatal()<<"File: "<<binaryFilePath<<" didn't open";
    }

    cleanSceneData >>cleanSceneJSON;
    qDebug()<<QString::fromUtf8(cleanSceneJSON["ObjectTemplateName"].get<std::string>().c_str());

    recursiveFindOfPokemonSceneTable(cleanSceneJSON["Objects"], devId, formId, gender, rare);

    modelCount = 0;
    fieldCount = 0;

    std::ofstream outFile(outputFilePath);
    outFile<<cleanSceneJSON.dump(2);
    outFile.close();
}
