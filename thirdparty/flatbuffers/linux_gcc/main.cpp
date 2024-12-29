#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/verifier.h>
#include "TrinitySceneObject_generated.h"

// Function to read the binary file into a vector of bytes
std::vector<char> ReadBinaryFile(const std::string &filePath) {
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
std::vector<int> FindMarkerOffsets(const std::vector<char> &data, const std::string &marker) {
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
bool IsValidOffset(int offset, int dataSize) {
    return offset >= 0 && offset < dataSize;
}

// Function to verify a FlatBuffer table
bool VerifyFlatBufferTable(const char *data, int size) {
    if (size < 4) {
        return false;
    }

    flatbuffers::Verifier verifier(reinterpret_cast<const uint8_t *>(data), size);
    return verifier.VerifyBuffer<pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>();
}

// Function to dynamically find the correct offset adjustment for a table
int FindCorrectOffsetAdjustment(
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
void PrintTableValues(const pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent *table) {
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

void PrintTableValues(const pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent *table) {
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

// Function to modify a FlatBuffer table in place
void ModifyTable_Model(char *rawData, int tableOffset, int markerOffset, int dataSize, const std::string &marker) {
    // Access the original table
    auto originalTable = flatbuffers::GetRoot<
        pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
        rawData + tableOffset);

    // Extract existing fields
    int dev_id = originalTable->DevId();
    int form_id = originalTable->FormId();
    int sex = originalTable->Sex();
    int rare = originalTable->Rare();
    int8_t field_04 = originalTable->Field_04();
    int8_t field_05 = originalTable->Field_05();
    int8_t field_06 = originalTable->Field_06();
    int8_t field_07 = originalTable->Field_07();
    bool field_08 = originalTable->Field_08();

    // Modify fields (example modifications)
    dev_id = 999;
    form_id = 2;
    sex = 0;   // Female
    rare = false;  // True

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
    // Restore the marker in the buffer
    std::memcpy(rawData + markerOffset, preservedMarker.data(), preservedMarker.size());
}

void ModifyTable_Field(char *rawData, int tableOffset, int markerOffset, int dataSize, const std::string &marker) {
    // Access the original table
    auto originalTable_f = flatbuffers::GetRoot<
        pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
        rawData + tableOffset);

    // Extract existing fields
    int dev_id = originalTable_f->DevId();
    int form_id = originalTable_f->FormId();
    int sex = originalTable_f->Sex();
    int rare = originalTable_f->Rare();
    int8_t field_04 = originalTable_f->Field_04();
    int8_t field_05 = originalTable_f->Field_05();
    int8_t field_06 = originalTable_f->Field_06();
    int8_t field_07 = originalTable_f->Field_07();
    bool field_08 = originalTable_f->Field_08();

    // Modify fields (example modifications)
    dev_id = 999;
    form_id = 2;
    sex = 0;   // Female
    rare = false;  // True

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
void SaveModifiedFile(const std::string &filePath, const std::vector<char> &data) {
    std::ofstream outFile(filePath, std::ios::binary);
    if (!outFile.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filePath);
    }

    outFile.write(data.data(), data.size());
    if (!outFile.good()) {
        throw std::runtime_error("Failed to write data to file: " + filePath);
    }

}

int main() {
    const std::string binaryFilePath = "common_0070_always_0_clean.trsog";
    const std::string outputFilePath = "modified_file.trsog";

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
                    std::cout << "Original table values (ti_PokemonModelComponent):\n";
                    PrintTableValues(table);

                    // Modify the table
                    ModifyTable_Model(data.data(), tableOffset, markerOffset, static_cast<int>(data.size()), marker[i]);

                    // Verify modifications
                    auto modifiedTable = flatbuffers::GetRoot<
                        pkNX::Structures::FlatBuffers::SV::Trinity::TIPokemonModelComponent>(
                        data.data() + tableOffset);
                    std::cout << "Modified table values:\n";
                    PrintTableValues(modifiedTable);
                }
                else{
                    auto table = flatbuffers::GetRoot<
                      pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                      data.data() + tableOffset);
                    std::cout << "Original table values (ti_FieldPokemonComponent):\n";
                    PrintTableValues(table);

                    // Modify the table
                    ModifyTable_Field(data.data(), tableOffset, markerOffset, static_cast<int>(data.size()), marker[i]);

                    // Verify modifications
                    auto modifiedTable = flatbuffers::GetRoot<
                        pkNX::Structures::FlatBuffers::SV::Trinity::TIFieldPokemonComponent>(
                        data.data() + tableOffset);
                    std::cout << "Modified table values:\n";
                    PrintTableValues(modifiedTable);
                }
            }
        }

        std::cout << "Saving Too values:" <<outputFilePath<<"\n";
        SaveModifiedFile(outputFilePath, data);

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}