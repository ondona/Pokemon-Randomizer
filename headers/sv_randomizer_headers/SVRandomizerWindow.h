#ifndef SVRANDOMIZERWINDOW_H
#define SVRANDOMIZERWINDOW_H

#include "../qtwindows_headers/sharedRandomizerWindow.h"
#include "../sv_randomizer_headers/svrandomizercode.h"

class SVRandomizerWindow : public QWidget, SharedRandomizerWindow
{
    Q_OBJECT

public:
    explicit SVRandomizerWindow(QWidget *parent = nullptr);
    ~SVRandomizerWindow();
    void createLayout();
    QMap<QString, int> pokeballIndex = {
        {"Poke Ball", 0},
        {"Great Ball", 1},
        {"Ultra Ball", 2},
        {"Master Ball", 3},
        {"Beast Ball", 4},
        {"Cherish Ball", 5},
        {"Luxury Ball", 6},
        {"Timer Ball", 7},
        {"Net Ball", 8},
        {"Nest Ball", 9},
        {"Repeat Ball", 10},
        {"Dive Ball", 11},
        {"Dusk Ball", 12},
        {"Heal Ball", 13},
        {"Quick Ball", 14},
        {"Premier Ball", 15},
        {"Safari Ball", 16},
        {"Fast Ball", 17},
        {"Level Ball", 18},
        {"Lure Ball", 19},
        {"Heavy Ball", 20},
        {"Love Ball", 21},
        {"Friend Ball", 22},
        {"Moon Ball", 23},
        {"Sport Ball", 24},
        {"Dream Ball", 25}
    };


protected:
    QScrollArea* setupStartersWidget() override;
    QVBoxLayout* createStartersWidget();
    QVBoxLayout* createGiftsWidget();
    QScrollArea* setupPersonalWidget() override;
    QVBoxLayout* createAbilitiesWidget();
    QVBoxLayout* createTypesWidget();
    QVBoxLayout* createMovesetWidget();
    QVBoxLayout* createBSTWidget();
    QVBoxLayout* fixEvolutionsWidget();
    QVBoxLayout* createEvolutionsWidget();
    QVBoxLayout* createTMWidget();
    QVBoxLayout* createItemWidget();
    QScrollArea* setupWildWidget() override;
    QWidget* createPaldeaWildWidget();
    QWidget* createKitakamiWildWidget();
    QWidget* createBlueberryWildWidget();
    QWidget* createFixedEncountersWidget();
    QVBoxLayout* createPaldeaWild();
    QVBoxLayout* createKitakamiWild();
    QVBoxLayout* createBlueberryWild();
    QVBoxLayout* createFixedEncounters();
    QScrollArea* setupRaidsWidget(); // Raids Start Here
    QWidget* createPaldeaRaidWidget();
    QWidget* createKitakamiRaidWidget();
    QWidget* createBlueberryRaidWidget();
    QVBoxLayout* createPaldeaRaid();
    QVBoxLayout* createKitakamiRaid();
    QVBoxLayout* createBlueberryRaid();
    QScrollArea* setupTrainersWidget() override;
    QWidget* createPaldeaTrainersWidget();
    QWidget* createKitakamiTrainersWidget();
    QWidget* createBlueberryTrainersWidget();
    QVBoxLayout* createTrainerSettings(QString region, QString trainerType, trainerSettings& settings, bool paldea = false);
    QScrollArea* setupBossWidget() override;
    QWidget* createPaldeaBossWidget();
    QWidget* createKitakamiBossWidget();
    QWidget* createBlueberryBossWidget();
    QVBoxLayout* createBossSettings(QString boss, allowedPokemonLimiter limiter, bool paldea = false);
    SVRandomizerCode randomizer;
    void showMessage(const QString &message, unsigned int id);
private:
    unsigned int hash = 0;
    const unsigned int ENDRANDOMIZER = 151;
    void createStarterPokemonSelection(QString starter, QStringList allowedPokemon,
                                               QStringList allowedPokeballs, QGroupBox* groupToAdd,
                                                QHBoxLayout* rowToAdd, QVBoxLayout* layoutToAdd, int index);
    void updateComboBoxGender(QComboBox *comboBox, QString text, int form);
    // redefinition of virtual functions - override is needed
    // to showcase that it is from the virtual function and not a new one
    void initializeSettings() override;
    void setupAllowedPokemon(QVBoxLayout *outerBox, allowedPokemonLimiter& details) override;

public slots:
    QJsonDocument exportSettings(QMap<QString, QVariant> map) override;
    void switchTabs(QStackedWidget * stackedWidget, int i = 0);
    void runRandomizer() override;

signals:
    void importSettings() override;

};


#endif // SVRANDOMIZERWINDOW_H
