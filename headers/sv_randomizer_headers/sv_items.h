#ifndef SV_ITEMS_H
#define SV_ITEMS_H

#include <headers/sv_randomizer_headers/sv_shared_class.h>
#include <QVector>
#include <QMap>

class SVItems: public SVShared{
    public:
        SVItems();
        ~SVItems();

        //Items
        // bool enable_items;
        bool randomize_hidden_items = false; // Done
        bool randomize_ball_items = false; // TBD
        bool randomize_pickup_items = false;
        bool randomize_pokemon_drops = false;
        bool randomize_synchro_items = false;

        bool randomize_items(int seed, int run);
        void randomizeHiddenItems(std::string fileName, std::string saveFile);
        void randomizePickUpItems();
        void randomizeLetsGoItems();
        void randomizePokemonDrops();
};

#endif // SV_ITEMS_H
