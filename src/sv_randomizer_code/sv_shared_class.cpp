#include "thirdparty/nlohmann/json.hpp"
#include "headers/sv_randomizer_headers/sv_shared_class.h"
#include <QMap>
#include <QVector>
#include <QString>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <QSet>
#include <chrono>
#include <random>
#include <QDebug>
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/verifier.h>
#include <TrinitySceneObject_generated.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;

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
        std::random_device rd;
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = now.time_since_epoch();
        int seed = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() ^ rd());
        std::srand(seed);
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
        qDebug()<<"Here";
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
    /**
     * Creates a folder hierarchy from a given path.
     * Parameter should be a folder inside the main output folder.
     * Ex: output/romfs/world/data/pokemon
     * @param folder: Output folder to create
     */

    std::string absolutePath = fs::absolute(folder).string();
    std::replace(absolutePath.begin(), absolutePath.end(), '\\', '/');

    std::vector<std::string> folders;
    std::istringstream iss(absolutePath);
    std::string part;

    while (std::getline(iss, part, '/')) {
        folders.push_back(part);
    }

    size_t indexValue = 0;
    for (size_t i = 0; i < folders.size(); ++i) {
        indexValue++;
        if (folders[i] == "output") {
            break;
        }
    }

    std::vector<std::string> subFolders(folders.begin() + indexValue, folders.end());

    std::string folderToGetPerms = "/";
    for (size_t i = 1; i < indexValue; ++i) {
        folderToGetPerms += folders[i] + "/";
    }

    for (const auto& subFolder : subFolders) {
        folderToGetPerms += subFolder + "/";
        fs::create_directories(folderToGetPerms);
        fs::permissions(folderToGetPerms, fs::perms::all);
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
    if (debug) {
        std::cout << "Platform: " << platform << ", Architecture: " << architecture << std::endl;
    }

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

    createFolderHierarchy("output/romfs/" + path + "/");
    std::string outpath = fs::absolute("output/romfs/" + path + "/").string();

    std::ostringstream command;
    command << flatc
            << " -b"
            << " -o " << outpath
            << " " << fs::absolute(schema).string()
            << " " << fs::absolute(jsonFile).string()
            << " " << "--no-warnings";

    if (debug) {
        std::cout << "Executing command: " << command.str() << std::endl;
    }

    int returnCode = std::system(command.str().c_str());
    if (debug) {
        if (returnCode != 0) {
            std::cerr << "Error executing command. Return code: " << returnCode << std::endl;
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
        std::cerr << "Error: Could not open data_clean.json!" << std::endl;
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
        std::cerr << "Error: Could not open data.json for writing!" << std::endl;
        return;
    }

    outfile << data.dump(2);
    outfile.close();

    std::cout << "Patched trpfd!" << std::endl;
}

// Function to read the binary file into a vector of bytes
std::vector<char> SVShared::ReadBinaryFile(const std::string &filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    int size = static_cast<int>(file.tellg());
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Failed to read file: " + filePath);
    }
    return buffer;
}

// Function to find marker offsets in the binary data
std::vector<int> SVShared::FindMarkerOffsets(const std::vector<char> &data, const std::string &marker) {
    std::vector<int> offsets;
    int index = 0;

    std::string dataAsString(data.begin(), data.end());
    while ((index = static_cast<int>(dataAsString.find(marker, index))) != std::string::npos) {
        offsets.push_back(index);
        ++index; // Move to the next character
    }
    return offsets;
}

// Function to validate offsets
bool SVShared::IsValidOffset(int offset, int dataSize) {
    return offset >= 0 && offset < dataSize;
}

// Function to verify a FlatBuffer table
bool SVShared::VerifyFlatBufferTable(const char *data, int size) {
    if (size < 4) {
        return false;
    }

    flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t *>(data), size);
    return verifier.VerifyBuffer<pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>();
}

// Function to dynamically find the correct offset adjustment for a table
int SVShared::FindCorrectOffsetAdjustment(
    const std::vector<char> &data,
    int markerOffset,
    int maxAdjustmentRange,
    int step,
    int dataSize,
    bool (*verifier)(const char *, int)) {

    for (int adjustment = 0; adjustment <= maxAdjustmentRange; adjustment += step) {
        int adjustedOffset = markerOffset - adjustment;

        if (!IsValidOffset(adjustedOffset, dataSize)) {
            continue; // Skip invalid offsets
        }

        const int remainingBytes = dataSize - adjustedOffset;
        if (remainingBytes < 4) { // Minimum size for FlatBuffer table
            continue;
        }

        if (verifier(data.data() + adjustedOffset, remainingBytes)) {
            return adjustedOffset; // Found valid adjustment
        }
    }

    return -1; // No valid adjustment found
}

// Function to print table values
void SVShared::PrintTableValues(const pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent *table) {
    std::cout << "DevId: " << table->DevId() << "\n";
    std::cout << "FormId: " << table->FormId() << "\n";

    std::string sexStr;
    switch (table->Sex()) {
    case -1: sexStr = "Unknown"; break;
    case 0: sexStr = "Male"; break;
    case 1: sexStr = "Female"; break;
    default: sexStr = "Invalid"; break;
    }
    std::cout << "Sex: " << sexStr << "\n";

    std::string rareStr = (table->Rare() == 1) ? "True" : "False";
    std::cout << "Rare: " << rareStr << "\n";
    std::cout << "Field_04: " << (int)table->Field_04() << "\n";
    std::cout << "Field_05: " << (int)table->Field_05() << "\n";
    std::cout << "Field_06: " << (int)table->Field_06() << "\n";
    std::cout << "Field_07: " << (int)table->Field_07() << "\n";
    std::cout << "Field_08: " << table->Field_08() << "\n";
}

void SVShared::PrintTableValues(const pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent *table) {
    std::cout << "DevId: " << table->DevId() << "\n";
    std::cout << "FormId: " << table->FormId() << "\n";

    std::string sexStr;
    switch (table->Sex()) {
    case -1: sexStr = "Unknown"; break;
    case 0: sexStr = "Male"; break;
    case 1: sexStr = "Female"; break;
    default: sexStr = "Invalid"; break;
    }
    std::cout << "Sex: " << sexStr << "\n";

    std::string rareStr = (table->Rare() == 1) ? "True" : "False";
    std::cout << "Rare: " << rareStr << "\n";
    std::cout << "Field_04: " << (int)table->Field_04() << "\n";
    std::cout << "Field_05: " << (int)table->Field_05() << "\n";
    std::cout << "Field_06: " << (int)table->Field_06() << "\n";
    std::cout << "Field_07: " << (int)table->Field_07() << "\n";
    std::cout << "Field_08: " << table->Field_08() << "\n";
}
std::string fileCheck;
// Function to modify a FlatBuffer table in place
void SVShared::ModifyTable_Model(char *rawData, int tableOffset, int markerOffset, int dataSize, const std::string &marker,
                                 int devid, int form, int gender, bool shiny) {
    // Access the original table
    auto originalTable = flatbuffers::GetRoot<
        pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
        rawData + tableOffset);

    // Extract existing fields
    uint16_t dev_id = 0;
    uint16_t form_id = 0;
    int8_t sex = originalTable->Sex();
    int8_t rare = 0;
    int8_t field_04 = originalTable->Field_04();
    int8_t field_05 = originalTable->Field_05();
    int8_t field_06 = originalTable->Field_06();
    int8_t field_07 = originalTable->Field_07();
    bool field_08 = originalTable->Field_08();

    // Modify fields
    dev_id = devid;
    form_id = form;
    if(fileCheck.find("_0060_always") != std::string::npos){
        form_id = 0;
    }
    sex = gender;
    rare = (int8_t)shiny;  // True

    // Serialize the new table
    flatbuffers::FlatBufferBuilder builder(1024);

    // Create the modified table
    auto modifiedTable = pkNX::Structures::FlatBuffers::SV::Trinity::CreateTIPokemonModelComponent(
        builder, dev_id, form_id, sex, rare, field_04, field_05, field_06, field_07, field_08);

    builder.Finish(modifiedTable);

    // Extract only the serialized table bytes
    const uint8_t *bufferPtr = builder.GetBufferPointer();
    int tableSize = builder.GetSize();

    // Preserve the marker
    std::string preservedMarker(rawData + markerOffset, marker.size());

    // Replace the table's data in the buffer
    std::memcpy(rawData + tableOffset, bufferPtr, tableSize);

    // // Restore the marker in the buffer
    std::memcpy(rawData + markerOffset, preservedMarker.data(), preservedMarker.size());

}

void SVShared::ModifyTable_Field(char *rawData, int tableOffset, int markerOffset, int dataSize, const std::string &marker,
    int devid, int form, int gender, bool shiny) {
    // Access the original table
    auto originalTable_f = flatbuffers::GetRoot<
        pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
        rawData + tableOffset);

    // Extract existing fields
    uint16_t dev_id = 0;
    uint16_t form_id = 0;
    int8_t sex = originalTable_f->Sex();
    int8_t rare = 0;
    int8_t field_04 = originalTable_f->Field_04();
    int8_t field_05 = originalTable_f->Field_05();
    int8_t field_06 = originalTable_f->Field_06();
    int8_t field_07 = originalTable_f->Field_07();
    bool field_08 = originalTable_f->Field_08();

    // Modify fields (example modifications)
    dev_id = devid;
    form_id = form;
    sex = gender;   // Female
    rare = (int8_t)shiny;  // True

    // Serialize the new table
    flatbuffers::FlatBufferBuilder builder2(1024);

    // Create the modified table
    auto modifiedTable_f = pkNX::Structures::FlatBuffers::SV::Trinity::CreateTIFieldPokemonComponent(
        builder2, dev_id, form_id, sex, rare, field_04, field_05, field_06, field_07, field_08);

    builder2.Finish(modifiedTable_f);

    // Extract only the serialized table bytes
    const uint8_t *bufferPtr = builder2.GetBufferPointer();
    int tableSize = builder2.GetSize();

    // Preserve the marker
    std::string preservedMarker(rawData + markerOffset, marker.size());

    // Replace the table's data in the buffer
    std::memcpy(rawData + tableOffset, bufferPtr, tableSize);
    // Restore the marker in the buffer
    std::memcpy(rawData + markerOffset, preservedMarker.data(), preservedMarker.size());
}

// Function to save the modified data into a new file
void SVShared::SaveModifiedFile(const std::string &filePath, const std::vector<char> &data) {
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filePath);
    }

    outFile.write(data.data(), data.size());
    if (!outFile.good()) {
        throw std::runtime_error("Failed to write data to file: " + filePath);
    }

    std::cout<<"Saved data on: "<<filePath<<"\n";

}

bool SVShared::modifyPokemonScene(std::vector<int> devId, std::vector<int> formId, std::vector<int> gender, std::vector<bool> rare, std::string input, std::string output) {
    std::string filePath = fs::absolute("SV_FLATBUFFERS/SV_SCENES").string();

    const std::string binaryFilePath = filePath+"/"+input;
    const std::string outputFilePath = filePath+"/"+output;
    fileCheck = input;

    try {
        auto data = ReadBinaryFile(binaryFilePath);
        if (data.empty()) {
            throw std::runtime_error("Binary file is empty.");
        }

        const std::string marker[2] = {"ti_PokemonModelComponent", "ti_FieldPokemonComponent"};
        int maxAdjustmentRange = 64;
        int step = 4;
        for(int i=0; i<2; i++) {
            auto markerOffsets = FindMarkerOffsets(data, marker[i]);
            if (markerOffsets.empty()) {
                continue;
            }
            int total = 0;
            for (int markerOffset : markerOffsets) {
                int tableOffset = FindCorrectOffsetAdjustment(
                    data,
                    markerOffset,
                    maxAdjustmentRange,
                    step,
                    static_cast<int>(data.size()),
                    VerifyFlatBufferTable
                    );

                if (tableOffset == -1) {
                    continue;
                }

                if(marker[i] == "ti_PokemonModelComponent"){
                    auto table = flatbuffers::GetRoot<
                        pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
                        data.data() + tableOffset);
                    //std::cout << "Original table values (ti_PokemonModelComponent):\n";
                    //PrintTableValues(table);

                    // Modify the table
                    ModifyTable_Model(data.data(), tableOffset, markerOffset, static_cast<int>(data.size()), marker[i],
                    devId[total], formId[total], gender[total], rare[total]);

                    // Verify modifications
                    auto modifiedTable = flatbuffers::GetRoot<
                        pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
                        data.data() + tableOffset);
                    //std::cout << "Modified table values:\n";
                    //PrintTableValues(modifiedTable);
                }
                else{
                    auto table = flatbuffers::GetRoot<
                        pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                        data.data() + tableOffset);
                    //std::cout << "Original table values (ti_FieldPokemonComponent):\n";
                    //PrintTableValues(table);

                    // Modify the table
                    ModifyTable_Field(data.data(), tableOffset, markerOffset, static_cast<int>(data.size()), marker[i],
                    devId[total], formId[total], gender[total], rare[total]);

                    // Verify modifications
                    auto modifiedTable = flatbuffers::GetRoot<
                        pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                        data.data() + tableOffset);
                    //std::cout << "Modified table values:\n";
                    //PrintTableValues(modifiedTable);
                }
                total++;
            }
        }

        SaveModifiedFile(outputFilePath, data);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return true;
}
