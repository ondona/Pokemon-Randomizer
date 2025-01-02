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
#include "sv_boss.h"


class SVRandomizerCode : public SVShared
{
public:
    SVRandomizerCode();
    ~SVRandomizerCode();
    //General
    bool auto_patch = true; // Added
    bool kaizo_mode;       // Added
    unsigned int bulk_amount = 1;
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
    SVBoss svRandomizerBoss;

    bool svRandomizeStarters(SVStarters svs);
    bool svRandomizeGifts(SVStarters svs);
    bool svRandomizeStats(SVStats stats);
    bool svRandomizeItem(SVItems items);
    bool svRandomizeWilds(SVWilds wild);
    bool svRandomizeRaids(SVRaids raids);
    bool svRandomizeTrainers(SVTrainers trainers);
    bool svRandomizeBoss(SVBoss boss, QDir dir);
};

#endif // SVRANDOMIZERCODE_H
