#ifndef SV_RAIDS_H
#define SV_RAIDS_H

#include <QVector>
#include <QMap>
#include "headers/sv_randomizer_headers/sv_shared_class.h"

class SVRaids: public SVShared{
    public:    
        bool praids_randomize = false;
        bool praids_randomize_per_star = false;
        bool praids_force_shiny = false;
        int praids_shiny_chance = 10;
        bool praids_onlyLegends = false;
        bool praids_onlyParadox = false;
        bool praids_onlyLegendsandParadox = false;
        bool praids_BanStage1 = false;
        bool praids_BanStage2 = false;
        bool praids_BanStage3 = false;
        bool praids_Ban1Stage = false;
        bool praidsBalanceAreaPerBST = false; //TBD
        bool paldea_Settings_for_all_raids = false;
        QVector<bool> praidsgeneratio = {false, false, false,
                                         false, false, false,
                                         false, false, false};

        bool kraids_randomize = false;
        bool kraids_randomize_per_star = false;
        bool kraids_force_shiny = false;
        int kraids_shiny_chance = 10;
        bool kraids_onlyLegends = false;
        bool kraids_onlyParadox = false;
        bool kraids_onlyLegendsandParadox = false;
        bool kraids_BanStage1 = false;
        bool kraids_BanStage2 = false;
        bool kraids_BanStage3 = false;
        bool kraids_Ban1Stage = false;
        bool kraidsBalanceAreaPerBST = false; //TBD
        QVector<bool> kraidsgeneration = {false, false, false,
                                          false, false, false,
                                          false, false, false};

        bool braids_randomize = false;
        bool braids_randomize_per_star = false;
        bool braids_force_shiny = false;
        int braids_shiny_chance = 10;
        bool braids_onlyLegends = false;
        bool braids_onlyParadox = false;
        bool braids_onlyLegendsandParadox = false;
        bool braids_BanStage1 = false;
        bool braids_BanStage2 = false;
        bool braids_BanStage3 = false;
        bool braids_Ban1Stage = false;
        bool braidsBalanceAreaPerBST = false; //TBD
        QVector<bool> braidsgeneration = {false, false, false,
                                          false, false, false,
                                          false, false, false};

        bool randomize_paldean_raids(std::string fileName);
        bool randomize_kitakami_raids(std::string fileName);
        bool randomize_blueberry_raids(std::string fileName);
};

#endif // SV_RAIDS_H
