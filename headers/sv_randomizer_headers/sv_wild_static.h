#ifndef SV_WILD_STATIC_H
#define SV_WILD_STATIC_H

#include <QVector>
#include "headers/sv_randomizer_headers/sv_shared_class.h"
#include <QMap>

class SVWilds: public SVShared{
    public:
        bool randomize_paldea_wild = false;
        bool paldea_ExcludeLegends = false;
        bool paldea_OnlyLegends = false;
        bool paldea_OnlyParadox = false;
        bool paldea_OnlyLegendsAndParadox = false;
        bool paldea_BanStage1 = false;
        bool paldea_BanStage2 = false;
        bool paldea_BanStage3 = false;
        bool paldea_Ban1Stage = false;
        bool paldea_Settings_for_all_wild = false;
        bool paldea_BalanceAreaPerBST = false; //TBD
        bool let_oger_pagos_spawn = false;
        bool types_changed = false;
        QVector<bool> generation_paldea_wild = {false, false, false,
                                                false, false, false,
                                                false, false, false};

        bool randomize_kitakami_wild = false;
        bool kitakami_ExcludeLegends = false;
        bool kitakami_OnlyLegends = false;
        bool kitakami_OnlyParadox = false;
        bool kitakami_OnlyLegendsAndParadox = false;
        bool kitakami_BanStage1 = false;
        bool kitakami_BanStage2 = false;
        bool kitakami_BanStage3 = false;
        bool kitakami_Ban1Stage = false;
        bool kitakami_BalanceAreaPerBST = false; //TBD
        QVector<bool> generation_kitakami_wild = {false, false, false,
                                                  false, false, false,
                                                  false, false, false};

        bool randomize_blueberry_wild = false;
        bool blueberry_ExcludeLegends = false;
        bool blueberry_OnlyLegends = false;
        bool blueberry_OnlyParadox = false;
        bool blueberry_OnlyLegendsAndParadox = false;
        bool blueberry_BanStage1 = false;
        bool blueberry_BanStage2 = false;
        bool blueberry_BanStage3 = false;
        bool blueberry_Ban1Stage = false;
        bool blueberry_BalanceAreaPerBST = false; // TBD
        QVector<bool> generation_blueberry_wild = {false, false, false,
                                                   false, false, false,
                                                   false, false, false};

        bool statics_ExcludeLegends = false;
        bool statics_OnlyLegends = false;
        bool statics_OnlyParadox = false;
        bool statics_OnlyLegendsAndParadox = false;
        bool statics_BanStage1 = false;
        bool statics_BanStage2 = false;
        bool statics_BanStage3 = false;
        bool statics_Ban1Stage = false;
        bool statics_BalanceAreaPerBST = false; //TBD
        QVector<bool> generation_statics_wild = {false, false, false,
                                                 false, false, false,
                                                 false, false, false};

        bool randomize_paldea_wilds(std::string fileName);
        bool randomize_kitakami_wilds();
        bool randomize_blueberry_wilds();
        bool randomize_statics();
};

#endif // SV_WILD_STATIC_H
