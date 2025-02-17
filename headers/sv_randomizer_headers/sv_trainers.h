#ifndef SV_TRAINERS_H
#define SV_TRAINERS_H

#include <QVector>
#include <QMap>
#include "headers/sv_randomizer_headers/sv_shared_class.h"

class SVTrainers: public SVShared
{
    public:
        //Paldea
        /*
         * Key:
         *    Global: [0] - Allow Terastalization
         *            [1] - Randomize Terastalization
         *            [2] - Allow Shiny Pokemon
         *            [3] - Randomnly Choose Between Singles/Doubles
         *            [4] - Only Double Battles
         *            [5] - Use Rival Settings for All
         *    All Other Options:
         *            [0] - Force 6 Pokemon
         *            [1] - Give Extra Pokemon
         *            [2] - Allow Terastalization
         *            [3] - Force Perfect IVs
         *            [4] - Make AI Smart
         *            [5] - Only Legends
         *            [6] - Only Paradox
         *            [7] - Only Legends and Paradox
         *            [8] - Ban Stage 1 Pokemon
         *            [9] - Ban Stage 2 Pokemon
         *            [10] - Ban Stage 3 Pokemon
         *            [11] - Ban 1 Stage Pokemon
         */
        bool randomize_paldea_trainers = false;
        bool randomize_kitakami_trainers = false;
        bool randomize_blueberry_trainers = false;
        QVector<bool> prival_randomizer= {false, false, false,
                                           false, false, false,
                                           false, false, false,
                                           false, false, false};
        QVector<bool> proute_randomizer= {false, false, false,
                                           false, false, false,
                                           false, false, false,
                                           false, false, false};
        QVector<bool> pgym_randomizer= {false, false, false,
                                         false, false, false,
                                         false, false, false,
                                         false, false, false};
        QVector<bool> pelite4_randomizer= {false, false, false,
                                            false, false, false,
                                            false, false, false,
                                            false, false, false};
        QVector<bool> pchampion_randomizer= {false, false, false,
                                              false, false, false,
                                              false, false, false,
                                              false, false, false};
        QVector<bool> praid_randomizer= {false, false, false,
                                          false, false, false,
                                          false, false, false,
                                          false, false, false};
        QVector<bool> pglobal_trainer_randomizer_settings = {false, false, false, false, false, false};
        QVector<bool> ptrainersgeneration= {false, false, false,
                                             false, false, false,
                                             false, false, false};
        bool use_trainer_paldea_for_all = false;

        //Kitakami
        QVector<bool> krival_randomizer= {false, false, false,
                                           false, false, false,
                                           false, false, false,
                                           false, false, false};
        QVector<bool> kroute_randomizer= {false, false, false,
                                           false, false, false,
                                           false, false, false,
                                           false, false, false};
        QVector<bool> kogre_clan_randomizer= {false, false, false,
                                               false, false, false,
                                               false, false, false,
                                               false, false, false};
        QVector<bool> kraid_randomizer= {false, false, false,
                                          false, false, false,
                                          false, false, false,
                                          false, false, false};
        QVector<bool> kglobal_trainer_randomizer_settings = {false, false, false, false, false, false};
        QVector<bool> ktrainersgeneration= {false, false, false,
                                             false, false, false,
                                             false, false, false,
                                             false, false, false};

        //Blueberry
        QVector<bool> brival_randomizer= {false, false, false,
                                           false, false, false,
                                           false, false, false,
                                           false, false, false};
        QVector<bool> broute_randomizer= {false, false, false,
                                           false, false, false,
                                           false, false, false,
                                           false, false, false};
        QVector<bool> b_bb4_randomizer= {false, false, false,
                                          false, false, false,
                                          false, false, false,
                                          false, false, false};
        QVector<bool> braid_randomizer= {false, false, false,
                                          false, false, false,
                                          false, false, false,
                                          false, false, false};
        QVector<bool> bglobal_trainer_randomizer_settings = {false, false, false, false, false, false};
        QVector<bool> btrainersgeneration= {false, false, false,
                                             false, false, false,
                                             false, false, false};
        void randomizeTrainerFights(QList<int> FightLists, QVector<bool> settings, QVector<bool> globalSettings, QList<int> AllowedMons);
        
		void openFiles();
        void closeFiles();
        void noSoftlockParadise(QList<int> AllowedMons);
        void noSoftlockTerapagos(QList<int> AllowedMons);
        QList<int> randomize_penny = {356, 357, 358, 677, 678};
        QList<int> randomize_geeta = {359, 360, 361};
        QList<int> randomize_clavell = {362, 363, 364, 365, 366, 367, 368};
        QList<int> randomize_team_star = {369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385};
        QList<int> randomize_e4_paldea = {386, 387, 388, 389, 390};
        QList<int> randomize_gym = {391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410,
                                    411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422};
        QList<int> randomize_professors = {436, 438};
        QList<int> randomize_arven = {426, 427, 428, 429, 430, 701, 702};
        QList<int> randomize_nemona = {494, 498, 502, 509, 513, 495, 499, 503, 506, 510, 514, 517, 520, 496, 500, 504, 507, 511, 515, 518, 521,
                                       497, 501, 505, 508, 512, 516, 519, 522, 523, 524, 525, 703, 704, 705, 706, 707, 708};
        QList<int> randomize_kieran_su1 = {585, 587, 589, 591, 593, 586, 588, 590, 592, 594};
        QList<int> randomize_kieran_su2 = {679, 681, 682};
        QList<int> randomize_carmine_su1 = {599, 601, 603, 600, 602, 604, 605, 606, 607, 608};
        QList<int> randomize_carmine_su2 = {710, 711};
        QList<int> randomize_ogre_clan = {609, 610, 611, 612, 613, 614, 615, 616};
        QList<int> randomize_bb_e4 = {683, 684, 685, 686, 687, 688, 689, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700};
        QList<int> randomize_bb_league = {709, 712, 713, 714, 715, 716, 717, 718, 719, 720, 721, 722, 723, 724, 725, 726, 727, 728, 729, 730,
                                          731, 732, 733, 734, 735, 736, 737, 738, 739, 740, 741, 742, 743, 744, 745, 746};
        QList<int> randomize_school_professors = {423, 424, 425, 492, 526, 527, 747, 754, 755};
        QList<int> randomize_professor_dragon = {437, 439};
        QList<int> randomize_arven_titan = {431, 432, 433, 434, 435};
        QList<int> randomize_kieran_terapagos = {680};
        QList<int> randomize_perrin = {595};
        QList<int> randomize_billy_onare = {596, 597, 598};
		
		
        allowedPokemonLimiter PRivalLimiter;
        QList<int> PRivalAllowedPokemon;
        allowedPokemonLimiter PRouteLimiter;
        QList<int> PRouteAllowedPokemon;
        allowedPokemonLimiter PGymLimiter;
        QList<int> PGymAllowedPokemon;
        allowedPokemonLimiter PE4Limiter;
        QList<int> PE4AllowedPokemon;
        allowedPokemonLimiter PRaidLimiter;
        QList<int> PRaidAllowedPokemon;
        allowedPokemonLimiter PChampionLimiter;
        QList<int> PChampionAllowedPokemon;
		
        allowedPokemonLimiter KRivalLimiter;
        QList<int> KRivalAllowedPokemon;
        allowedPokemonLimiter KRouteLimiter;
        QList<int> KRouteAllowedPokemon;
        allowedPokemonLimiter KOCLimiter;
        QList<int> KOCAllowedPokemon;
        allowedPokemonLimiter KRaidLimiter;
        QList<int> KRaidAllowedPokemon;
		
        allowedPokemonLimiter BRivalLimiter;
        QList<int> BRivalAllowedPokemon;
        allowedPokemonLimiter BRouteLimiter;
        QList<int> BRouteAllowedPokemon;
        allowedPokemonLimiter BBB4Limiter;
        QList<int> BBB4AllowedPokemon;
        allowedPokemonLimiter BRaidLimiter;
        QList<int> BRaidAllowedPokemon;
		
};



#endif // SV_TRAINERS_H
