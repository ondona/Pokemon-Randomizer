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

protected:
    QScrollArea* setupStartersWidget() override;
    QVBoxLayout* createStartersWidget();
    QVBoxLayout* createGiftsWidget();
    QScrollArea* setupPersonalWidget() override;
    QVBoxLayout* createAbilitiesWidget();
    QVBoxLayout* createTypesWidget();
    QVBoxLayout* createMovesetWidget();
    QVBoxLayout* createBSTWidget();
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
    QVBoxLayout* createTrainerSettings(QString region, QString trainerType, QVector<bool> allowedSettings, allowedPokemonLimiter limiter, bool paldea = false);
    QScrollArea* setupBossWidget() override;
    QWidget* createPaldeaBossWidget();
    QWidget* createKitakamiBossWidget();
    QWidget* createBlueberryBossWidget();
    QVBoxLayout* createBossSettings(QString boss, allowedPokemonLimiter limiter, bool paldea = false);

private:
    SVRandomizerCode randomizer;
    unsigned int hash = 0;

    void createStarterPokemonSelection(QString starter, QStringList allowedPokemon,
                                               QStringList allowedPokeballs, QGroupBox* groupToAdd,
                                                QHBoxLayout* rowToAdd, QVBoxLayout* layoutToAdd, int index);
    void updateComboBoxGender(QComboBox *comboBox, QString text, int form);
    // redefinition of virtual functions - override is needed
    // to showcase that it is from the virtual function and not a new one
    void initializeSettings() override;
    void setupAllowedPokemon(QVBoxLayout *outerBox, allowedPokemonLimiter details) override;

public slots:
    QJsonDocument exportSettings(QMap<QString, QVariant> map) override;
    void switchTabs(QStackedWidget * stackedWidget, int i = 0);

signals:
    void importSettings() override;

};


#endif // SVRANDOMIZERWINDOW_H
