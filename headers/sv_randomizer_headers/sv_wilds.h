#ifndef SV_WILDS_H
#define SV_WILDS_H

#include "sv_shared_class.h"

class svWilds: public QObject, SVShared{
    Q_OBJECT

    public:
        bool ogerponTerapagosPaldea = false;
        bool ogerponTerapagosKitakami = false;
        bool ogerponTearapagosBlueberry = false;

        bool balanceBSTPaldea = false;
        bool balanceBSTKitakami = false;
        bool balanceBSTBlueberry = false;

        bool randomizePaldeaWild = false;
        bool randomizeBlueberryWild = false;
        bool randomizeKitakamiWild = false;

        bool paldeaForAll = false;
        bool typesChanged = false;

        allowedPokemonLimiter paldeaWilds;
        allowedPokemonLimiter kitakamiWilds;
        allowedPokemonLimiter blueberrWilds;

        QMap<int, QList<int>> usableWildPokemon;

        void randomize();

    private:
        QString path;
        QString schema;
        QString output;
        QString pickRandomBiome();
        QString generateAreaList();
        void randomizeWilds(QMap<int, QList<int>> allowedPokemon, bool ogerponTerapagos, bool bstBalance = false);
};

#endif // SV_WILDS_H
