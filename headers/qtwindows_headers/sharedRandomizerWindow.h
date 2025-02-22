#ifndef SHAREDRANDOMIZERWINDOW_H
#define SHAREDRANDOMIZERWINDOW_H

#include <QWidget>
#include <QMap>
#include <QVariant>
#include <QMessageBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QGroupBox>
#include <QStringListModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabBar>
#include <QLineEdit>
#include <QScrollArea>
#include <QFormLayout>
#include <QCompleter>
#include <QFileDialog>
#include <nlohmann/json.hpp>
#include "../sv_randomizer_headers/sv_shared_class.h"

class SharedRandomizerWindow: public SVShared{
protected:
    // Variables - uninitialized
    QMap<QString, QVariant> settings;
    QStringList starterNames;
    QMessageBox* progressBar = nullptr;

    // Protected Functions
    // virtual int obtainPokemonGenderRatios(QString pokemonStatsFilePath);
    // virtual QMap<QString, QVariant> setUpSettings(QMap<QString, QVariant> s);
    virtual QScrollArea* setupStartersWidget() = 0;
    virtual QScrollArea* setupPersonalWidget() = 0;
    virtual QScrollArea* setupWildWidget() = 0;
    virtual QScrollArea* setupTrainersWidget() = 0;
    virtual QScrollArea* setupBossWidget() = 0;
    // virtual QScrollArea* setupShopWidget();
    virtual void initializeSettings() =0;
    virtual void setupAllowedPokemon(QVBoxLayout *outerBox, allowedPokemonLimiter details)=0;

public:
    // Functions - Not Overloaded
    // virtual bool checkAndDeleteFiles(QString fileToCheck);
    // virtual void createFolderHierarchy(const QString& folder);
    // virtual QString getItemForPokemon(int pokemon, int form);
    // virtual QString getPokemonItemId(int index, int form);
    // virtual int getPokemonItemValue(int index, int form);

    // Functions Overloaded
    // virtual QList<int> getDevIdsOfAllowedPokemon(allowedPokemonLimiter limiterCheck);
    // virtual QList<int> getDevIdsOfAllowedPokemon(allowedPokemonLimiter limiterCheck, QList<int> &allowedLegends);

//     // TODO
    virtual QJsonDocument exportSettings(QMap<QString, QVariant> map) =0;
//     // void runRandomizer();
//     // void switchTabs(QStackedWidget* sw, int i =0);
//     // void sendMessageOfConfirmation(QString message);

    virtual void importSettings() =0;

private:
    // TODO


};


#endif // SHAREDRANDOMIZERWINDOW_H
