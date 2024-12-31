#include "headers/sv_randomizer_headers/svrandomizercode.h"
#include "headers/sv_randomizer_headers/sv_starters_gifts.h"
#include <QString>
#include <QMap>
#include <QDebug>

// Look into QSharedData in the future

SVRandomizerCode::SVRandomizerCode()
{}


SVRandomizerCode::~SVRandomizerCode()
{}

bool SVRandomizerCode::svRandomizeStarters(SVStarters starters){
    starters.randomize_starters();

    return true;
}

bool SVRandomizerCode::svRandomizeGifts(SVStarters gifts){
    gifts.randomize_gifts();

    return true;
}

bool SVRandomizerCode::svRandomizeStats(SVStats stats){
    stats.randomize_stats();

    return true;
}

bool SVRandomizerCode::svRandomizeItem(SVItems items){
    items.randomize_items();

    return true;
}

bool SVRandomizerCode::svRandomizeWilds(SVWilds wilds){
    if(wilds.paldea_Settings_for_all_wild == true){
        if(wilds.randomize_paldea_wild == true){
            wilds.randomize_paldea_wilds("pokedata_array.json");
            wilds.randomize_paldea_wilds("pokedata_su1_array.json");
            wilds.randomize_paldea_wilds("pokedata_su2_array.json");
            wilds.randomize_statics();
        }
    }else{
        if(wilds.randomize_paldea_wild == true){
            wilds.randomize_paldea_wilds("pokedata_array.json");
            wilds.randomize_statics();
        }
        if(wilds.randomize_kitakami_wild == true){
            wilds.randomize_kitakami_wilds();
        }
        if(wilds.randomize_blueberry_wild == true){
            wilds.randomize_blueberry_wilds();
        }
    }

    return true;
}

bool SVRandomizerCode::svRandomizeRaids(SVRaids raids){
    if(raids.paldea_Settings_for_all_raids == true){
        if(raids.praids_randomize == true){
            raids.randomize_paldean_raids("raid_enemy_0");
            raids.randomize_paldean_raids("su1_raid_enemy_0");
            raids.randomize_paldean_raids("su2_raid_enemy_0");
        }
    }else{
        if(raids.praids_randomize == true){
            raids.randomize_paldean_raids("raid_enemy_0");
        }
        if(raids.kraids_randomize == true){
            raids.randomize_kitakami_raids("su1_raid_enemy_0");
        }
        if(raids.braids_randomize == true){
            raids.randomize_blueberry_raids("su2_raid_enemy_0");
        }
    }

    return true;
}

bool SVRandomizerCode::svRandomizeTrainers(SVTrainers trainers){
    if(trainers.use_trainer_paldea_for_all == true){
        if(trainers.randomize_paldea_trainers == true){
            trainers.openFiles();
            qDebug()<<"Inside - 2";
            if(trainers.pglobal_trainer_randomizer_settings[5] == true){
                qDebug()<<"Inside";
                QList<int> indicesToCheck = {};
                indicesToCheck = trainers.randomize_nemona;
                indicesToCheck += trainers.randomize_geeta;
                indicesToCheck += trainers.randomize_clavell;
                indicesToCheck += trainers.randomize_team_star;
                indicesToCheck += trainers.randomize_e4_paldea;
                indicesToCheck += trainers.randomize_gym;
                indicesToCheck += trainers.randomize_professors;
                indicesToCheck += trainers.randomize_arven;
                indicesToCheck += trainers.randomize_kieran_su1;
                indicesToCheck += trainers.randomize_kieran_su2;
                indicesToCheck += trainers.randomize_carmine_su1;
                indicesToCheck += trainers.randomize_carmine_su2;
                indicesToCheck += trainers.randomize_ogre_clan;
                indicesToCheck += trainers.randomize_bb_e4;
                indicesToCheck += trainers.randomize_bb_league;
                indicesToCheck += trainers.randomize_school_professors;
                indicesToCheck += trainers.randomize_professor_dragon;
                indicesToCheck += trainers.randomize_arven_titan;
                indicesToCheck += trainers.randomize_kieran_terapagos;
                indicesToCheck += trainers.randomize_perrin;
                indicesToCheck += trainers.randomize_billy_onare;
                indicesToCheck += trainers.randomize_penny;
                indicesToCheck += wild_trainer_index;
                indicesToCheck += multi_battles_index;
                indicesToCheck += raid_npc_index;
                trainers.randomize_trainers(indicesToCheck, trainers.prival_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);
            }else{
                qDebug()<<"Inside - how?";
                QList<int> indicesToCheck = {};
                indicesToCheck = trainers.randomize_nemona;
                indicesToCheck += trainers.randomize_arven;
                indicesToCheck += trainers.randomize_penny;
                indicesToCheck += trainers.randomize_clavell;
                indicesToCheck += trainers.randomize_team_star;
                indicesToCheck += trainers.randomize_arven_titan;
                indicesToCheck += trainers.randomize_kieran_terapagos;
                indicesToCheck += trainers.randomize_kieran_su1;
                indicesToCheck += trainers.randomize_carmine_su1;
                indicesToCheck += trainers.randomize_carmine_su2;
                indicesToCheck += trainers.randomize_school_professors;
                trainers.randomize_trainers(indicesToCheck, trainers.prival_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = wild_trainer_index;
                indicesToCheck += multi_battles_index;
                trainers.randomize_trainers(indicesToCheck, trainers.proute_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = trainers.randomize_gym;
                trainers.randomize_trainers(indicesToCheck, trainers.pgym_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = trainers.randomize_bb_e4;
                indicesToCheck += trainers.randomize_e4_paldea;
                indicesToCheck += trainers.randomize_ogre_clan;
                trainers.randomize_trainers(indicesToCheck, trainers.pelite4_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = raid_npc_index;
                trainers.randomize_trainers(indicesToCheck, trainers.praid_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = trainers.randomize_bb_league;
                indicesToCheck += trainers.randomize_geeta;
                indicesToCheck += trainers.randomize_kieran_su2;
                indicesToCheck += trainers.randomize_professors;
                trainers.randomize_trainers(indicesToCheck, trainers.pchampion_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);
            }

            trainers.closeFiles();
        }

    }else{
        trainers.openFiles();
        if(trainers.randomize_paldea_trainers == true){
            if(trainers.pglobal_trainer_randomizer_settings[5] == true){
                QList<int> indicesToCheck = {};
                indicesToCheck = trainers.randomize_nemona;
                indicesToCheck += trainers.randomize_geeta;
                indicesToCheck += trainers.randomize_clavell;
                indicesToCheck += trainers.randomize_team_star;
                indicesToCheck += trainers.randomize_e4_paldea;
                indicesToCheck += trainers.randomize_gym;
                indicesToCheck += trainers.randomize_professors;
                indicesToCheck += trainers.randomize_arven;
                indicesToCheck += trainers.randomize_professor_dragon;
                indicesToCheck += trainers.randomize_arven_titan;
                indicesToCheck += trainers.randomize_penny;
                trainers.randomize_trainers(indicesToCheck, trainers.prival_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

            }else{
                QList<int> indicesToCheck = {};
                indicesToCheck = trainers.randomize_nemona;
                indicesToCheck += trainers.randomize_arven;
                indicesToCheck += trainers.randomize_penny;
                indicesToCheck += trainers.randomize_clavell;
                indicesToCheck += trainers.randomize_team_star;
                indicesToCheck += trainers.randomize_arven_titan;
                indicesToCheck += trainers.randomize_school_professors;
                trainers.randomize_trainers(indicesToCheck, trainers.prival_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = wild_trainer_index;
                indicesToCheck += multi_battles_index;
                trainers.randomize_trainers(indicesToCheck, trainers.proute_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = trainers.randomize_gym;
                trainers.randomize_trainers(indicesToCheck, trainers.pgym_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = trainers.randomize_bb_e4;
                indicesToCheck += trainers.randomize_e4_paldea;
                indicesToCheck += trainers.randomize_ogre_clan;
                trainers.randomize_trainers(indicesToCheck, trainers.pelite4_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = raid_npc_index;
                trainers.randomize_trainers(indicesToCheck, trainers.praid_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

                indicesToCheck = trainers.randomize_bb_league;
                indicesToCheck += trainers.randomize_geeta;
                indicesToCheck += trainers.randomize_kieran_su2;
                indicesToCheck += trainers.randomize_professors;
                trainers.randomize_trainers(indicesToCheck, trainers.pchampion_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.ptrainersgeneration);

            }

        }
        if(trainers.randomize_kitakami_trainers == true){

            if(trainers.kglobal_trainer_randomizer_settings[5] == true){
                QList<int> indicesToCheck = {};
                indicesToCheck = trainers.randomize_kieran_su1;
                indicesToCheck += trainers.randomize_carmine_su1;
                indicesToCheck += trainers.randomize_ogre_clan;
                indicesToCheck += trainers.randomize_perrin;
                indicesToCheck += trainers.randomize_billy_onare;
                trainers.randomize_trainers(indicesToCheck, trainers.krival_randomizer, trainers.kglobal_trainer_randomizer_settings, trainers.ktrainersgeneration);
            }else{
                QList<int> indicesToCheck = {};
                indicesToCheck = trainers.randomize_kieran_su1;
                indicesToCheck += trainers.randomize_carmine_su1;
                indicesToCheck += trainers.randomize_perrin;
                indicesToCheck += trainers.randomize_billy_onare;
                trainers.randomize_trainers(indicesToCheck, trainers.krival_randomizer, trainers.kglobal_trainer_randomizer_settings, trainers.ktrainersgeneration);

                indicesToCheck = wild_trainer_index;
                indicesToCheck += multi_battles_index;
                trainers.randomize_trainers(indicesToCheck, trainers.kroute_randomizer, trainers.kglobal_trainer_randomizer_settings, trainers.ktrainersgeneration);

                indicesToCheck = trainers.randomize_ogre_clan;
                trainers.randomize_trainers(indicesToCheck, trainers.kogre_clan_randomizer, trainers.kglobal_trainer_randomizer_settings, trainers.ktrainersgeneration);

                indicesToCheck = raid_npc_index;
                trainers.randomize_trainers(indicesToCheck, trainers.kraid_randomizer, trainers.kglobal_trainer_randomizer_settings, trainers.ktrainersgeneration);
            }
        }
        if(trainers.randomize_blueberry_trainers == true){
            if(trainers.bglobal_trainer_randomizer_settings[5] == true){
                QList<int> indicesToCheck = {};
                indicesToCheck = trainers.randomize_kieran_su2;
                indicesToCheck += trainers.randomize_carmine_su2;
                indicesToCheck += trainers.randomize_bb_league;
                indicesToCheck += trainers.randomize_bb_e4;
                indicesToCheck += trainers.randomize_kieran_terapagos;
                trainers.randomize_trainers(indicesToCheck, trainers.brival_randomizer, trainers.bglobal_trainer_randomizer_settings, trainers.btrainersgeneration);
            }else{
                QList<int> indicesToCheck = {};
                indicesToCheck = trainers.randomize_kieran_su2;
                indicesToCheck += trainers.randomize_carmine_su2;
                indicesToCheck += trainers.randomize_kieran_terapagos;
                trainers.randomize_trainers(indicesToCheck, trainers.brival_randomizer, trainers.bglobal_trainer_randomizer_settings, trainers.btrainersgeneration);

                indicesToCheck = wild_trainer_index;
                indicesToCheck += multi_battles_index;
                trainers.randomize_trainers(indicesToCheck, trainers.broute_randomizer, trainers.bglobal_trainer_randomizer_settings, trainers.btrainersgeneration);

                indicesToCheck = trainers.randomize_bb_e4;
                indicesToCheck += trainers.randomize_bb_league;
                trainers.randomize_trainers(indicesToCheck, trainers.b_bb4_randomizer, trainers.bglobal_trainer_randomizer_settings, trainers.btrainersgeneration);

                indicesToCheck = raid_npc_index;
                trainers.randomize_trainers(indicesToCheck, trainers.b_bb4_randomizer, trainers.bglobal_trainer_randomizer_settings, trainers.btrainersgeneration);

            }
        }
        trainers.closeFiles();
    }
    return true;
}
