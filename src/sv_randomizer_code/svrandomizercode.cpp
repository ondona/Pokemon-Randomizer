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

bool SVRandomizerCode::svRandomizeBoss(SVBoss boss, QDir dir){
    boss.randomizeBosses(dir);

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
	QList<int> RivalsList = {};
	QList<int> WildsList = {};
	QList<int> E4List = {};
	QList<int> ChampionsList = {};

	QVector<bool> BB4Settings = trainers.b_bb4_randomizer;
	QList<int> BBB4Allowed = trainers.BBB4AllowedPokemon;
	
	if(trainers.bglobal_trainer_randomizer_settings[5] == true){//if use rival for all, change lists to rival lists
	QVector<bool> BB4Settings = trainers.brival_randomizer;
	QList<int> BBB4Allowed = trainers.BRivalAllowedPokemon;
	}
	
	QVector<bool> OCSettings = trainers.kogre_clan_randomizer;
	QList<int> OCAllowed = trainers.KOCAllowedPokemon;
	
	if(trainers.kglobal_trainer_randomizer_settings[5] == true){//if use rival for all, change lists to rival lists
	QVector<bool> OCSettings = trainers.krival_randomizer;
	QList<int> OCAllowed = trainers.KRivalAllowedPokemon;
	}
	//store these to reduce line counts and if statements
	QVector<bool> PRouteSettings = trainers.proute_randomizer;
	QVector<bool> E4Settings = trainers.pelite4_randomizer;
	QVector<bool> GymSettings = trainers.pgym_randomizer;
	QVector<bool> PRaidSettings = trainers.praid_randomizer;
	QVector<bool> PChampionSettings = trainers.pchampion_randomizer;
	QList<int> PRouteAllowed = trainers.PRouteAllowedPokemon;
	QList<int> E4Allowed = trainers.PE4AllowedPokemon;
	QList<int> GymAllowed = trainers.PGymAllowedPokemon;
	QList<int> ChampionAllowed = trainers.PChampionAllowedPokemon;
	QList<int> RaidAllowed = trainers.PRaidAllowedPokemon;
	
	if(trainers.pglobal_trainer_randomizer_settings[5] == true){//if use rival for all, change lists to rival lists
	QVector<bool> PRouteSettings = trainers.prival_randomizer;
	QVector<bool> E4Settings = trainers.prival_randomizer;
	QVector<bool> GymSettings = trainers.prival_randomizer;
	QVector<bool> PRaidSettings = trainers.prival_randomizer;
	QVector<bool> PChampionSettings = trainers.prival_randomizer;
	QList<int> PRouteAllowed = trainers.PRivalAllowedPokemon;
	QList<int> E4Allowed = trainers.PRivalAllowedPokemon;
	QList<int> GymAllowed = trainers.PRivalAllowedPokemon;
	QList<int> ChampionAllowed = trainers.PRivalAllowedPokemon;
	QList<int> RaidAllowed = trainers.PRivalAllowedPokemon;
	}
	//OPEN FILES
	trainers.openFiles();
	//Start with Rivals, Reverse Order for use paldea for all checks
	RivalsList = trainers.randomize_kieran_terapagos;
	RivalsList += trainers.randomize_carmine_su2;
	RivalsList += trainers.randomize_kieran_su2;//this was in champion section but then rival in another
	
	if(trainers.use_trainer_paldea_for_all == false){//check if they using paldea for all, if not randomize current list and reset it
	trainers.randomizeTrainerFights(RivalsList, trainers.brival_randomizer, trainers.bglobal_trainer_randomizer_settings, trainers.BRivalAllowedPokemon);
	//Kitakami - rivals
	RivalsList = trainers.randomize_kieran_su1;
	}else{
	RivalsList += trainers.randomize_kieran_su1;}//if not use paldea, amend the line instead of reset list
	
	RivalsList += trainers.randomize_carmine_su1;
	RivalsList += trainers.randomize_perrin;//was missing from else, assumed here
	RivalsList += trainers.randomize_billy_onare;//was missing from else, assumed here
	
	if(trainers.use_trainer_paldea_for_all == false){//check if they using paldea for all, if not randomize current list and reset it
	trainers.randomizeTrainerFights(RivalsList, trainers.krival_randomizer, trainers.kglobal_trainer_randomizer_settings, trainers.KRivalAllowedPokemon);
	//Paldea - rivals
	RivalsList = trainers.randomize_nemona;
	}else{
	RivalsList += trainers.randomize_nemona;}//if not use paldea, amend the line instead of reset list
	
	RivalsList += trainers.randomize_arven;
	RivalsList += trainers.randomize_penny;
	RivalsList += trainers.randomize_clavell;
	RivalsList += trainers.randomize_team_star;
	RivalsList += trainers.randomize_arven_titan;
	RivalsList += trainers.randomize_school_professors;
	RivalsList += trainers.randomize_professor_dragon;//was missing from else, assumed here
	trainers.randomizeTrainerFights(RivalsList, trainers.prival_randomizer, trainers.pglobal_trainer_randomizer_settings, trainers.PRivalAllowedPokemon);
	
	//Wilds trainers - was already using the same list for all 3 sections, changed to prioritize paldea settings for now
	WildsList = wild_trainer_index;
	WildsList += multi_battles_index;
	trainers.randomizeTrainerFights(WildsList, PRouteSettings, trainers.pglobal_trainer_randomizer_settings, PRouteAllowed);
	/*trainers.randomizeTrainerFights(indicesToCheck, trainers.kroute_randomizer, trainers.kglobal_trainer_randomizer_settings, trainers.KRouteAllowedPokemon);
	trainers.randomizeTrainerFights(indicesToCheck, trainers.broute_randomizer, trainers.bglobal_trainer_randomizer_settings, trainers.BRouteAllowedPokemon);*/
	
	//Gyms
	trainers.randomizeTrainerFights(trainers.randomize_gym, GymSettings, trainers.pglobal_trainer_randomizer_settings, GymAllowed);
	
	//Elite 4/OC/BB - again reverse order
	E4List = trainers.randomize_bb_league;
	E4List += trainers.randomize_bb_e4;
	if(trainers.use_trainer_paldea_for_all == false){//check if they using paldea for all, if not randomize current list as BB, do OC then reset it for paldea
	trainers.randomizeTrainerFights(E4List, BB4Settings, trainers.bglobal_trainer_randomizer_settings, BBB4Allowed);
	trainers.randomizeTrainerFights(trainers.randomize_ogre_clan, OCSettings, trainers.kglobal_trainer_randomizer_settings, OCAllowed);
	E4List = trainers.randomize_e4_paldea;
	}else{//if use paldea for all the entries instead of resetting list to finish with paldea
	E4List += trainers.randomize_ogre_clan;
	E4List += trainers.randomize_e4_paldea;
	}
	trainers.randomizeTrainerFights(E4List, E4Settings, trainers.pglobal_trainer_randomizer_settings, E4Allowed);
	
	//Raids - was using the same list for all sections, defaulting to use paldea settings
	trainers.randomizeTrainerFights(raid_npc_index, PRaidSettings, trainers.pglobal_trainer_randomizer_settings, RaidAllowed);
	/*trainers.randomizeTrainerFights(indicesToCheck, trainers.kraid_randomizer, trainers.kglobal_trainer_randomizer_settings, trainers.KRaidAllowedPokemon);
	trainers.randomizeTrainerFights(indicesToCheck, trainers.braid_randomizer, trainers.bglobal_trainer_randomizer_settings, trainers.BRaidAllowedPokemon);*/
	
	//Champions
	ChampionsList = trainers.randomize_geeta;
	ChampionsList += trainers.randomize_professors;
	trainers.randomizeTrainerFights(ChampionsList, PChampionSettings, trainers.pglobal_trainer_randomizer_settings, ChampionAllowed);
	
	//CLOSE FILES
	trainers.closeFiles();
    return true;
}