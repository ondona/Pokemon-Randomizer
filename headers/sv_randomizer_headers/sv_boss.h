#ifndef SV_BOSS_H
#define SV_BOSS_H
#include "headers/sv_randomizer_headers/sv_shared_class.h"
#include <QDir>

class SVBoss: public SVShared{
    public:
        bool randomize_bosses = false;
        /* Key: [0] Legends Only
         *      [1] Paradox Only
         *      [2] Legends and Paradox Only
         *      [3] Ban Stage 1
         *      [4] Ban Stage 2
         *      [5] Ban Stage 3
         *      [6] Ban 1 Stage
         */
        QVector<bool> boss_settings = {false, false, false, false, false, false, false};
        QVector<bool> boss_generation= {false, false, false, false, false, false, false, false, false};


        allowedPokemonLimiter BossLimiter;
        QList<int> BossAllowedPokemon;

        void randomizeBosses(QDir dir);
        void randomizeFight(unsigned long long index);
        void randomizeStellarOgerpon(unsigned long long index);
        void copyFight(unsigned long long indexSet, unsigned long long indexCopy);
        void copyStellarOgerpon(unsigned long long indexSet, unsigned long long indexCopy);
        void patchMultiBattle();
        void patchGimmighoul();
        void patchLeChonk();
        void patchHoundoom();
        void patchSunflora();
        void patchDonzoTitan();
        void patchOrthwormTitan();
        void patchBombariderTitan();
        void patchKlawfTitan();
        void patchGreatIronTitan();
        void patchTingLu();
        void patchChienPao();
        void patchWoChien();
        void patchChiYu();
        void patchLoyalThreeOgerpon();
        void patchKitakami();
        void patchStellarUnderdeepths();
        void patchTerapagos();
        void patchAreaZeroLegends();
        void patchSnacksworths();
        void obtainPokemonScene(int &dev, int &form, int& gender, int &rare);
        void saveIndividualPokemon(int index, std::vector<int> &dev, std::vector<int> &form, std::vector<int> &gender, std::vector<bool> &rare);
        void changeScene(std::vector<std::pair<std::string, std::string>> filePairs,
                         std::vector<int> &dev, std::vector<int> &form, std::vector<int> &gender, std::vector<bool> &rare,
                         std::string output, std::string romAddress);
        void changeSceneOne(std::vector<std::pair<std::string, std::string>> filePairs,
                                std::vector<int> &dev, std::vector<int> &form, std::vector<int> &gender, std::vector<bool> &rare,
                                std::string output, std::string romAddress);
        void savePokemonFromTrainer(int id, std::string& pokeKey,
                                    std::vector<int> &dev, std::vector<int> &form, std::vector<int> &gender, std::vector<bool> &rare);
};

#endif // SV_BOSS_H
