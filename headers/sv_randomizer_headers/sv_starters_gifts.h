#ifndef SV_STARTERS_GIFTS_H
#define SV_STARTERS_GIFTS_H

#include <QString>
#include <QMap>
#include <QVector>
#include "thirdparty/nlohmann/json.hpp"
#include "headers/sv_randomizer_headers/sv_shared_class.h"

using json = nlohmann::json;

class SVStarters: public SVShared{
    public:
        SVStarters();
        ~SVStarters();
        int totalPokemon = 1025; // needs to be 1025 so we can use all pokemon ids
        QList<int> bannedPokemon;
        QList<int> allowedPokemon;
        QList<int> allowedLegends;

        //Starters
        QVector<bool> generations_starters = {false, false, false, false, false, false, false, false, false};
        bool enable_starters = false; // TB Deleted
        bool force_shiny_starter = false; // Added
        bool all_starters_shiny = false;  // Added
        int shiny_starter_rate = 10;
        bool show_starters_in_overworld = true; // Added
        bool randomize_starters_tera_types = false; //Added
        bool ban_stage_1_pokemon_starters = false; //Added
        bool ban_stage_2_pokemon_starters = false; //Added
        bool ban_stage_3_pokemon_starters = false; //Added
        bool ban_single_stage_pokemon_starters = false; //Added
        bool only_legendary_pokemon_starters = false; //Added
        bool only_paradox_pokemon_starters = false; //Added
        bool only_legends_and_paradox_starters = false; //Added
        QVector<QString> starters = {"", "", ""};
        QVector<int> starters_forms = {0,0,0};
        QVector<QString> starters_gender = {"MALE", "MALE", "MALE"};
        QVector<QString> starters_pokeball = {"Poke Ball", "Poke Ball", "Poke Ball"};
        QVector<bool> starters_shiny = {false, false, false};

        // Gifts
        QList<int> bannedPokemon_gifts;
        QList<int> allowedPokemon_gifts;
        QList<int> allowedLegends_gifts;
        bool enable_gifts = false;
        int shiny_static_rate = 10;
        bool randomize_tera_types = false;
        QVector<bool> generation_gifts = {false, false, false, false, false, false, false, false, false};

        // functions
        bool randomize_starters();
        void get_selected_starter(int index, QString name, int form=0, QString gender="DEFAULT", bool shiny=false, QString ball ="Poke Ball");
        int getTeraTypeInt(std::string teraType);

        // functions gifts
        bool randomize_gifts();
    };

#endif // SV_STARTERS_GIFTS_H
