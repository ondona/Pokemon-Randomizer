#ifndef SVRANDOMIZERCODE_H
#define SVRANDOMIZERCODE_H

#include <QString>
#include <QMap>
#include "sv_shared_class.h"
// #include "sv_trainers.h"
// #include "sv_raids.h"
// #include "sv_wild_static.h"
#include "sv_items.h"
#include "sv_personal.h"
#include "sv_starters.h"
// #include "sv_boss.h"


class SVRandomizerCode: public SVShared
{
public:
    SVRandomizerCode();
    ~SVRandomizerCode();

    //General
    bool auto_patch = true; // Added
    bool kaizo_mode = false;       // Added
    unsigned int bulk_amount = 1;
    QString seed = "";

    svStarters svRandomizerStarters;
    svPersonal svRandomizerPersonal;
    svItems svRandomizerItems;

    //void setSeed

};

#endif // SVRANDOMIZERCODE_H
