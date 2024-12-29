#ifndef SVRANDOMIZERCODE_H
#define SVRANDOMIZERCODE_H

#include <QString>
#include <QMap>
#include "sv_trainers.h"
#include "sv_raids.h"
#include "sv_wild_static.h"
#include "sv_items.h"
#include "sv_stats.h"
#include "sv_starters_gifts.h"


class SVRandomizerCode : public SVShared
{
public:
    SVRandomizerCode();
    ~SVRandomizerCode();
    //General
    bool auto_patch = true; // Added
    bool kaizo_mode;       // Added
    int bulk_amount = 1;
    QString seed = "";

    //Starters and Gifts
    SVStarters svRandomizerStarters;

    //Stats
    SVStats svRandomizerStats;

    //TMs and Items
    SVItems svRandomizerItems;

    // Wild and Static Encounters
    SVWilds svRandomizerWilds;

    // Raids
    SVRaids svRandomizerRaids;

    //Trainers
    SVTrainers svRandomizerTrainers;

    //Bosses
    bool randomize_bosses;
    QVector<bool> boss_settings;
    QVector<bool> boss_generation;

    bool svRandomizeStarters(SVStarters svs, int seed, int run);
    bool svRandomizeGifts(SVStarters svs, int seed, int run);
    bool svRandomizeStats(SVStats stats, int seed, int run);
    bool svRandomizeItem(SVItems items, int seed, int run);
    bool svRandomizeWilds(SVWilds wild, int seed, int run);
    bool svRandomizeRaids(SVRaids raids, int seed, int run);
    bool svRandomizeTrainers(SVTrainers trainers, int seed, int run);

};

#endif // SVRANDOMIZERCODE_H
