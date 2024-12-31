#ifndef SVRANDOMIZERWINDOW_H
#define SVRANDOMIZERWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTabBar>
#include <QStackedWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include "sv_randomizer_headers/svrandomizercode.h"

class SVRandomizerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SVRandomizerWindow(QWidget *parent = nullptr);
    ~SVRandomizerWindow();

private:
    void createLayout();

    // All Options Variables
    QPushButton *importButton;
    QPushButton *exportButton;
    QPushButton *startRandomizer;
    QLineEdit *seed;
    SVRandomizerCode randomizer;

    //GUI Variables
    QWidget *centralWidget;
    QTabBar *topBar;
    QTabBar *innerWildBar;
    QTabBar *innerRaidBar;
    QTabBar *innerTrainerBar;
    QStackedWidget *stackedWidget;
    QStackedWidget *innerWildStackWidget;
    QStackedWidget *innerRaidStackWidget;
    QStackedWidget *innerTrainerStackWidget;

    QLabel *previewLabel;
    // Gift and General Variables
        //General
        QCheckBox *auto_patch; // Added
        QCheckBox *kaizo_mode; // Added
        QSpinBox *bulk_amount; // Added

        //Starters
        QVector<QCheckBox*> generations_starters; // Added
        QCheckBox *enable_starters; // Added
        QCheckBox *force_shiny_starter; // Added
        QCheckBox *all_starters_shiny; // Added
        QSpinBox *shiny_starter_rate; // Added
        QCheckBox *show_starters_in_overworld; // Added
        QCheckBox *randomize_starters_tera_types; // Added
        QCheckBox *ban_stage_1_pokemon_starters; // Added
        QCheckBox *ban_stage_2_pokemon_starters; // Added
        QCheckBox *ban_stage_3_pokemon_starters; // Added
        QCheckBox *ban_single_stage_pokemon_starters; // Added
        QCheckBox *only_legendary_pokemon_starters; // Added
        QCheckBox *only_paradox_pokemon_starters; // Added
        QCheckBox *only_legends_and_paradox_starters; // Added
        QVector<QLineEdit *> starters; // Added
        QVector<QCheckBox *> starters_shiny; // Added
        QVector<QComboBox *> starters_form; // Added
        QVector<QComboBox *> starters_gender; // Added
        QVector<QComboBox *> starters_pokeball; // Added


        // Gifts
        QCheckBox *enable_gifts; // Added
        QSpinBox *shiny_static_rate; // Added
        QCheckBox *randomize_tera_types; // Added
        QVector<QCheckBox*> generation_gifts; // Added

    // Stats, TMs and Items Variables
        //Stats
        // QCheckBox *enable_stats; // Added
        QCheckBox *randomize_abilities; // Added
        QCheckBox *randomize_types; // Added
        QCheckBox *give_extra_types; // Added
        QCheckBox *ban_wonder_guard; // Added
        QCheckBox *randomize_moveset; // Added
        QCheckBox *moveset_same_as_type; // Added
        QCheckBox *randomize_bst; // Added
        QCheckBox *keep_same_bst; // Added
        QCheckBox *randomize_evolutions; // Added
        QCheckBox *force_unobtainable_evolutions_at_night; // Added
        QCheckBox *evolve_every_5_levels; // Added
        QCheckBox *force_tera_blast_every_5_levels; // Added
        QVector<QCheckBox*> generation_stats; // Added

        //TMs
        QCheckBox *enable_TMs; // Added
        QCheckBox *randomize_tms; // Added
        QCheckBox *randomize_pokemon_tms; // Added
        QCheckBox *allow_moves_without_animation; // Added

        //Items
        // QCheckBox *enable_items;
        QCheckBox *randomize_hidden_items; // Added
        QCheckBox *randomize_ball_items; // TBD
        QCheckBox *randomize_pickup_items; // Added
        QCheckBox *randomize_pokemon_drops; // Added
        QCheckBox *randomize_synchro_items; // Added

    // Wild and Static Encounters
        QCheckBox *randomize_paldea_wild;
        QCheckBox *paldea_ExcludeLegends; // Added
        QCheckBox *paldea_OnlyLegends; // Added
        QCheckBox *paldea_OnlyParadox; // Added
        QCheckBox *paldea_OnlyLegendsAndParadox; // Added
        QCheckBox *paldea_BanStage1; // Added
        QCheckBox *paldea_BanStage2; // Added
        QCheckBox *paldea_BanStage3; // Added
        QCheckBox *paldea_Ban1Stage; // Added
        QCheckBox *paldea_Settings_for_all_wild; // Added
        QCheckBox *paldea_BalanceAreaPerBST; //TBD
        QCheckBox *let_ogre_pagos_spawn;
        QVector<QCheckBox*> generation_paldea_wild; // Added

        QCheckBox *randomize_kitakami_wild;
        QCheckBox *kitakami_ExcludeLegends; // Added
        QCheckBox *kitakami_OnlyLegends; // Added
        QCheckBox *kitakami_OnlyParadox; // Added
        QCheckBox *kitakami_OnlyLegendsAndParadox; // Added
        QCheckBox *kitakami_BanStage1; // Added
        QCheckBox *kitakami_BanStage2; // Added
        QCheckBox *kitakami_BanStage3; // Added
        QCheckBox *kitakami_Ban1Stage; // Added
        QCheckBox *kitakami_BalanceAreaPerBST; //TBD
        QVector<QCheckBox*> generation_kitakami_wild; // Added

        QCheckBox *randomize_blueberry_wild;
        QCheckBox *blueberry_ExcludeLegends; // Added
        QCheckBox *blueberry_OnlyLegends; // Added
        QCheckBox *blueberry_OnlyParadox; // Added
        QCheckBox *blueberry_OnlyLegendsAndParadox; // Added
        QCheckBox *blueberry_BanStage1; // Added
        QCheckBox *blueberry_BanStage2; // Added
        QCheckBox *blueberry_BanStage3; // Added
        QCheckBox *blueberry_Ban1Stage; // Added
        QCheckBox *blueberry_BalanceAreaPerBST; // TBD
        QVector<QCheckBox*> generation_blueberry_wild; // Added

        QCheckBox *statics_ExcludeLegends; // Added
        QCheckBox *statics_OnlyLegends; // Added
        QCheckBox *statics_OnlyParadox; // Added
        QCheckBox *statics_OnlyLegendsAndParadox; // Added
        QCheckBox *statics_BanStage1; // Added
        QCheckBox *statics_BanStage2; // Added
        QCheckBox *statics_BanStage3; // Added
        QCheckBox *statics_Ban1Stage; // Added
        QCheckBox *statics_BalanceAreaPerBST; //TBD
        QVector<QCheckBox*> generation_statics_wild; // Added
    // Raids
        QCheckBox *praids_randomize_per_star; // Added
        QCheckBox *praids_force_shiny; // Added
        QSpinBox *praids_shiny_chance; // Added
        QCheckBox *praids_onlyLegends; // Added
        QCheckBox *praids_onlyParadox; // Added
        QCheckBox *praids_onlyLegendsandParadox; // Added
        QCheckBox *praids_BanStage1; // Added
        QCheckBox *praids_BanStage2; // Added
        QCheckBox *praids_BanStage3; // Added
        QCheckBox *praids_Ban1Stage; // Added
        QCheckBox *praidsBalanceAreaPerBST; //TBD
        QCheckBox *paldea_Settings_for_all_raids; // Added
        QVector<QCheckBox*> praidsgeneration; // Added

        QCheckBox *kraids_randomize_per_star; // Added
        QCheckBox *kraids_force_shiny; // Added
        QSpinBox *kraids_shiny_chance; // Added
        QCheckBox *kraids_onlyLegends; // Added
        QCheckBox *kraids_onlyParadox; // Added
        QCheckBox *kraids_onlyLegendsandParadox; // Added
        QCheckBox *kraids_BanStage1; // Added
        QCheckBox *kraids_BanStage2; // Added
        QCheckBox *kraids_BanStage3; // Added
        QCheckBox *kraids_Ban1Stage; // Added
        QCheckBox *kraidsBalanceAreaPerBST; //TBD
        QVector<QCheckBox*> kraidsgeneration; // Added

        QCheckBox *braids_randomize_per_star; // Added
        QCheckBox *braids_force_shiny; // Added
        QSpinBox *braids_shiny_chance; // Added
        QCheckBox *braids_onlyLegends; // Added
        QCheckBox *braids_onlyParadox; // Added
        QCheckBox *braids_onlyLegendsandParadox; // Added
        QCheckBox *braids_BanStage1; // Added
        QCheckBox *braids_BanStage2; // Added
        QCheckBox *braids_BanStage3; // Added
        QCheckBox *braids_Ban1Stage; // Added
        QCheckBox *braidsBalanceAreaPerBST; //TBD
        QVector<QCheckBox*> braidsgeneration; // Added
        QCheckBox * praids_randomize;
        QCheckBox * kraids_randomize;
        QCheckBox * braids_randomize;
    //Trainers
        //Paldea
        QVector<QCheckBox*> prival_randomizer; // Done
        QVector<QCheckBox*> proute_randomizer; // Done
        QVector<QCheckBox*> pgym_randomizer; // Done
        QVector<QCheckBox*> pelite4_randomizer; // Done
        QVector<QCheckBox*> pchampion_randomizer; // Done
        QVector<QCheckBox*> praid_randomizer; // Done
        QVector<QCheckBox*> pglobal_trainer_randomizer_settings; // Done
        QVector<QCheckBox*> ptrainersgeneration; // Done
        QCheckBox *use_trainer_paldea_for_all; // Done

        //Kitakami
        QVector<QCheckBox*> krival_randomizer; // Done
        QVector<QCheckBox*> kroute_randomizer; // Done
        QVector<QCheckBox*> kogre_clan_randomizer; // Done
        QVector<QCheckBox*> kraid_randomizer; // Done
        QVector<QCheckBox*> kglobal_trainer_randomizer_settings; // Done
        QVector<QCheckBox*> ktrainersgeneration; // Done

        //Blueberry
        QVector<QCheckBox*> brival_randomizer; // Done
        QVector<QCheckBox*> broute_randomizer; // Done
        QVector<QCheckBox*> b_bb4_randomizer; // Done
        QVector<QCheckBox*> braid_randomizer; //Done
        QVector<QCheckBox*> bglobal_trainer_randomizer_settings; // Done
        QVector<QCheckBox*> btrainersgeneration; // Done

        QCheckBox *randomize_paldea_trainers;
        QCheckBox *randomize_kitakami_trainers;
        QCheckBox *randomize_blueberry_trainers;
    //Bosses
        QCheckBox *randomize_bosses; // Done
        QVector<QCheckBox*> boss_settings; // Done
        QVector<QCheckBox*> boss_generation; // Done

private slots:
    void addToFavorites();  // Slot to add this window to favorites
    void switchTabs(int i=0);
    void switchInnerWildTabs(int i=0);
    void switchInnerRaidTabs(int i=0);
    void switchInnerTrainerTabs(int i=0);

    QWidget* setupGiftWidget(); // Done
    QWidget* setupStatsWidget(); // Done
    QWidget* setupWildWidget(); // TBD
    QWidget* setupRaidsWidget(); // TBD
    QWidget* setupTrainersWidget(); // TBD
    QWidget* setupScenesWidget(); // TBD
    QWidget* setupShopWidget(); // TBD

    QWidget* setupPaldeaWildWidget();
    QWidget* setupKitakamiWildWidget();
    QWidget* setupBlueberryWildWidget();

    QWidget* setupPaldeaRaidWidget();
    QWidget* setupKitakamiRaidWidget();
    QWidget* setupBlueberryRaidWidget();

    QWidget* setupTrainersPaldeaWidget();
    QWidget* setupTrainersKitaWidget();
    QWidget* setupTrainersBerryWidget();

    void saveCheckboxState();
    void saveSpinBoxValue();
    void saveStringInput();
    void saveComboInput();
    void updateComboBoxForms(QComboBox *c, const QString &t);
    void updateComboBoxGender(QComboBox *c, const QString &t);
    void updatePreview(const QString &text);
    void showRestartWidget();
    bool checkAndDeleteFile(std::string filePath);
};

#endif // SVRANDOMIZERWINDOW_H
