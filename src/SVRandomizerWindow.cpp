#include "headers/SVRandomizerWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QFormLayout>
#include <QCompleter>
#include <QGroupBox>
#include <ctime>
#include <QProcess>
#include <QCoreApplication>
#include <QApplication>
#include <iostream>
#include <random>
#include <QScrollArea>
#include <QSpinBox>
#include <QStringListModel>
#include <QComboBox>
#include <QDir>


SVRandomizerWindow::SVRandomizerWindow(QWidget *parent)
    : QWidget(parent)
{
    createLayout();  // This function creates the UI layout and adds widgets
}

SVRandomizerWindow::~SVRandomizerWindow()
{
    // Cleanup if needed
}

void SVRandomizerWindow::createLayout()
{
    // Create the main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ------------------------General Settings Section------------------------
        QGroupBox *generalGroup = new QGroupBox("General Settings Section");

        // Actual Definition of Questions
        // Add Single Checkbox section with multiple horizontal questions and varied inputs
        QVBoxLayout *generalSettings = new QVBoxLayout(generalGroup);

        // Horizontal layout for side-by-side questions and inputs in singleCheckboxGroup
        QHBoxLayout *mainSettings = new QHBoxLayout();
        QHBoxLayout *extraSettings = new QHBoxLayout();
        QHBoxLayout *seedSettings = new QHBoxLayout();
        QHBoxLayout *settings = new QHBoxLayout();

        extraSettings->setContentsMargins(0, 0, 0, 0);
        extraSettings->setSpacing(0);

        auto_patch = new QCheckBox("Auto Patch Randomizer ", generalGroup);
        auto_patch->setChecked(true);
        mainSettings->addWidget(auto_patch);
        connect(auto_patch, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        kaizo_mode = new QCheckBox("Kaizo Randomizer (Useless for now)", generalGroup);
        mainSettings->addWidget(kaizo_mode);
        connect(kaizo_mode, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        QLabel *bulk_question = new QLabel("Randomizers to Create ", generalGroup);
        bulk_question->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        bulk_amount= new QSpinBox(generalGroup);  // Number input
        bulk_amount->setRange(1, 100);  // Set range for the number input
        bulk_amount->setValue(1);
        connect(bulk_amount, QOverload<int>::of(&QSpinBox::valueChanged), this, &SVRandomizerWindow::saveSpinBoxValue);
        bulk_amount->setMaximumWidth(100);

        extraSettings->addWidget(bulk_question);
        extraSettings->addWidget(bulk_amount);

        // Add the horizontal layout to the group
        extraSettings->addStretch(1);  // Pushes widgets to the left

        importButton = new QPushButton("Import Settings and Seed", generalGroup);
        //connect(importButton, &QPushButton::clicked, this, &SVRandomizerWindow::addToFavorites);

        settings->addWidget(importButton);

        exportButton = new QPushButton("Export Settings and Seed", generalGroup);
        //connect(exportButton, &QPushButton::clicked, this, &SVRandomizerWindow::addToFavorites);

        settings->addWidget(exportButton);

        seed = new QLineEdit(generalGroup);
        connect(seed, &QLineEdit::textChanged, this, &SVRandomizerWindow::saveStringInput);

        QLabel *seedsets = new QLabel("Seed ", generalGroup);
        seedsets->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        seedSettings->addWidget(seedsets);

        seed->setFixedWidth(348);
        seed->setPlaceholderText("Leave Blank to use time... (no limit)");
        seedSettings->addWidget(seed);

        startRandomizer = new QPushButton("Start Randomizing", generalGroup);
        connect(startRandomizer, &QPushButton::clicked, this, &SVRandomizerWindow::addToFavorites);
        seedSettings->addWidget(startRandomizer);

        generalSettings->addLayout(mainSettings);
        generalSettings->addLayout(extraSettings);
        generalSettings->addLayout(settings);
        generalSettings->addLayout(seedSettings);

    mainLayout->addWidget(generalGroup);


    // Configure the tab bar
    topBar = new QTabBar(this);
    topBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    topBar->addTab("Starter/Gift Pokemon");
    topBar->addTab("Stats/TMs/Items");
    topBar->addTab("Wild/Static Pokemon");
    topBar->addTab("Raids");
    topBar->addTab("Trainers");
    //topBar->addTab("Bosses");
    //topBar->addTab("Shop");

    connect(topBar, &QTabBar::currentChanged, this, &SVRandomizerWindow::switchTabs);

    // Adjust the minimum width to a specific value to ensure it expands
    topBar->setMinimumWidth(800); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(topBar); // Add the tab bar to the layout

    // Configure the stacked widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(stackedWidget);

    // Add each widget to the stacked widget
    stackedWidget->addWidget(setupGiftWidget());
    stackedWidget->addWidget(setupStatsWidget());
    stackedWidget->addWidget(setupWildWidget());
    stackedWidget->addWidget(setupRaidsWidget());
    stackedWidget->addWidget(setupTrainersWidget());
    //stackedWidget->addWidget(setupScenesWidget());
    //stackedWidget->addWidget(setupShopWidget());

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // Apply the main layout to RandomizerWindow
    setLayout(mainLayout);
}

// Work on these for now
QWidget* SVRandomizerWindow::setupGiftWidget(){
    QWidget *giftWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(giftWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // ------------------------Settings for Starter ------------------------
    // Code for options that are considered "General"
    QGroupBox *startersGroupSettings= new QGroupBox("Starter Settings Section");
    QPushButton *toggleStartersGroupSettings= new QPushButton("Toggle Starter Settings Section", giftWidget);
    toggleStartersGroupSettings->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleStartersGroupSettings, &QPushButton::clicked, [startersGroupSettings]() {
        startersGroupSettings->setVisible(!startersGroupSettings->isVisible());
    });
    formLayout->addRow(toggleStartersGroupSettings);

    QVBoxLayout *startersSettingsLayout = new QVBoxLayout(startersGroupSettings);
    QHBoxLayout *enabled = new QHBoxLayout();
    QHBoxLayout *startersRow_Q1 = new QHBoxLayout();
    QHBoxLayout *startersRow_Q2 = new QHBoxLayout();
    QHBoxLayout *startersRow_Q3 = new QHBoxLayout();
    QHBoxLayout *bannedStages = new QHBoxLayout();
    QHBoxLayout *onlySelection = new QHBoxLayout();
    QHBoxLayout *miscSelection = new QHBoxLayout();
    QHBoxLayout *generation_StartersHeader = new QHBoxLayout();
    QHBoxLayout *generation_StartersSelection = new QHBoxLayout();

    for(int i = 0; i<3; i++)
        starters.append(new QLineEdit(startersGroupSettings));

    // Sample list of suggestions
    for(int i = 0; i<3; i++){
        QCompleter *completer = new QCompleter(randomizer.pokemonInGame, starters[i]);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setFilterMode(Qt::MatchContains);
        starters[i]->setCompleter(completer);
        connect(starters[i], &QLineEdit::textChanged, this, &SVRandomizerWindow::saveStringInput);
    }

    enable_starters = new QCheckBox("Randomize Starters ", startersGroupSettings);
    enabled->addWidget(enable_starters);
    connect(enable_starters, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    randomize_starters_tera_types = new QCheckBox("Randomize Tera Types ", startersGroupSettings);
    enabled->addWidget(randomize_starters_tera_types);
    connect(randomize_starters_tera_types, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    all_starters_shiny = new QCheckBox("Force All Starters Shiny ", startersGroupSettings);
    enabled->addWidget(all_starters_shiny);
    connect(all_starters_shiny, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    force_shiny_starter = new QCheckBox("Force One Starter Shiny ", startersGroupSettings);
    enabled->addWidget(force_shiny_starter);
    connect(force_shiny_starter, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    startersSettingsLayout->addLayout(enabled);

    // Set up Sprigattito - with form changes

    starters_shiny.append(new QCheckBox("Shiny ", startersGroupSettings));
    startersRow_Q1->addWidget(starters_shiny[0]);
    connect(starters_shiny[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    QLabel *sprigatito = new QLabel("Sprigatito ", startersGroupSettings);
    sprigatito->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q1->addWidget(sprigatito);

    starters[0]->setFixedWidth(175);
    starters[0]->setPlaceholderText("Leave Blank for Random...");
    startersRow_Q1->addWidget(starters[0]);


    QLabel *form_s1 = new QLabel("Form ", startersGroupSettings);
    form_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q1->addWidget(form_s1);

    starters_form.append(new QComboBox());
    starters_form[0]->addItem("---");
    starters_form[0]->setFixedSize(100, 25);

    startersRow_Q1->addWidget(starters_form[0]);

    connect(starters[0], &QLineEdit::textChanged, this, [=](const QString &text) {
        updateComboBoxForms(starters_form[0], text);
    });
    connect(starters_form[0], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    QLabel *gender_s1 = new QLabel("Gender ", startersGroupSettings);
    gender_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q1->addWidget(gender_s1);

    starters_gender.append(new QComboBox());
    starters_gender[0]->addItem("---");
    starters_gender[0]->setFixedSize(100, 25);

    startersRow_Q1->addWidget(starters_gender[0]);

    connect(starters[0], &QLineEdit::textChanged, this, [=](const QString &text) {
        updateComboBoxGender(starters_gender[0], text);
    });

    QLabel *pokeball_s1 = new QLabel("Pokeball ", startersGroupSettings);
    pokeball_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q1->addWidget(pokeball_s1);

    starters_pokeball.append(new QComboBox());
    starters_pokeball[0]->addItem("Poke Ball"); // Added
    starters_pokeball[0]->addItem("Great Ball"); // Added
    starters_pokeball[0]->addItem("Ultra Ball"); // Added
    starters_pokeball[0]->addItem("Master Ball"); // Added
    starters_pokeball[0]->addItem("Beast Ball"); // Added

    starters_pokeball[0]->addItem("Luxury Ball"); // Added
    starters_pokeball[0]->addItem("Timer Ball"); // Added
    starters_pokeball[0]->addItem("Net Ball"); // Added
    starters_pokeball[0]->addItem("Nest Ball"); // Added
    starters_pokeball[0]->addItem("Dive Ball"); // Added
    starters_pokeball[0]->addItem("Dusk Ball"); // Added
    starters_pokeball[0]->addItem("Repeat Ball"); // Added
    starters_pokeball[0]->addItem("Premier Ball"); //Added
    starters_pokeball[0]->addItem("Heal Ball"); //Added
    starters_pokeball[0]->addItem("Quick Ball"); // Added

    starters_pokeball[0]->addItem("Fast Ball"); //Added
    starters_pokeball[0]->addItem("Level Ball"); //Added
    starters_pokeball[0]->addItem("Lure Ball"); //Added
    starters_pokeball[0]->addItem("Heavy Ball"); //Added
    starters_pokeball[0]->addItem("Love Ball"); //Added
    starters_pokeball[0]->addItem("Friend Ball"); //Added
    starters_pokeball[0]->addItem("Moon Ball"); //Added
    starters_pokeball[0]->addItem("Sport Ball"); //Added
    starters_pokeball[0]->addItem("Safari Ball"); //Added
    starters_pokeball[0]->addItem("Dream Ball"); //Added
    starters_pokeball[0]->setFixedSize(100, 25);
    connect(starters_pokeball[0], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    startersRow_Q1->addWidget(starters_pokeball[0]);

    startersSettingsLayout->addLayout(startersRow_Q1);
    startersRow_Q1->addStretch(1);  // Pushes widgets to the left

    // Set up Fuecoco - with form changes
    starters_shiny.append(new QCheckBox("Shiny ", startersGroupSettings));
    startersRow_Q2->addWidget(starters_shiny[1]);
    connect(starters_shiny[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    QLabel *fuecoco = new QLabel("Fuecoco    ", startersGroupSettings);
    fuecoco->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q2->addWidget(fuecoco);

    starters[1]->setFixedWidth(175);
    starters[1]->setPlaceholderText("Leave Blank for Random...");
    startersRow_Q2->addWidget(starters[1]);

    QLabel *form_s2 = new QLabel("Form ", startersGroupSettings);
    form_s2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q2->addWidget(form_s2);

    starters_form.append(new QComboBox());
    starters_form[1]->addItem("---");
    starters_form[1]->setFixedSize(100, 25);

    startersRow_Q2->addWidget(starters_form[1]);

    connect(starters[1], &QLineEdit::textChanged, this, [=](const QString &text) {
        updateComboBoxForms(starters_form[1], text);
    });
    connect(starters_form[1], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    QLabel *gender_s2 = new QLabel("Gender ", startersGroupSettings);
    gender_s2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q2->addWidget(gender_s2);

    starters_gender.append(new QComboBox());
    starters_gender[1]->addItem("---");
    starters_gender[1]->setFixedSize(100, 25);

    startersRow_Q2->addWidget(starters_gender[1]);

    connect(starters[1], &QLineEdit::textChanged, this, [=](const QString &text) {
        updateComboBoxGender(starters_gender[1], text);
    });

    QLabel *pokeball_s2 = new QLabel("Pokeball ", startersGroupSettings);
    pokeball_s2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    startersRow_Q2->addWidget(pokeball_s2);
    starters_pokeball.append(new QComboBox());
    starters_pokeball[1]->addItem("Poke Ball"); // Added
    starters_pokeball[1]->addItem("Great Ball"); // Added
    starters_pokeball[1]->addItem("Ultra Ball"); // Added
    starters_pokeball[1]->addItem("Master Ball"); // Added
    starters_pokeball[1]->addItem("Beast Ball"); // Added

    starters_pokeball[1]->addItem("Luxury Ball"); // Added
    starters_pokeball[1]->addItem("Timer Ball"); // Added
    starters_pokeball[1]->addItem("Net Ball"); // Added
    starters_pokeball[1]->addItem("Nest Ball"); // Added
    starters_pokeball[1]->addItem("Dive Ball"); // Added
    starters_pokeball[1]->addItem("Dusk Ball"); // Added
    starters_pokeball[1]->addItem("Repeat Ball"); // Added
    starters_pokeball[1]->addItem("Premier Ball"); //Added
    starters_pokeball[1]->addItem("Heal Ball"); //Added
    starters_pokeball[1]->addItem("Quick Ball"); // Added

    starters_pokeball[1]->addItem("Fast Ball"); //Added
    starters_pokeball[1]->addItem("Level Ball"); //Added
    starters_pokeball[1]->addItem("Lure Ball"); //Added
    starters_pokeball[1]->addItem("Heavy Ball"); //Added
    starters_pokeball[1]->addItem("Love Ball"); //Added
    starters_pokeball[1]->addItem("Friend Ball"); //Added
    starters_pokeball[1]->addItem("Moon Ball"); //Added
    starters_pokeball[1]->addItem("Sport Ball"); //Added
    starters_pokeball[1]->addItem("Safari Ball"); //Added
    starters_pokeball[1]->addItem("Dream Ball"); //Added
    starters_pokeball[1]->setFixedSize(100, 25);
    connect(starters_pokeball[1], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    startersRow_Q2->addWidget(starters_pokeball[1]);

    startersSettingsLayout->addLayout(startersRow_Q2);
    startersRow_Q2->addStretch(1);  // Pushes widgets to the left

    // Set up Quaxly - with form changes
    starters_shiny.append(new QCheckBox("Shiny ", startersGroupSettings));
    startersRow_Q3->addWidget(starters_shiny[2]);
    connect(starters_shiny[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    QLabel *quaxly = new QLabel("Quaxly \t ", startersGroupSettings);
    quaxly->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q3->addWidget(quaxly);

    starters[2]->setFixedWidth(175);
    starters[2]->setPlaceholderText("Leave Blank for Random...");
    startersRow_Q3->addWidget(starters[2]);

    QLabel *form_s3 = new QLabel("Form ", startersGroupSettings);
    form_s3->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q3->addWidget(form_s3);

    starters_form.append(new QComboBox());
    starters_form[2]->addItem("---");
    starters_form[2]->setFixedSize(100, 25);

    startersRow_Q3->addWidget(starters_form[2]);

    connect(starters[2], &QLineEdit::textChanged, this, [=](const QString &text) {
        updateComboBoxForms(starters_form[2], text);
    });
    connect(starters_form[2], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    QLabel *gender_s3 = new QLabel("Gender ", startersGroupSettings);
    gender_s3->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q3->addWidget(gender_s3);

    starters_gender.append(new QComboBox());
    starters_gender[2]->addItem("---");
    starters_gender[2]->setFixedSize(100, 25);

    startersRow_Q3->addWidget(starters_gender[2]);

    connect(starters[2], &QLineEdit::textChanged, this, [=](const QString &text) {
        updateComboBoxGender(starters_gender[2], text);
    });

    QLabel *pokeball_s3 = new QLabel("Pokeball ", startersGroupSettings);
    pokeball_s3->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    startersRow_Q3->addWidget(pokeball_s3);
    starters_pokeball.append(new QComboBox());
    starters_pokeball[2]->addItem("Poke Ball"); // Added
    starters_pokeball[2]->addItem("Great Ball"); // Added
    starters_pokeball[2]->addItem("Ultra Ball"); // Added
    starters_pokeball[2]->addItem("Master Ball"); // Added
    starters_pokeball[2]->addItem("Beast Ball"); // Added

    starters_pokeball[2]->addItem("Luxury Ball"); // Added
    starters_pokeball[2]->addItem("Timer Ball"); // Added
    starters_pokeball[2]->addItem("Net Ball"); // Added
    starters_pokeball[2]->addItem("Nest Ball"); // Added
    starters_pokeball[2]->addItem("Dive Ball"); // Added
    starters_pokeball[2]->addItem("Dusk Ball"); // Added
    starters_pokeball[2]->addItem("Repeat Ball"); // Added
    starters_pokeball[2]->addItem("Premier Ball"); //Added
    starters_pokeball[2]->addItem("Heal Ball"); //Added
    starters_pokeball[2]->addItem("Quick Ball"); // Added

    starters_pokeball[2]->addItem("Fast Ball"); //Added
    starters_pokeball[2]->addItem("Level Ball"); //Added
    starters_pokeball[2]->addItem("Lure Ball"); //Added
    starters_pokeball[2]->addItem("Heavy Ball"); //Added
    starters_pokeball[2]->addItem("Love Ball"); //Added
    starters_pokeball[2]->addItem("Friend Ball"); //Added
    starters_pokeball[2]->addItem("Moon Ball"); //Added
    starters_pokeball[2]->addItem("Sport Ball"); //Added
    starters_pokeball[2]->addItem("Safari Ball"); //Added
    starters_pokeball[2]->addItem("Dream Ball"); //Added
    starters_pokeball[2]->setFixedSize(100, 25);
    connect(starters_pokeball[2], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    startersRow_Q3->addWidget(starters_pokeball[2]);

    startersSettingsLayout->addLayout(startersRow_Q3);
    startersRow_Q3->addStretch(1);  // Pushes widgets to the left
    // Banned Stages
    ban_stage_1_pokemon_starters = new QCheckBox("Ban Stage 1 Pokemon ", startersGroupSettings);
    bannedStages->addWidget(ban_stage_1_pokemon_starters);
    connect(ban_stage_1_pokemon_starters, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    ban_stage_2_pokemon_starters = new QCheckBox("Ban Stage 2 Pokemon ", startersGroupSettings);
    bannedStages->addWidget(ban_stage_2_pokemon_starters);
    connect(ban_stage_2_pokemon_starters, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    ban_stage_3_pokemon_starters = new QCheckBox("Ban Stage 3 Pokemon ", startersGroupSettings);
    bannedStages->addWidget(ban_stage_3_pokemon_starters);
    connect(ban_stage_3_pokemon_starters, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    ban_single_stage_pokemon_starters = new QCheckBox("Ban 1 Stage Pokemon ", startersGroupSettings);
    bannedStages->addWidget(ban_single_stage_pokemon_starters);
    connect(ban_single_stage_pokemon_starters, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    startersSettingsLayout->addLayout(bannedStages);

    // Only Settings
    only_legendary_pokemon_starters = new QCheckBox("Only Legends ", startersGroupSettings);
    onlySelection->addWidget(only_legendary_pokemon_starters);
    connect(only_legendary_pokemon_starters, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    only_paradox_pokemon_starters = new QCheckBox("Only Paradox ", startersGroupSettings);
    onlySelection->addWidget(only_paradox_pokemon_starters);
    connect(only_paradox_pokemon_starters, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    only_legends_and_paradox_starters = new QCheckBox("Only Legends and Paradox ", startersGroupSettings);
    onlySelection->addWidget(only_legends_and_paradox_starters);
    connect(only_legends_and_paradox_starters, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    startersSettingsLayout->addLayout(onlySelection);

    // Misc Settings
    show_starters_in_overworld = new QCheckBox("Show New Starters in Overworld ", startersGroupSettings);
    show_starters_in_overworld->setChecked(true);
    miscSelection->addWidget(show_starters_in_overworld);
    connect(show_starters_in_overworld, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    QLabel *shiny_rate_Starters = new QLabel("Starters Shiny Rate ", startersGroupSettings);
    shiny_rate_Starters->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    shiny_starter_rate= new QSpinBox(startersGroupSettings);  // Number input
    shiny_starter_rate->setRange(1, 100);  // Set range for the number input
    shiny_starter_rate->setValue(10);
    connect(shiny_starter_rate, QOverload<int>::of(&QSpinBox::valueChanged), this, &SVRandomizerWindow::saveSpinBoxValue);
    shiny_starter_rate->setMaximumWidth(100);

    miscSelection->addWidget(shiny_rate_Starters);
    miscSelection->addWidget(shiny_starter_rate);

    // Add the horizontal layout to the group
    miscSelection->addStretch(1);  // Pushes widgets to the left

    startersSettingsLayout->addLayout(miscSelection);

    // Starters Generation
    QLabel *startersSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", startersGroupSettings);
    startersSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_StartersHeader->addWidget(startersSectionHeader);

    for(int i =0; i<9; i++){
        generations_starters.append(new QCheckBox(QStringLiteral("%1").arg(i+1), startersGroupSettings));
        generation_StartersSelection->addWidget(generations_starters[i]);
        connect(generations_starters[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    startersSettingsLayout->addLayout(generation_StartersHeader);
    startersSettingsLayout->addLayout(generation_StartersSelection);

    // if not added it will open a new window
    formLayout->addRow(startersGroupSettings);

    // ------------------------Gift Settings Section------------------------
    QGroupBox *giftPokemonSection = new QGroupBox("Gifts Settings Section");
    QPushButton *toggleGiftGroup = new QPushButton("Toggle Gifts Settings Section", giftWidget);
    toggleGiftGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleGiftGroup, &QPushButton::clicked, [giftPokemonSection]() {
        giftPokemonSection->setVisible(!giftPokemonSection->isVisible());
    });

    formLayout->addRow(toggleGiftGroup);
    // Code for Gift Settings Section
    QVBoxLayout *giftsSettingsLayout = new QVBoxLayout(giftPokemonSection);
    QHBoxLayout *gifts_general = new QHBoxLayout();
    QHBoxLayout *generation_GiftsHeader = new QHBoxLayout();
    QHBoxLayout *generation_GiftsSelection = new QHBoxLayout();

    enable_gifts = new QCheckBox("Randomize Gift Pokemon ", giftPokemonSection);
    gifts_general->addWidget(enable_gifts);
    connect(enable_gifts, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    randomize_tera_types = new QCheckBox("Randomize Tera Types ", giftPokemonSection);
    gifts_general->addWidget(randomize_tera_types);
    connect(randomize_tera_types, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    QLabel *shiny_rate_gifts = new QLabel("Statics Shiny Rate ", giftPokemonSection);
    shiny_rate_gifts->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    shiny_static_rate= new QSpinBox(giftPokemonSection);  // Number input
    shiny_static_rate->setRange(1, 100);  // Set range for the number input
    shiny_static_rate->setValue(10);
    connect(shiny_static_rate, QOverload<int>::of(&QSpinBox::valueChanged), this, &SVRandomizerWindow::saveSpinBoxValue);
    shiny_static_rate->setMaximumWidth(100);

    gifts_general->addWidget(shiny_rate_gifts);
    gifts_general->addWidget(shiny_static_rate);

    // Add the horizontal layout to the group
    gifts_general->addStretch(1);  // Pushes widgets to the left

    giftsSettingsLayout->addLayout(gifts_general);

    // Starters Generation
    QLabel *giftsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", giftPokemonSection);
    giftsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_GiftsHeader->addWidget(giftsSectionHeader);

    for(int i =0; i<9; i++){
        generation_gifts.append(new QCheckBox(QStringLiteral("%1").arg(i+1), giftPokemonSection));
        generation_GiftsSelection->addWidget(generation_gifts[i]);
        connect(generation_gifts[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    giftsSettingsLayout->addLayout(generation_GiftsHeader);
    giftsSettingsLayout->addLayout(generation_GiftsSelection);

    // if not added it will open a new window
    //Last thing to add
    formLayout->addRow(giftPokemonSection);

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(giftWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupStatsWidget(){
    QWidget *statsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(statsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //------------------------Stats Settings Sections------------------------
    QGroupBox *statsSettingsGroup = new QGroupBox("Stats Settings Section");
    QPushButton *togglestatsSettingsGroup = new QPushButton("Toggle Stats Settings Section", statsWidget);
    togglestatsSettingsGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(togglestatsSettingsGroup, &QPushButton::clicked, [statsSettingsGroup]() {
        statsSettingsGroup->setVisible(!statsSettingsGroup->isVisible());
    });

    QVBoxLayout *statsSettingsLayout = new QVBoxLayout(statsSettingsGroup);
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();
    QHBoxLayout *row3 = new QHBoxLayout();
    QHBoxLayout *row4 = new QHBoxLayout();
    QHBoxLayout *row_tms = new QHBoxLayout();
    // QHBoxLayout *generation_stats_header = new QHBoxLayout();
    // QHBoxLayout *generation_stats_selection = new QHBoxLayout();

    formLayout->addRow(togglestatsSettingsGroup); // if not added the button will open an alternate window

    randomize_abilities = new QCheckBox("Randomize Abilities ", statsSettingsGroup);
    row1->addWidget(randomize_abilities);
    connect(randomize_abilities, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    ban_wonder_guard = new QCheckBox("Ban Wonder Guard", statsSettingsGroup);
    row1->addWidget(ban_wonder_guard);
    connect(ban_wonder_guard, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    randomize_types = new QCheckBox("Randomize Types ", statsSettingsGroup);
    row1->addWidget(randomize_types);
    connect(randomize_types, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    give_extra_types = new QCheckBox("Grant Extra Types", statsSettingsGroup);
    row1->addWidget(give_extra_types);
    connect(give_extra_types, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    statsSettingsLayout->addLayout(row1);

    randomize_moveset = new QCheckBox("Randomize Moveset", statsSettingsGroup);
    row2->addWidget(randomize_moveset);
    connect(randomize_moveset, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    // moveset_same_as_type = new QCheckBox("Moveset Same as Type", statsSettingsGroup);
    // row2->addWidget(moveset_same_as_type);
    // connect(moveset_same_as_type, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    force_tera_blast_every_5_levels = new QCheckBox("Force Tera Blast Every 5 levels", statsSettingsGroup);
    row2->addWidget(force_tera_blast_every_5_levels);
    connect(force_tera_blast_every_5_levels, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    statsSettingsLayout->addLayout(row2);

    randomize_bst = new QCheckBox("Randomize BST", statsSettingsGroup);
    row3->addWidget(randomize_bst);
    connect(randomize_bst, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    keep_same_bst = new QCheckBox("Keep Same BST", statsSettingsGroup);
    row3->addWidget(keep_same_bst);
    connect(keep_same_bst, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    statsSettingsLayout->addLayout(row3);

    randomize_evolutions = new QCheckBox("Randomize Evolutions (This will take 1-2 minutes)", statsSettingsGroup);
    row4->addWidget(randomize_evolutions);
    connect(randomize_evolutions, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    evolve_every_5_levels = new QCheckBox("Evolve Every Level", statsSettingsGroup);
    row4->addWidget(evolve_every_5_levels);
    connect(evolve_every_5_levels, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    force_unobtainable_evolutions_at_night = new QCheckBox("Regional Evolutions At Night", statsSettingsGroup);
    row4->addWidget(force_unobtainable_evolutions_at_night);
    connect(force_unobtainable_evolutions_at_night, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    statsSettingsLayout->addLayout(row4);
        //------------------------TMs Settings Sections------------------------
        QGroupBox *tm_group = new QGroupBox("TMs Section", statsSettingsGroup);
        enable_TMs = new QCheckBox("Randomize TMs", statsSettingsGroup);
        tm_group->setVisible(false);
        connect(enable_TMs, &QCheckBox::toggled, tm_group, &QGroupBox::setVisible);
        connect(enable_TMs, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        row_tms->addWidget(enable_TMs);
        statsSettingsLayout->addLayout(row_tms);

        QVBoxLayout *tms_layout = new QVBoxLayout(tm_group);
        QHBoxLayout *inner_tms = new QHBoxLayout();

        // randomize_tms = new QCheckBox("Randomize TMs ", tm_group);
        // inner_tms->addWidget(randomize_tms);
        // connect(randomize_tms, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        // randomize_pokemon_tms = new QCheckBox("Randomize TMs Pokemon Uses (Leave Unchecked if you want random TMs and not just randomize the allow) ", tm_group);
        // inner_tms->addWidget(randomize_pokemon_tms);
        // connect(randomize_pokemon_tms, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        allow_moves_without_animation = new QCheckBox("Allow Moves Without Animation ", tm_group);
        inner_tms->addWidget(allow_moves_without_animation);
        connect(allow_moves_without_animation, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        tms_layout->addLayout(inner_tms);

        statsSettingsLayout->addWidget(tm_group); // if not added the button will open an alternate window

    // QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", statsSettingsGroup);
    // startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    // generation_stats_header->addWidget(startsSectionHeader);

    // for(int i =0; i<9; i++){
    //     generation_stats.append(new QCheckBox(QStringLiteral("%1").arg(i+1), statsSettingsGroup));
    //     generation_stats_selection->addWidget(generation_stats[i]);
    //     connect(generation_stats[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    // }
    // statsSettingsLayout->addLayout(generation_stats_header);
    // statsSettingsLayout->addLayout(generation_stats_selection);

    formLayout->addRow(statsSettingsGroup); // if not added the button will open an alternate window
    //------------------------Items Settings Sections------------------------
    QGroupBox *itemsSettingsGroup = new QGroupBox("Items Settings Section");
    QPushButton *toggleitemsSettingsGroup = new QPushButton("Toggle Items Settings Section", statsWidget);
    toggleitemsSettingsGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleitemsSettingsGroup, &QPushButton::clicked, [itemsSettingsGroup]() {
        itemsSettingsGroup->setVisible(!itemsSettingsGroup->isVisible());
    });

    formLayout->addRow(toggleitemsSettingsGroup);

    QVBoxLayout *itemsSettingsLayout = new QVBoxLayout(itemsSettingsGroup);
    QHBoxLayout *row1Items = new QHBoxLayout();
    QHBoxLayout *row2Items = new QHBoxLayout();

    randomize_hidden_items = new QCheckBox("Randomize Hidden Items", statsSettingsGroup);
    row1Items->addWidget(randomize_hidden_items);
    connect(randomize_hidden_items, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    randomize_pickup_items = new QCheckBox("Randomize PickUp Items", statsSettingsGroup);
    row1Items->addWidget(randomize_pickup_items);
    connect(randomize_pickup_items, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    itemsSettingsLayout->addLayout(row1Items);

    randomize_pokemon_drops = new QCheckBox("Randomize Pokemon Drops", statsSettingsGroup);
    row2Items->addWidget(randomize_pokemon_drops);
    connect(randomize_pokemon_drops, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    randomize_synchro_items = new QCheckBox("Randomize Synchro Items", statsSettingsGroup);
    row2Items->addWidget(randomize_synchro_items);
    connect(randomize_synchro_items, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    itemsSettingsLayout->addLayout(row2Items);

    formLayout->addRow(itemsSettingsGroup); // if not added the button will open an alternate window
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(statsWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupWildWidget(){
    QWidget *wildWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(wildWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Configure the tab bar
    innerWildBar = new QTabBar(this);
    innerWildBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    innerWildBar->addTab("Paldea Wilds");
    innerWildBar->addTab("Kitakami Wilds");
    innerWildBar->addTab("Blueberry Academy Wilds");

    connect(innerWildBar, &QTabBar::currentChanged, this, &SVRandomizerWindow::switchInnerWildTabs);

    // Adjust the minimum width to a specific value to ensure it expands
    innerWildBar->setMinimumWidth(750); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(innerWildBar); // Add the tab bar to the layout

    // Configure the stacked widget
    innerWildStackWidget = new QStackedWidget(this);
    innerWildStackWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(innerWildStackWidget);

    // Add each widget to the stacked widget
    innerWildStackWidget->addWidget(setupPaldeaWildWidget());
    innerWildStackWidget->addWidget(setupKitakamiWildWidget());
    innerWildStackWidget->addWidget(setupBlueberryWildWidget());

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(wildWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupRaidsWidget(){
    QWidget *raidsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(raidsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);


    // Configure the tab bar
    innerRaidBar = new QTabBar(this);
    innerRaidBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    innerRaidBar->addTab("Paldea Raids");
    innerRaidBar->addTab("Kitakami Raids");
    innerRaidBar->addTab("Blueberry Academy Raids");


    connect(innerRaidBar, &QTabBar::currentChanged, this, &SVRandomizerWindow::switchInnerRaidTabs);

    // Adjust the minimum width to a specific value to ensure it expands
    innerRaidBar->setMinimumWidth(750); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(innerRaidBar); // Add the tab bar to the layout

    // Configure the stacked widget
    innerRaidStackWidget = new QStackedWidget(this);
    innerRaidStackWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(innerRaidStackWidget);

    // Add each widget to the stacked widget
    innerRaidStackWidget->addWidget(setupPaldeaRaidWidget());
    innerRaidStackWidget->addWidget(setupKitakamiRaidWidget());
    innerRaidStackWidget->addWidget(setupBlueberryRaidWidget());

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(raidsWidget);
    return scrollArea;

}

QWidget* SVRandomizerWindow::setupTrainersWidget(){
    QWidget *trainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(trainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);


    // Configure the tab bar
    innerTrainerBar = new QTabBar(this);
    innerTrainerBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    innerTrainerBar->addTab("Paldea Trainers");
    innerTrainerBar->addTab("Kitakami Trainers");
    innerTrainerBar->addTab("Blueberry Academy Trainers");


    connect(innerTrainerBar, &QTabBar::currentChanged, this, &SVRandomizerWindow::switchInnerTrainerTabs);

    // Adjust the minimum width to a specific value to ensure it expands
    innerTrainerBar->setMinimumWidth(750); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(innerTrainerBar); // Add the tab bar to the layout

    // Configure the stacked widget
    innerTrainerStackWidget = new QStackedWidget(this);
    innerTrainerStackWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(innerTrainerStackWidget);

    // Add each widget to the stacked widget
    innerTrainerStackWidget->addWidget(setupTrainersPaldeaWidget());
    innerTrainerStackWidget->addWidget(setupTrainersKitaWidget());
    innerTrainerStackWidget->addWidget(setupTrainersBerryWidget());

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(trainersWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupScenesWidget(){
    QWidget *bossWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(bossWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();
    QHBoxLayout *generation_boss_header = new QHBoxLayout();
    QHBoxLayout *generation_boss_selection = new QHBoxLayout();

    randomize_bosses =new QCheckBox("Randomize Bosses", bossWidget);
    row0->addWidget(randomize_bosses);
    connect(randomize_bosses, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row0);

    // -------------- New Row --------------

    boss_settings.append(new QCheckBox("Exclude Legends", bossWidget));
    row1->addWidget(boss_settings[0]);
    connect(boss_settings[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    boss_settings.append(new QCheckBox("Only Legends", bossWidget));
    row1->addWidget(boss_settings[1]);
    connect(boss_settings[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    boss_settings.append(new QCheckBox("Only Paradox", bossWidget));
    row1->addWidget(boss_settings[2]);
    connect(boss_settings[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    boss_settings.append(new QCheckBox("Only Legends and Paradox", bossWidget));
    row1->addWidget(boss_settings[3]);
    connect(boss_settings[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row1);

    // -------------- New Row --------------

    boss_settings.append(new QCheckBox("Ban Stage 1", bossWidget));
    row2->addWidget(boss_settings[4]);
    connect(boss_settings[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    boss_settings.append(new QCheckBox("Ban Stage 2", bossWidget));
    row2->addWidget(boss_settings[5]);
    connect(boss_settings[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    boss_settings.append(new QCheckBox("Ban Stage 3", bossWidget));
    row2->addWidget(boss_settings[6]);
    connect(boss_settings[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    boss_settings.append(new QCheckBox("Ban 1 Stage", bossWidget));
    row2->addWidget(boss_settings[7]);
    connect(boss_settings[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row2);


    QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", bossWidget);
    startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_boss_header->addWidget(startsSectionHeader);

    for(int i =0; i<9; i++){
        boss_generation.append(new QCheckBox(QStringLiteral("%1").arg(i+1), bossWidget));
        generation_boss_selection->addWidget(boss_generation[i]);
        connect(boss_generation[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    formLayout->addRow(generation_boss_header);
    formLayout->addRow(generation_boss_selection);

    QLabel *comingsoon = new QLabel("More Options Coming Soon", bossWidget);
    comingsoon->setStyleSheet("font-weight: bold; padding: 0px 0;");

    formLayout->addRow(comingsoon);
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(bossWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupShopWidget(){
    QWidget *shopWidget = new QWidget();

    // My Code Here

    return shopWidget;
}

QWidget* SVRandomizerWindow::setupPaldeaWildWidget(){
    QWidget *paldeaWildWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(paldeaWildWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //--------------Wild Settings Start--------------
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();
    QHBoxLayout *row3 = new QHBoxLayout();
    QHBoxLayout *generation_wild_header = new QHBoxLayout();
    QHBoxLayout *generation_wild_selection = new QHBoxLayout();
    QHBoxLayout *row0 = new QHBoxLayout();

    randomize_paldea_wild = new QCheckBox("Randomize Wilds", paldeaWildWidget);
    row0->addWidget(randomize_paldea_wild);
    connect(randomize_paldea_wild, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row0);

    paldea_ExcludeLegends = new QCheckBox("Exclude Legends", paldeaWildWidget);
    row1->addWidget(paldea_ExcludeLegends);
    connect(paldea_ExcludeLegends, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    paldea_OnlyLegends = new QCheckBox("Only Legends", paldeaWildWidget);
    row1->addWidget(paldea_OnlyLegends);
    connect(paldea_OnlyLegends, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    paldea_OnlyParadox = new QCheckBox("Only Paradox", paldeaWildWidget);
    row1->addWidget(paldea_OnlyParadox);
    connect(paldea_OnlyParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    paldea_OnlyLegendsAndParadox = new QCheckBox("Only Legends and Paradox", paldeaWildWidget);
    row1->addWidget(paldea_OnlyLegendsAndParadox);
    connect(paldea_OnlyLegendsAndParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row1);

    // -------------- New Row --------------

    paldea_BanStage1 = new QCheckBox("Ban Stage 1", paldeaWildWidget);
    row2->addWidget(paldea_BanStage1);
    connect(paldea_BanStage1, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    paldea_BanStage2 = new QCheckBox("Ban Stage 2", paldeaWildWidget);
    row2->addWidget(paldea_BanStage2);
    connect(paldea_BanStage2, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    paldea_BanStage3 = new QCheckBox("Ban Stage 3", paldeaWildWidget);
    row2->addWidget(paldea_BanStage3);
    connect(paldea_BanStage3, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    paldea_Ban1Stage = new QCheckBox("Ban 1 Stage", paldeaWildWidget);
    row2->addWidget(paldea_Ban1Stage);
    connect(paldea_Ban1Stage, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row2);

    // -------------- New Row --------------
    paldea_Settings_for_all_wild = new QCheckBox("Use Paldea Settings for all other wilds", paldeaWildWidget);
    row3->addWidget(paldea_Settings_for_all_wild);
    connect(paldea_Settings_for_all_wild, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    paldea_BalanceAreaPerBST = new QCheckBox("Balance Area per BST (useless for now)", paldeaWildWidget);
    row3->addWidget(paldea_BalanceAreaPerBST);
    connect(paldea_BalanceAreaPerBST, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row3);

    // -------------- New Row --------------
    QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", paldeaWildWidget);
    startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_wild_header->addWidget(startsSectionHeader);

    for(int i =0; i<9; i++){
        generation_paldea_wild.append(new QCheckBox(QStringLiteral("%1").arg(i+1), paldeaWildWidget));
        generation_wild_selection->addWidget(generation_paldea_wild[i]);
        connect(generation_paldea_wild[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    formLayout->addRow(generation_wild_header);
    formLayout->addRow(generation_wild_selection);

    //--------------Wild Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(paldeaWildWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupKitakamiWildWidget(){
    QWidget *kitakamiWildWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(kitakamiWildWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //--------------Wild Settings Start--------------
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();
    QHBoxLayout *row3 = new QHBoxLayout();
    QHBoxLayout *generation_wild_header = new QHBoxLayout();
    QHBoxLayout *generation_wild_selection = new QHBoxLayout();
    QHBoxLayout *row0 = new QHBoxLayout();

    randomize_kitakami_wild = new QCheckBox("Randomize Wilds", kitakamiWildWidget);
    row0->addWidget(randomize_kitakami_wild);
    connect(randomize_kitakami_wild, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row0);

    kitakami_ExcludeLegends = new QCheckBox("Exclude Legends", kitakamiWildWidget);
    row1->addWidget(kitakami_ExcludeLegends);
    connect(kitakami_ExcludeLegends, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kitakami_OnlyLegends = new QCheckBox("Only Legends", kitakamiWildWidget);
    row1->addWidget(kitakami_OnlyLegends);
    connect(kitakami_OnlyLegends, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kitakami_OnlyParadox = new QCheckBox("Only Paradox", kitakamiWildWidget);
    row1->addWidget(kitakami_OnlyParadox);
    connect(kitakami_OnlyParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kitakami_OnlyLegendsAndParadox = new QCheckBox("Only Legends and Paradox", kitakamiWildWidget);
    row1->addWidget(kitakami_OnlyLegendsAndParadox);
    connect(kitakami_OnlyLegendsAndParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row1);

    // -------------- New Row --------------

    kitakami_BanStage1 = new QCheckBox("Ban Stage 1", kitakamiWildWidget);
    row2->addWidget(kitakami_BanStage1);
    connect(kitakami_BanStage1, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kitakami_BanStage2 = new QCheckBox("Ban Stage 2", kitakamiWildWidget);
    row2->addWidget(kitakami_BanStage2);
    connect(kitakami_BanStage2, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kitakami_BanStage3 = new QCheckBox("Ban Stage 3", kitakamiWildWidget);
    row2->addWidget(kitakami_BanStage3);
    connect(kitakami_BanStage3, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kitakami_Ban1Stage = new QCheckBox("Ban 1 Stage", kitakamiWildWidget);
    row2->addWidget(kitakami_Ban1Stage);
    connect(kitakami_Ban1Stage, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row2);

    // -------------- New Row --------------

    kitakami_BalanceAreaPerBST = new QCheckBox("Balance Area per BST (useless for now)", kitakamiWildWidget);
    row3->addWidget(kitakami_BalanceAreaPerBST);
    connect(kitakami_BalanceAreaPerBST, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row3);

    // -------------- New Row --------------
    QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", kitakamiWildWidget);
    startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_wild_header->addWidget(startsSectionHeader);

    for(int i =0; i<9; i++){
        generation_kitakami_wild.append(new QCheckBox(QStringLiteral("%1").arg(i+1), kitakamiWildWidget));
        generation_wild_selection->addWidget(generation_kitakami_wild[i]);
        connect(generation_kitakami_wild[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    formLayout->addRow(generation_wild_header);
    formLayout->addRow(generation_wild_selection);

    //--------------Wild Settings End--------------

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(kitakamiWildWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupBlueberryWildWidget(){
    QWidget *blueberryWildWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(blueberryWildWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //--------------Wild Settings Start--------------
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();
    QHBoxLayout *row3 = new QHBoxLayout();
    QHBoxLayout *generation_wild_header = new QHBoxLayout();
    QHBoxLayout *generation_wild_selection = new QHBoxLayout();
    QHBoxLayout *row0 = new QHBoxLayout();

    randomize_blueberry_wild = new QCheckBox("Randomize Wilds", blueberryWildWidget);
    row0->addWidget(randomize_blueberry_wild);
    connect(randomize_blueberry_wild, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row0);

    blueberry_ExcludeLegends = new QCheckBox("Exclude Legends", blueberryWildWidget);
    row1->addWidget(blueberry_ExcludeLegends);
    connect(blueberry_ExcludeLegends, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    blueberry_OnlyLegends = new QCheckBox("Only Legends", blueberryWildWidget);
    row1->addWidget(blueberry_OnlyLegends);
    connect(blueberry_OnlyLegends, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    blueberry_OnlyParadox = new QCheckBox("Only Paradox", blueberryWildWidget);
    row1->addWidget(blueberry_OnlyParadox);
    connect(blueberry_OnlyParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    blueberry_OnlyLegendsAndParadox = new QCheckBox("Only Legends and Paradox", blueberryWildWidget);
    row1->addWidget(blueberry_OnlyLegendsAndParadox);
    connect(blueberry_OnlyLegendsAndParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row1);

    // -------------- New Row --------------

    blueberry_BanStage1 = new QCheckBox("Ban Stage 1", blueberryWildWidget);
    row2->addWidget(blueberry_BanStage1);
    connect(blueberry_BanStage1, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    blueberry_BanStage2 = new QCheckBox("Ban Stage 2", blueberryWildWidget);
    row2->addWidget(blueberry_BanStage2);
    connect(blueberry_BanStage2, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    blueberry_BanStage3 = new QCheckBox("Ban Stage 3", blueberryWildWidget);
    row2->addWidget(blueberry_BanStage3);
    connect(blueberry_BanStage3, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    blueberry_Ban1Stage = new QCheckBox("Ban 1 Stage", blueberryWildWidget);
    row2->addWidget(blueberry_Ban1Stage);
    connect(blueberry_Ban1Stage, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row2);

    // -------------- New Row --------------

    blueberry_BalanceAreaPerBST = new QCheckBox("Balance Area per BST (useless for now)", blueberryWildWidget);
    row3->addWidget(blueberry_BalanceAreaPerBST);
    connect(blueberry_BalanceAreaPerBST, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row3);

    // -------------- New Row --------------
    QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", blueberryWildWidget);
    startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_wild_header->addWidget(startsSectionHeader);

    for(int i =0; i<9; i++){
        generation_blueberry_wild.append(new QCheckBox(QStringLiteral("%1").arg(i+1), blueberryWildWidget));
        generation_wild_selection->addWidget(generation_blueberry_wild[i]);
        connect(generation_blueberry_wild[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    formLayout->addRow(generation_wild_header);
    formLayout->addRow(generation_wild_selection);

    //--------------Wild Settings End--------------

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(blueberryWildWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupPaldeaRaidWidget(){
    QWidget *paldeaRaidsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(paldeaRaidsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //--------------Raid Settings Start--------------
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();
    QHBoxLayout *row3 = new QHBoxLayout();
    QHBoxLayout *row4 = new QHBoxLayout();
    QHBoxLayout *generation_wild_header = new QHBoxLayout();
    QHBoxLayout *generation_wild_selection = new QHBoxLayout();

    praids_randomize = new QCheckBox("Randomize Raids", paldeaRaidsWidget);
    row0->addWidget(praids_randomize);
    connect(praids_randomize, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    praids_randomize_per_star = new QCheckBox("Use Settings per Raid Level (Useless for now)", paldeaRaidsWidget);
    row0->addWidget(praids_randomize_per_star);
    connect(praids_randomize_per_star, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row0);
    // -------------- New Row --------------

    praids_onlyLegends = new QCheckBox("Only Legends", paldeaRaidsWidget);
    row1->addWidget(praids_onlyLegends);
    connect(praids_onlyLegends, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    praids_onlyParadox = new QCheckBox("Only Paradox", paldeaRaidsWidget);
    row1->addWidget(praids_onlyParadox);
    connect(praids_onlyParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    praids_onlyLegendsandParadox = new QCheckBox("Only Legends and Paradox", paldeaRaidsWidget);
    row1->addWidget(praids_onlyLegendsandParadox);
    connect(praids_onlyLegendsandParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row1);

    // -------------- New Row --------------

    praids_BanStage1 = new QCheckBox("Ban Stage 1", paldeaRaidsWidget);
    row2->addWidget(praids_BanStage1);
    connect(praids_BanStage1, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    praids_BanStage2 = new QCheckBox("Ban Stage 2", paldeaRaidsWidget);
    row2->addWidget(praids_BanStage2);
    connect(praids_BanStage2, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    praids_BanStage3 = new QCheckBox("Ban Stage 3", paldeaRaidsWidget);
    row2->addWidget(praids_BanStage3);
    connect(praids_BanStage3, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    praids_Ban1Stage = new QCheckBox("Ban 1 Stage", paldeaRaidsWidget);
    row2->addWidget(praids_Ban1Stage);
    connect(praids_Ban1Stage, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row2);

    // -------------- New Row --------------
    paldea_Settings_for_all_raids = new QCheckBox("Use Paldea Settings for all other raids", paldeaRaidsWidget);
    row3->addWidget(paldea_Settings_for_all_raids);
    connect(paldea_Settings_for_all_raids, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    praidsBalanceAreaPerBST = new QCheckBox("Balance Raid per BST (useless for now)", paldeaRaidsWidget);
    row3->addWidget(praidsBalanceAreaPerBST);
    connect(praidsBalanceAreaPerBST, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row3);

    // -------------- New Row --------------
    praids_force_shiny = new QCheckBox("Force Shiny Raids", paldeaRaidsWidget);
    row4->addWidget(praids_force_shiny);
    connect(praids_force_shiny, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row4);

    QHBoxLayout *horizontalQuestionsLayout2 = new QHBoxLayout();
    horizontalQuestionsLayout2->setContentsMargins(0, 0, 0, 0);
    horizontalQuestionsLayout2->setSpacing(0);

    QLabel *bulk_question = new QLabel("Raids Shiny Chance ", paldeaRaidsWidget);
    bulk_question->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    praids_shiny_chance= new QSpinBox(paldeaRaidsWidget);  // Number input
    praids_shiny_chance->setRange(1, 100);  // Set range for the number input
    praids_shiny_chance->setValue(10);
    connect(praids_shiny_chance, QOverload<int>::of(&QSpinBox::valueChanged), this, &SVRandomizerWindow::saveSpinBoxValue);
    praids_shiny_chance->setMaximumWidth(100);

    horizontalQuestionsLayout2->addWidget(bulk_question);
    horizontalQuestionsLayout2->addWidget(praids_shiny_chance);

    // Add the horizontal layout to the group
    horizontalQuestionsLayout2->addStretch(1);
    formLayout->addRow(horizontalQuestionsLayout2);

    // -------------- New Row --------------
    QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", paldeaRaidsWidget);
    startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_wild_header->addWidget(startsSectionHeader);

    for(int i =0; i<9; i++){
        praidsgeneration.append(new QCheckBox(QStringLiteral("%1").arg(i+1), paldeaRaidsWidget));
        generation_wild_selection->addWidget(praidsgeneration[i]);
        connect(praidsgeneration[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    formLayout->addRow(generation_wild_header);
    formLayout->addRow(generation_wild_selection);

    //--------------Raid Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(paldeaRaidsWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupKitakamiRaidWidget(){
    QWidget *kitakamiRaidsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(kitakamiRaidsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //--------------Raid Settings Start--------------
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();
    QHBoxLayout *row3 = new QHBoxLayout();
    QHBoxLayout *row4 = new QHBoxLayout();
    QHBoxLayout *generation_wild_header = new QHBoxLayout();
    QHBoxLayout *generation_wild_selection = new QHBoxLayout();

    kraids_randomize = new QCheckBox("Randomize Raids", kitakamiRaidsWidget);
    row0->addWidget(kraids_randomize);
    connect(kraids_randomize, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraids_randomize_per_star = new QCheckBox("Use Settings per Raid Level (Useless for now)", kitakamiRaidsWidget);
    row0->addWidget(kraids_randomize_per_star);
    connect(kraids_randomize_per_star, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row0);
    // -------------- New Row --------------

    kraids_onlyLegends = new QCheckBox("Only Legends", kitakamiRaidsWidget);
    row1->addWidget(kraids_onlyLegends);
    connect(kraids_onlyLegends, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraids_onlyParadox = new QCheckBox("Only Paradox", kitakamiRaidsWidget);
    row1->addWidget(kraids_onlyParadox);
    connect(kraids_onlyParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraids_onlyLegendsandParadox = new QCheckBox("Only Legends and Paradox", kitakamiRaidsWidget);
    row1->addWidget(kraids_onlyLegendsandParadox);
    connect(kraids_onlyLegendsandParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row1);

    // -------------- New Row --------------

    kraids_BanStage1 = new QCheckBox("Ban Stage 1", kitakamiRaidsWidget);
    row2->addWidget(kraids_BanStage1);
    connect(kraids_BanStage1, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraids_BanStage2 = new QCheckBox("Ban Stage 2", kitakamiRaidsWidget);
    row2->addWidget(kraids_BanStage2);
    connect(kraids_BanStage2, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraids_BanStage3 = new QCheckBox("Ban Stage 3", kitakamiRaidsWidget);
    row2->addWidget(kraids_BanStage3);
    connect(kraids_BanStage3, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraids_Ban1Stage = new QCheckBox("Ban 1 Stage", kitakamiRaidsWidget);
    row2->addWidget(kraids_Ban1Stage);
    connect(kraids_Ban1Stage, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row2);

    // -------------- New Row --------------

    kraidsBalanceAreaPerBST = new QCheckBox("Balance Raid per BST (useless for now)", kitakamiRaidsWidget);
    row3->addWidget(kraidsBalanceAreaPerBST);
    connect(kraidsBalanceAreaPerBST, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row3);

    // -------------- New Row --------------
    kraids_force_shiny = new QCheckBox("Force Shiny Raids", kitakamiRaidsWidget);
    row4->addWidget(kraids_force_shiny);
    connect(kraids_force_shiny, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row4);

    QHBoxLayout *horizontalQuestionsLayout2 = new QHBoxLayout();
    horizontalQuestionsLayout2->setContentsMargins(0, 0, 0, 0);
    horizontalQuestionsLayout2->setSpacing(0);

    QLabel *bulk_question = new QLabel("Raids Shiny Chance ", kitakamiRaidsWidget);
    bulk_question->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    kraids_shiny_chance= new QSpinBox(kitakamiRaidsWidget);  // Number input
    kraids_shiny_chance->setRange(1, 100);  // Set range for the number input
    kraids_shiny_chance->setValue(10);
    connect(kraids_shiny_chance, QOverload<int>::of(&QSpinBox::valueChanged), this, &SVRandomizerWindow::saveSpinBoxValue);
    kraids_shiny_chance->setMaximumWidth(100);

    horizontalQuestionsLayout2->addWidget(bulk_question);
    horizontalQuestionsLayout2->addWidget(kraids_shiny_chance);

    // Add the horizontal layout to the group
    horizontalQuestionsLayout2->addStretch(1);
    formLayout->addRow(horizontalQuestionsLayout2);

    // -------------- New Row --------------
    QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", kitakamiRaidsWidget);
    startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_wild_header->addWidget(startsSectionHeader);

    for(int i =0; i<9; i++){
        kraidsgeneration.append(new QCheckBox(QStringLiteral("%1").arg(i+1), kitakamiRaidsWidget));
        generation_wild_selection->addWidget(kraidsgeneration[i]);
        connect(kraidsgeneration[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    formLayout->addRow(generation_wild_header);
    formLayout->addRow(generation_wild_selection);

    //--------------Raid Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(kitakamiRaidsWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupBlueberryRaidWidget(){
    QWidget *blueberryRaidsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(blueberryRaidsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //--------------Raid Settings Start--------------
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();
    QHBoxLayout *row2 = new QHBoxLayout();
    QHBoxLayout *row3 = new QHBoxLayout();
    QHBoxLayout *row4 = new QHBoxLayout();
    QHBoxLayout *generation_wild_header = new QHBoxLayout();
    QHBoxLayout *generation_wild_selection = new QHBoxLayout();

    braids_randomize = new QCheckBox("Randomize Raids", blueberryRaidsWidget);
    row0->addWidget(braids_randomize);
    connect(braids_randomize, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braids_randomize_per_star = new QCheckBox("Use Settings per Raid Level (Useless for now)", blueberryRaidsWidget);
    row0->addWidget(braids_randomize_per_star);
    connect(braids_randomize_per_star, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row0);
    // -------------- New Row --------------

    braids_onlyLegends = new QCheckBox("Only Legends", blueberryRaidsWidget);
    row1->addWidget(braids_onlyLegends);
    connect(braids_onlyLegends, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braids_onlyParadox = new QCheckBox("Only Paradox", blueberryRaidsWidget);
    row1->addWidget(braids_onlyParadox);
    connect(braids_onlyParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braids_onlyLegendsandParadox = new QCheckBox("Only Legends and Paradox", blueberryRaidsWidget);
    row1->addWidget(braids_onlyLegendsandParadox);
    connect(braids_onlyLegendsandParadox, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row1);

    // -------------- New Row --------------

    braids_BanStage1 = new QCheckBox("Ban Stage 1", blueberryRaidsWidget);
    row2->addWidget(braids_BanStage1);
    connect(braids_BanStage1, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braids_BanStage2 = new QCheckBox("Ban Stage 2", blueberryRaidsWidget);
    row2->addWidget(braids_BanStage2);
    connect(braids_BanStage2, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braids_BanStage3 = new QCheckBox("Ban Stage 3", blueberryRaidsWidget);
    row2->addWidget(braids_BanStage3);
    connect(braids_BanStage3, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braids_Ban1Stage = new QCheckBox("Ban 1 Stage", blueberryRaidsWidget);
    row2->addWidget(braids_Ban1Stage);
    connect(braids_Ban1Stage, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row2);

    // -------------- New Row --------------

    braidsBalanceAreaPerBST = new QCheckBox("Balance Raid per BST (useless for now)", blueberryRaidsWidget);
    row3->addWidget(braidsBalanceAreaPerBST);
    connect(braidsBalanceAreaPerBST, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row3);

    // -------------- New Row --------------
    braids_force_shiny = new QCheckBox("Force Shiny Raids", blueberryRaidsWidget);
    row4->addWidget(braids_force_shiny);
    connect(braids_force_shiny, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row4);

    QHBoxLayout *horizontalQuestionsLayout2 = new QHBoxLayout();
    horizontalQuestionsLayout2->setContentsMargins(0, 0, 0, 0);
    horizontalQuestionsLayout2->setSpacing(0);

    QLabel *bulk_question = new QLabel("Raids Shiny Chance ", blueberryRaidsWidget);
    bulk_question->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    braids_shiny_chance= new QSpinBox(blueberryRaidsWidget);  // Number input
    braids_shiny_chance->setRange(1, 100);  // Set range for the number input
    braids_shiny_chance->setValue(10);
    connect(braids_shiny_chance, QOverload<int>::of(&QSpinBox::valueChanged), this, &SVRandomizerWindow::saveSpinBoxValue);
    braids_shiny_chance->setMaximumWidth(100);

    horizontalQuestionsLayout2->addWidget(bulk_question);
    horizontalQuestionsLayout2->addWidget(braids_shiny_chance);

    // Add the horizontal layout to the group
    horizontalQuestionsLayout2->addStretch(1);
    formLayout->addRow(horizontalQuestionsLayout2);

    // -------------- New Row --------------
    QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", blueberryRaidsWidget);
    startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_wild_header->addWidget(startsSectionHeader);

    for(int i =0; i<9; i++){
        braidsgeneration.append(new QCheckBox(QStringLiteral("%1").arg(i+1), blueberryRaidsWidget));
        generation_wild_selection->addWidget(braidsgeneration[i]);
        connect(braidsgeneration[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    formLayout->addRow(generation_wild_header);
    formLayout->addRow(generation_wild_selection);

    //--------------Raid Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget

    scrollArea->setWidget(blueberryRaidsWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupTrainersPaldeaWidget(){
    QWidget *paldeaTrainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(paldeaTrainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //--------------All Trainers Settings Start--------------
        QGroupBox *allTrainers_Settings_Group = new QGroupBox("General Settings Section");
        QPushButton *toggleAllTrainersSettingGroup = new QPushButton("Toggle General Settings Section", paldeaTrainersWidget);
        toggleAllTrainersSettingGroup->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"
            "   color: white;"
            "   border: none;"
            "   padding: 8px 10px;"
            "   border-radius: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #1c6692;"
            "}"
            );

        // Connect to Hide the section if needed
        connect(toggleAllTrainersSettingGroup, &QPushButton::clicked, [allTrainers_Settings_Group]() {
            allTrainers_Settings_Group->setVisible(!allTrainers_Settings_Group->isVisible());
        });

        formLayout->addRow(toggleAllTrainersSettingGroup);

        QVBoxLayout *allTrainersLayout = new QVBoxLayout(allTrainers_Settings_Group);
        QHBoxLayout *row0_all = new QHBoxLayout();
        QHBoxLayout *row1_all = new QHBoxLayout();
        QHBoxLayout *row2_all = new QHBoxLayout();
        // QHBoxLayout *row3 = new QHBoxLayout();
        // QHBoxLayout *row4 = new QHBoxLayout();
        QHBoxLayout *generation_trainers_header = new QHBoxLayout();
        QHBoxLayout *generation_trainers_selection = new QHBoxLayout();

        // -------------- New Row --------------
        randomize_paldea_trainers = new QCheckBox("Randomize Trainers", allTrainers_Settings_Group);
        row0_all->addWidget(randomize_paldea_trainers);
        connect(randomize_paldea_trainers, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pglobal_trainer_randomizer_settings.append(new QCheckBox("Allow All Trainers to Tera", allTrainers_Settings_Group));
        row0_all->addWidget(pglobal_trainer_randomizer_settings[0]);
        connect(pglobal_trainer_randomizer_settings[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pglobal_trainer_randomizer_settings.append(new QCheckBox("Randomize Tera Types", allTrainers_Settings_Group));
        row0_all->addWidget(pglobal_trainer_randomizer_settings[1]);
        connect(pglobal_trainer_randomizer_settings[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pglobal_trainer_randomizer_settings.append(new QCheckBox("Allow Shiny Pokemon", allTrainers_Settings_Group));
        row0_all->addWidget(pglobal_trainer_randomizer_settings[2]);
        connect(pglobal_trainer_randomizer_settings[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        allTrainersLayout->addLayout(row0_all);

        // -------------- New Row --------------
        pglobal_trainer_randomizer_settings.append(new QCheckBox("Randomnly Choose Single/Double Battles", allTrainers_Settings_Group));
        row1_all->addWidget(pglobal_trainer_randomizer_settings[3]);
        connect(pglobal_trainer_randomizer_settings[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pglobal_trainer_randomizer_settings.append(new QCheckBox("Only Double Battles", allTrainers_Settings_Group));
        row1_all->addWidget(pglobal_trainer_randomizer_settings[4]);
        connect(pglobal_trainer_randomizer_settings[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        allTrainersLayout->addLayout(row1_all);

        // -------------- New Row --------------
        use_trainer_paldea_for_all = new QCheckBox("Use Paldea Settings for All Regions", allTrainers_Settings_Group);
        row2_all->addWidget(use_trainer_paldea_for_all);
        connect(use_trainer_paldea_for_all, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pglobal_trainer_randomizer_settings.append(new QCheckBox("Use Rival settings for All", allTrainers_Settings_Group));
        row2_all->addWidget(pglobal_trainer_randomizer_settings[5]);
        connect(pglobal_trainer_randomizer_settings[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        allTrainersLayout->addLayout(row2_all);

        // -------------- New Row --------------
        QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", allTrainers_Settings_Group);
        startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
        generation_trainers_header->addWidget(startsSectionHeader);

        for(int i =0; i<9; i++){
            ptrainersgeneration.append(new QCheckBox(QStringLiteral("%1").arg(i+1), allTrainers_Settings_Group));
            generation_trainers_selection->addWidget(ptrainersgeneration[i]);
            connect(ptrainersgeneration[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
        }
        allTrainersLayout->addLayout(generation_trainers_header);
        allTrainersLayout->addLayout(generation_trainers_selection);

    formLayout->addRow(allTrainers_Settings_Group);
    //--------------All Trainers Settings End--------------
    //--------------Rivals Settings Start--------------
        QGroupBox *rivals_Settings_Group = new QGroupBox("Rival Settings Section");
        QPushButton *toggleRivalsSettingGroup = new QPushButton("Toggle Rival Settings Section", paldeaTrainersWidget);
        toggleRivalsSettingGroup->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"
            "   color: white;"
            "   border: none;"
            "   padding: 8px 10px;"
            "   border-radius: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #1c6692;"
            "}"
            );

        // Connect to Hide the section if needed
        connect(toggleRivalsSettingGroup, &QPushButton::clicked, [rivals_Settings_Group]() {
            rivals_Settings_Group->setVisible(!rivals_Settings_Group->isVisible());
        });

        formLayout->addRow(toggleRivalsSettingGroup);

        QVBoxLayout *rivalsTrainersLayout = new QVBoxLayout(rivals_Settings_Group);
        QHBoxLayout *row0_rivals = new QHBoxLayout();
        QHBoxLayout *row1_rivals = new QHBoxLayout();
        QHBoxLayout *row2_rivals = new QHBoxLayout();
        QHBoxLayout *row3_rivals = new QHBoxLayout();
        // QHBoxLayout *row4 = new QHBoxLayout();

        // -------------- New Row --------------
        prival_randomizer.append(new QCheckBox("Force 6 Pokemon", rivals_Settings_Group));
        row0_rivals->addWidget(prival_randomizer[0]);
        connect(prival_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        prival_randomizer.append(new QCheckBox("Give Extra Pokemon", rivals_Settings_Group));
        row0_rivals->addWidget(prival_randomizer[1]);
        connect(prival_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        prival_randomizer.append(new QCheckBox("Allow Terastalization", rivals_Settings_Group));
        row0_rivals->addWidget(prival_randomizer[2]);
        connect(prival_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        rivalsTrainersLayout->addLayout(row0_rivals);

        // -------------- New Row --------------
        prival_randomizer.append(new QCheckBox("Force Perfect IVs", rivals_Settings_Group));
        row1_rivals->addWidget(prival_randomizer[3]);
        connect(prival_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        prival_randomizer.append(new QCheckBox("Make AI Smart", rivals_Settings_Group));
        row1_rivals->addWidget(prival_randomizer[4]);
        connect(prival_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


        rivalsTrainersLayout->addLayout(row1_rivals);

        // -------------- New Row --------------
        prival_randomizer.append(new QCheckBox("Only Legends", rivals_Settings_Group));
        row2_rivals->addWidget(prival_randomizer[5]);
        connect(prival_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        prival_randomizer.append(new QCheckBox("Only Paradox", rivals_Settings_Group));
        row2_rivals->addWidget(prival_randomizer[6]);
        connect(prival_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        prival_randomizer.append(new QCheckBox("Only Legends and Paradox", rivals_Settings_Group));
        row2_rivals->addWidget(prival_randomizer[7]);
        connect(prival_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        rivalsTrainersLayout->addLayout(row2_rivals);

        // -------------- New Row --------------
        prival_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", rivals_Settings_Group));
        row3_rivals->addWidget(prival_randomizer[8]);
        connect(prival_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        prival_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", rivals_Settings_Group));
        row3_rivals->addWidget(prival_randomizer[9]);
        connect(prival_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        prival_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", rivals_Settings_Group));
        row3_rivals->addWidget(prival_randomizer[10]);
        connect(prival_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        prival_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", rivals_Settings_Group));
        row3_rivals->addWidget(prival_randomizer[11]);
        connect(prival_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        rivalsTrainersLayout->addLayout(row3_rivals);


    formLayout->addRow(rivals_Settings_Group);
    //--------------Rivals Settings End--------------
    //--------------Route Settings Start--------------
        QGroupBox *route_Settings_Group = new QGroupBox("Routes Settings Section");
        QPushButton *toggleRoutesSettingGroup = new QPushButton("Toggle Routes Settings Section", paldeaTrainersWidget);
        toggleRoutesSettingGroup->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"
            "   color: white;"
            "   border: none;"
            "   padding: 8px 10px;"
            "   border-radius: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #1c6692;"
            "}"
            );

        // Connect to Hide the section if needed
        connect(toggleRoutesSettingGroup, &QPushButton::clicked, [route_Settings_Group]() {
            route_Settings_Group->setVisible(!route_Settings_Group->isVisible());
        });

        formLayout->addRow(toggleRoutesSettingGroup);

        QVBoxLayout *routesTrainersLayout = new QVBoxLayout(route_Settings_Group);
        QHBoxLayout *row0_routes = new QHBoxLayout();
        QHBoxLayout *row1_routes = new QHBoxLayout();
        QHBoxLayout *row2_routes = new QHBoxLayout();
        QHBoxLayout *row3_routes = new QHBoxLayout();
        // QHBoxLayout *row4 = new QHBoxLayout();

        // -------------- New Row --------------
        proute_randomizer.append(new QCheckBox("Force 6 Pokemon", route_Settings_Group));
        row0_routes->addWidget(proute_randomizer[0]);
        connect(proute_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        proute_randomizer.append(new QCheckBox("Give Extra Pokemon", route_Settings_Group));
        row0_routes->addWidget(proute_randomizer[1]);
        connect(proute_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        proute_randomizer.append(new QCheckBox("Allow Terastalization", route_Settings_Group));
        row0_routes->addWidget(proute_randomizer[2]);
        connect(proute_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        routesTrainersLayout->addLayout(row0_routes);

        // -------------- New Row --------------
        proute_randomizer.append(new QCheckBox("Force Perfect IVs", route_Settings_Group));
        row1_routes->addWidget(proute_randomizer[3]);
        connect(proute_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        proute_randomizer.append(new QCheckBox("Make AI Smart", route_Settings_Group));
        row1_routes->addWidget(proute_randomizer[4]);
        connect(proute_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


        routesTrainersLayout->addLayout(row1_routes);

        // -------------- New Row --------------
        proute_randomizer.append(new QCheckBox("Only Legends", route_Settings_Group));
        row2_routes->addWidget(proute_randomizer[5]);
        connect(proute_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        proute_randomizer.append(new QCheckBox("Only Paradox", route_Settings_Group));
        row2_routes->addWidget(proute_randomizer[6]);
        connect(proute_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        proute_randomizer.append(new QCheckBox("Only Legends and Paradox", route_Settings_Group));
        row2_routes->addWidget(proute_randomizer[7]);
        connect(proute_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        routesTrainersLayout->addLayout(row2_routes);

        // -------------- New Row --------------
        proute_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", route_Settings_Group));
        row3_routes->addWidget(proute_randomizer[8]);
        connect(proute_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        proute_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", route_Settings_Group));
        row3_routes->addWidget(proute_randomizer[9]);
        connect(proute_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        proute_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", route_Settings_Group));
        row3_routes->addWidget(proute_randomizer[10]);
        connect(proute_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        proute_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", route_Settings_Group));
        row3_routes->addWidget(proute_randomizer[11]);
        connect(proute_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        routesTrainersLayout->addLayout(row3_routes);


        formLayout->addRow(route_Settings_Group);
    //--------------Route Settings End--------------
    //--------------Gym Settings Start--------------
        QGroupBox *gym_Settings_Group = new QGroupBox("Gym Settings Section");
        QPushButton *toggleGymsSettingGroup = new QPushButton("Toggle Gym Settings Section", paldeaTrainersWidget);
        toggleGymsSettingGroup->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"
            "   color: white;"
            "   border: none;"
            "   padding: 8px 10px;"
            "   border-radius: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #1c6692;"
            "}"
            );

        // Connect to Hide the section if needed
        connect(toggleGymsSettingGroup, &QPushButton::clicked, [gym_Settings_Group]() {
            gym_Settings_Group->setVisible(!gym_Settings_Group->isVisible());
        });

        formLayout->addRow(toggleGymsSettingGroup);

        QVBoxLayout *gymTrainersLayout = new QVBoxLayout(gym_Settings_Group);
        QHBoxLayout *row0_gym = new QHBoxLayout();
        QHBoxLayout *row1_gym = new QHBoxLayout();
        QHBoxLayout *row2_gym = new QHBoxLayout();
        QHBoxLayout *row3_gym = new QHBoxLayout();
        // QHBoxLayout *row4 = new QHBoxLayout();

        // -------------- New Row --------------
        pgym_randomizer.append(new QCheckBox("Force 6 Pokemon", gym_Settings_Group));
        row0_gym->addWidget(pgym_randomizer[0]);
        connect(pgym_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pgym_randomizer.append(new QCheckBox("Give Extra Pokemon", gym_Settings_Group));
        row0_gym->addWidget(pgym_randomizer[1]);
        connect(pgym_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pgym_randomizer.append(new QCheckBox("Allow Terastalization", gym_Settings_Group));
        row0_gym->addWidget(pgym_randomizer[2]);
        connect(pgym_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        gymTrainersLayout->addLayout(row0_gym);

        // -------------- New Row --------------
        pgym_randomizer.append(new QCheckBox("Force Perfect IVs", gym_Settings_Group));
        row1_gym->addWidget(pgym_randomizer[3]);
        connect(pgym_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pgym_randomizer.append(new QCheckBox("Make AI Smart", gym_Settings_Group));
        row1_gym->addWidget(pgym_randomizer[4]);
        connect(pgym_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


        gymTrainersLayout->addLayout(row1_gym);

        // -------------- New Row --------------
        pgym_randomizer.append(new QCheckBox("Only Legends", gym_Settings_Group));
        row2_gym->addWidget(pgym_randomizer[5]);
        connect(pgym_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pgym_randomizer.append(new QCheckBox("Only Paradox", gym_Settings_Group));
        row2_gym->addWidget(pgym_randomizer[6]);
        connect(pgym_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pgym_randomizer.append(new QCheckBox("Only Legends and Paradox", gym_Settings_Group));
        row2_gym->addWidget(pgym_randomizer[7]);
        connect(pgym_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        gymTrainersLayout->addLayout(row2_gym);

        // -------------- New Row --------------
        pgym_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", gym_Settings_Group));
        row3_gym->addWidget(pgym_randomizer[8]);
        connect(pgym_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pgym_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", gym_Settings_Group));
        row3_gym->addWidget(pgym_randomizer[9]);
        connect(pgym_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pgym_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", gym_Settings_Group));
        row3_gym->addWidget(pgym_randomizer[10]);
        connect(pgym_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pgym_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", gym_Settings_Group));
        row3_gym->addWidget(pgym_randomizer[11]);
        connect(pgym_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        gymTrainersLayout->addLayout(row3_gym);


        formLayout->addRow(gym_Settings_Group);
    //--------------Gym Settings End--------------
    //--------------E4 Settings Start--------------
        QGroupBox *e4_Settings_Group = new QGroupBox("Elite 4 Settings Section");
        QPushButton *toggleE4SettingGroup = new QPushButton("Toggle Elite 4 Settings Section", paldeaTrainersWidget);
        toggleE4SettingGroup->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"
            "   color: white;"
            "   border: none;"
            "   padding: 8px 10px;"
            "   border-radius: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #1c6692;"
            "}"
            );

        // Connect to Hide the section if needed
        connect(toggleE4SettingGroup, &QPushButton::clicked, [e4_Settings_Group]() {
            e4_Settings_Group->setVisible(!e4_Settings_Group->isVisible());
        });

        formLayout->addRow(toggleE4SettingGroup);

        QVBoxLayout *e4TrainersLayout = new QVBoxLayout(e4_Settings_Group);
        QHBoxLayout *row0_e4 = new QHBoxLayout();
        QHBoxLayout *row1_e4 = new QHBoxLayout();
        QHBoxLayout *row2_e4 = new QHBoxLayout();
        QHBoxLayout *row3_e4 = new QHBoxLayout();
        // QHBoxLayout *row4 = new QHBoxLayout();

        // -------------- New Row --------------
        pelite4_randomizer.append(new QCheckBox("Force 6 Pokemon", e4_Settings_Group));
        row0_e4->addWidget(pelite4_randomizer[0]);
        connect(pelite4_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pelite4_randomizer.append(new QCheckBox("Give Extra Pokemon", e4_Settings_Group));
        row0_e4->addWidget(pelite4_randomizer[1]);
        connect(pelite4_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pelite4_randomizer.append(new QCheckBox("Allow Terastalization", e4_Settings_Group));
        row0_e4->addWidget(pelite4_randomizer[2]);
        connect(pelite4_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        e4TrainersLayout->addLayout(row0_e4);

        // -------------- New Row --------------
        pelite4_randomizer.append(new QCheckBox("Force Perfect IVs", e4_Settings_Group));
        row1_e4->addWidget(pelite4_randomizer[3]);
        connect(pelite4_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pelite4_randomizer.append(new QCheckBox("Make AI Smart", e4_Settings_Group));
        row1_e4->addWidget(pelite4_randomizer[4]);
        connect(pelite4_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


        e4TrainersLayout->addLayout(row1_e4);

        // -------------- New Row --------------
        pelite4_randomizer.append(new QCheckBox("Only Legends", e4_Settings_Group));
        row2_e4->addWidget(pelite4_randomizer[5]);
        connect(pelite4_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pelite4_randomizer.append(new QCheckBox("Only Paradox", e4_Settings_Group));
        row2_e4->addWidget(pelite4_randomizer[6]);
        connect(pelite4_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pelite4_randomizer.append(new QCheckBox("Only Legends and Paradox", e4_Settings_Group));
        row2_e4->addWidget(pelite4_randomizer[7]);
        connect(pelite4_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        e4TrainersLayout->addLayout(row2_e4);

        // -------------- New Row --------------
        pelite4_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", e4_Settings_Group));
        row3_e4->addWidget(pelite4_randomizer[8]);
        connect(pelite4_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pelite4_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", e4_Settings_Group));
        row3_e4->addWidget(pelite4_randomizer[9]);
        connect(pelite4_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pelite4_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", e4_Settings_Group));
        row3_e4->addWidget(pelite4_randomizer[10]);
        connect(pelite4_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pelite4_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", e4_Settings_Group));
        row3_e4->addWidget(pelite4_randomizer[11]);
        connect(pelite4_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        e4TrainersLayout->addLayout(row3_e4);


        formLayout->addRow(e4_Settings_Group);
    //--------------E4 Settings End--------------
    //--------------Champion Settings Start--------------
        QGroupBox *champion_Settings_Group = new QGroupBox("Champion Settings Section");
        QPushButton *toggleChampionSettingGroup = new QPushButton("Toggle Champion Settings Section", paldeaTrainersWidget);
        toggleChampionSettingGroup->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"
            "   color: white;"
            "   border: none;"
            "   padding: 8px 10px;"
            "   border-radius: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #1c6692;"
            "}"
            );

        // Connect to Hide the section if needed
        connect(toggleChampionSettingGroup, &QPushButton::clicked, [champion_Settings_Group]() {
            champion_Settings_Group->setVisible(!champion_Settings_Group->isVisible());
        });

        formLayout->addRow(toggleChampionSettingGroup);

        QVBoxLayout *champTrainersLayout = new QVBoxLayout(champion_Settings_Group);
        QHBoxLayout *row0_champ = new QHBoxLayout();
        QHBoxLayout *row1_champ = new QHBoxLayout();
        QHBoxLayout *row2_champ = new QHBoxLayout();
        QHBoxLayout *row3_champ = new QHBoxLayout();
        // QHBoxLayout *row4 = new QHBoxLayout();

        // -------------- New Row --------------
        pchampion_randomizer.append(new QCheckBox("Force 6 Pokemon", champion_Settings_Group));
        row0_champ->addWidget(pchampion_randomizer[0]);
        connect(pchampion_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pchampion_randomizer.append(new QCheckBox("Give Extra Pokemon", champion_Settings_Group));
        row0_champ->addWidget(pchampion_randomizer[1]);
        connect(pchampion_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pchampion_randomizer.append(new QCheckBox("Allow Terastalization", champion_Settings_Group));
        row0_champ->addWidget(pchampion_randomizer[2]);
        connect(pchampion_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        champTrainersLayout->addLayout(row0_champ);

        // -------------- New Row --------------
        pchampion_randomizer.append(new QCheckBox("Force Perfect IVs", champion_Settings_Group));
        row1_champ->addWidget(pchampion_randomizer[3]);
        connect(pchampion_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pchampion_randomizer.append(new QCheckBox("Make AI Smart", champion_Settings_Group));
        row1_champ->addWidget(pchampion_randomizer[4]);
        connect(pchampion_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


        champTrainersLayout->addLayout(row1_champ);

        // -------------- New Row --------------
        pchampion_randomizer.append(new QCheckBox("Only Legends", champion_Settings_Group));
        row2_champ->addWidget(pchampion_randomizer[5]);
        connect(pchampion_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pchampion_randomizer.append(new QCheckBox("Only Paradox", champion_Settings_Group));
        row2_champ->addWidget(pchampion_randomizer[6]);
        connect(pchampion_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pchampion_randomizer.append(new QCheckBox("Only Legends and Paradox", champion_Settings_Group));
        row2_champ->addWidget(pchampion_randomizer[7]);
        connect(pchampion_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        champTrainersLayout->addLayout(row2_champ);

        // -------------- New Row --------------
        pchampion_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", champion_Settings_Group));
        row3_champ->addWidget(pchampion_randomizer[8]);
        connect(pchampion_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pchampion_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", champion_Settings_Group));
        row3_champ->addWidget(pchampion_randomizer[9]);
        connect(pchampion_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pchampion_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", champion_Settings_Group));
        row3_champ->addWidget(pchampion_randomizer[10]);
        connect(pchampion_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        pchampion_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", champion_Settings_Group));
        row3_champ->addWidget(pchampion_randomizer[11]);
        connect(pchampion_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        champTrainersLayout->addLayout(row3_champ);


        formLayout->addRow(champion_Settings_Group);
    //--------------Champion Settings End--------------
    //--------------Raid Trainers Settings Start--------------
        QGroupBox *raids_Settings_Group = new QGroupBox("Raid Trainers Settings Section");
        QPushButton *toggleRaidsSettingGroup = new QPushButton("Toggle Raid Trainers Settings Section", paldeaTrainersWidget);
        toggleRaidsSettingGroup->setStyleSheet(
            "QPushButton {"
            "   background-color: #3498db;"
            "   color: white;"
            "   border: none;"
            "   padding: 8px 10px;"
            "   border-radius: 10px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #2980b9;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #1c6692;"
            "}"
            );

        // Connect to Hide the section if needed
        connect(toggleRaidsSettingGroup, &QPushButton::clicked, [raids_Settings_Group]() {
            raids_Settings_Group->setVisible(!raids_Settings_Group->isVisible());
        });

        formLayout->addRow(toggleRaidsSettingGroup);

        QVBoxLayout *raidTrainersLayout = new QVBoxLayout(raids_Settings_Group);
        QHBoxLayout *row0_raid = new QHBoxLayout();
        QHBoxLayout *row1_raid = new QHBoxLayout();
        QHBoxLayout *row2_raid = new QHBoxLayout();
        QHBoxLayout *row3_raid = new QHBoxLayout();
        // QHBoxLayout *row4 = new QHBoxLayout();

        // -------------- New Row --------------
        praid_randomizer.append(new QCheckBox("Force 6 Pokemon", raids_Settings_Group));
        row0_raid->addWidget(praid_randomizer[0]);
        connect(praid_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        praid_randomizer.append(new QCheckBox("Give Extra Pokemon", raids_Settings_Group));
        row0_raid->addWidget(praid_randomizer[1]);
        connect(praid_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        praid_randomizer.append(new QCheckBox("Allow Terastalization", raids_Settings_Group));
        row0_raid->addWidget(praid_randomizer[2]);
        connect(praid_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        raidTrainersLayout->addLayout(row0_raid);

        // -------------- New Row --------------
        praid_randomizer.append(new QCheckBox("Force Perfect IVs", raids_Settings_Group));
        row1_raid->addWidget(praid_randomizer[3]);
        connect(praid_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        praid_randomizer.append(new QCheckBox("Make AI Smart", raids_Settings_Group));
        row1_raid->addWidget(praid_randomizer[4]);
        connect(praid_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


        raidTrainersLayout->addLayout(row1_raid);

        // -------------- New Row --------------
        praid_randomizer.append(new QCheckBox("Only Legends", raids_Settings_Group));
        row2_raid->addWidget(praid_randomizer[5]);
        connect(praid_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        praid_randomizer.append(new QCheckBox("Only Paradox", raids_Settings_Group));
        row2_raid->addWidget(praid_randomizer[6]);
        connect(praid_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        praid_randomizer.append(new QCheckBox("Only Legends and Paradox", raids_Settings_Group));
        row2_raid->addWidget(praid_randomizer[7]);
        connect(praid_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        raidTrainersLayout->addLayout(row2_raid);

        // -------------- New Row --------------
        praid_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", raids_Settings_Group));
        row3_raid->addWidget(praid_randomizer[8]);
        connect(praid_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        praid_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", raids_Settings_Group));
        row3_raid->addWidget(praid_randomizer[9]);
        connect(praid_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        praid_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", raids_Settings_Group));
        row3_raid->addWidget(praid_randomizer[10]);
        connect(praid_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        praid_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", raids_Settings_Group));
        row3_raid->addWidget(praid_randomizer[11]);
        connect(praid_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        raidTrainersLayout->addLayout(row3_raid);


        formLayout->addRow(raids_Settings_Group);
        //--------------Champion Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(paldeaTrainersWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupTrainersKitaWidget(){
    QWidget *kitakamiTrainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(kitakamiTrainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //--------------All Trainers Settings Start--------------
    QGroupBox *allTrainers_Settings_Group = new QGroupBox("General Settings Section");
    QPushButton *toggleAllTrainersSettingGroup = new QPushButton("Toggle General Settings Section", kitakamiTrainersWidget);
    toggleAllTrainersSettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleAllTrainersSettingGroup, &QPushButton::clicked, [allTrainers_Settings_Group]() {
        allTrainers_Settings_Group->setVisible(!allTrainers_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleAllTrainersSettingGroup);

    QVBoxLayout *allTrainersLayout = new QVBoxLayout(allTrainers_Settings_Group);
    QHBoxLayout *row0_all = new QHBoxLayout();
    QHBoxLayout *row1_all = new QHBoxLayout();
    QHBoxLayout *row2_all = new QHBoxLayout();
    // QHBoxLayout *row3 = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();
    QHBoxLayout *generation_trainers_header = new QHBoxLayout();
    QHBoxLayout *generation_trainers_selection = new QHBoxLayout();

    // -------------- New Row --------------
    randomize_kitakami_trainers = new QCheckBox("Randomize Trainers", allTrainers_Settings_Group);
    row0_all->addWidget(randomize_kitakami_trainers);
    connect(randomize_kitakami_trainers, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kglobal_trainer_randomizer_settings.append(new QCheckBox("Allow All Trainers to Tera", allTrainers_Settings_Group));
    row0_all->addWidget(kglobal_trainer_randomizer_settings[0]);
    connect(kglobal_trainer_randomizer_settings[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kglobal_trainer_randomizer_settings.append(new QCheckBox("Randomize Tera Types", allTrainers_Settings_Group));
    row0_all->addWidget(kglobal_trainer_randomizer_settings[1]);
    connect(kglobal_trainer_randomizer_settings[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kglobal_trainer_randomizer_settings.append(new QCheckBox("Allow Shiny Pokemon", allTrainers_Settings_Group));
    row0_all->addWidget(kglobal_trainer_randomizer_settings[2]);
    connect(kglobal_trainer_randomizer_settings[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    allTrainersLayout->addLayout(row0_all);

    // -------------- New Row --------------
    kglobal_trainer_randomizer_settings.append(new QCheckBox("Randomnly Choose Single/Double Battles", allTrainers_Settings_Group));
    row1_all->addWidget(kglobal_trainer_randomizer_settings[3]);
    connect(kglobal_trainer_randomizer_settings[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kglobal_trainer_randomizer_settings.append(new QCheckBox("Only Double Battles", allTrainers_Settings_Group));
    row1_all->addWidget(kglobal_trainer_randomizer_settings[4]);
    connect(kglobal_trainer_randomizer_settings[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    allTrainersLayout->addLayout(row1_all);

    // -------------- New Row --------------
    kglobal_trainer_randomizer_settings.append(new QCheckBox("Use Rival settings for All", allTrainers_Settings_Group));
    row2_all->addWidget(kglobal_trainer_randomizer_settings[5]);
    connect(kglobal_trainer_randomizer_settings[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    allTrainersLayout->addLayout(row2_all);

    // -------------- New Row --------------
    QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", allTrainers_Settings_Group);
    startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_trainers_header->addWidget(startsSectionHeader);

    for(int i =0; i<9; i++){
        ktrainersgeneration.append(new QCheckBox(QStringLiteral("%1").arg(i+1), allTrainers_Settings_Group));
        generation_trainers_selection->addWidget(ktrainersgeneration[i]);
        connect(ktrainersgeneration[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    allTrainersLayout->addLayout(generation_trainers_header);
    allTrainersLayout->addLayout(generation_trainers_selection);

    formLayout->addRow(allTrainers_Settings_Group);
    //--------------All Trainers Settings End--------------
    //--------------Rivals Settings Start--------------
    QGroupBox *rivals_Settings_Group = new QGroupBox("Rival Settings Section");
    QPushButton *toggleRivalsSettingGroup = new QPushButton("Toggle Rival Settings Section", kitakamiTrainersWidget);
    toggleRivalsSettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleRivalsSettingGroup, &QPushButton::clicked, [rivals_Settings_Group]() {
        rivals_Settings_Group->setVisible(!rivals_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleRivalsSettingGroup);

    QVBoxLayout *rivalsTrainersLayout = new QVBoxLayout(rivals_Settings_Group);
    QHBoxLayout *row0_rivals = new QHBoxLayout();
    QHBoxLayout *row1_rivals = new QHBoxLayout();
    QHBoxLayout *row2_rivals = new QHBoxLayout();
    QHBoxLayout *row3_rivals = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();

    // -------------- New Row --------------
    krival_randomizer.append(new QCheckBox("Force 6 Pokemon", rivals_Settings_Group));
    row0_rivals->addWidget(krival_randomizer[0]);
    connect(krival_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    krival_randomizer.append(new QCheckBox("Give Extra Pokemon", rivals_Settings_Group));
    row0_rivals->addWidget(krival_randomizer[1]);
    connect(krival_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    krival_randomizer.append(new QCheckBox("Allow Terastalization", rivals_Settings_Group));
    row0_rivals->addWidget(krival_randomizer[2]);
    connect(krival_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    rivalsTrainersLayout->addLayout(row0_rivals);

    // -------------- New Row --------------
    krival_randomizer.append(new QCheckBox("Force Perfect IVs", rivals_Settings_Group));
    row1_rivals->addWidget(krival_randomizer[3]);
    connect(krival_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    krival_randomizer.append(new QCheckBox("Make AI Smart", rivals_Settings_Group));
    row1_rivals->addWidget(krival_randomizer[4]);
    connect(krival_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    rivalsTrainersLayout->addLayout(row1_rivals);

    // -------------- New Row --------------
    krival_randomizer.append(new QCheckBox("Only Legends", rivals_Settings_Group));
    row2_rivals->addWidget(krival_randomizer[5]);
    connect(krival_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    krival_randomizer.append(new QCheckBox("Only Paradox", rivals_Settings_Group));
    row2_rivals->addWidget(krival_randomizer[6]);
    connect(krival_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    krival_randomizer.append(new QCheckBox("Only Legends and Paradox", rivals_Settings_Group));
    row2_rivals->addWidget(krival_randomizer[7]);
    connect(krival_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    rivalsTrainersLayout->addLayout(row2_rivals);

    // -------------- New Row --------------
    krival_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", rivals_Settings_Group));
    row3_rivals->addWidget(krival_randomizer[8]);
    connect(krival_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    krival_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", rivals_Settings_Group));
    row3_rivals->addWidget(krival_randomizer[9]);
    connect(krival_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    krival_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", rivals_Settings_Group));
    row3_rivals->addWidget(krival_randomizer[10]);
    connect(krival_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    krival_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", rivals_Settings_Group));
    row3_rivals->addWidget(krival_randomizer[11]);
    connect(krival_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    rivalsTrainersLayout->addLayout(row3_rivals);


    formLayout->addRow(rivals_Settings_Group);
    //--------------Rivals Settings End--------------
    //--------------Route Settings Start--------------
    QGroupBox *route_Settings_Group = new QGroupBox("Routes Settings Section");
    QPushButton *toggleRoutesSettingGroup = new QPushButton("Toggle Routes Settings Section", kitakamiTrainersWidget);
    toggleRoutesSettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleRoutesSettingGroup, &QPushButton::clicked, [route_Settings_Group]() {
        route_Settings_Group->setVisible(!route_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleRoutesSettingGroup);

    QVBoxLayout *routesTrainersLayout = new QVBoxLayout(route_Settings_Group);
    QHBoxLayout *row0_routes = new QHBoxLayout();
    QHBoxLayout *row1_routes = new QHBoxLayout();
    QHBoxLayout *row2_routes = new QHBoxLayout();
    QHBoxLayout *row3_routes = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();

    // -------------- New Row --------------
    kroute_randomizer.append(new QCheckBox("Force 6 Pokemon", route_Settings_Group));
    row0_routes->addWidget(kroute_randomizer[0]);
    connect(kroute_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kroute_randomizer.append(new QCheckBox("Give Extra Pokemon", route_Settings_Group));
    row0_routes->addWidget(kroute_randomizer[1]);
    connect(kroute_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kroute_randomizer.append(new QCheckBox("Allow Terastalization", route_Settings_Group));
    row0_routes->addWidget(kroute_randomizer[2]);
    connect(kroute_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    routesTrainersLayout->addLayout(row0_routes);

    // -------------- New Row --------------
    kroute_randomizer.append(new QCheckBox("Force Perfect IVs", route_Settings_Group));
    row1_routes->addWidget(kroute_randomizer[3]);
    connect(kroute_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kroute_randomizer.append(new QCheckBox("Make AI Smart", route_Settings_Group));
    row1_routes->addWidget(kroute_randomizer[4]);
    connect(kroute_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    routesTrainersLayout->addLayout(row1_routes);

    // -------------- New Row --------------
    kroute_randomizer.append(new QCheckBox("Only Legends", route_Settings_Group));
    row2_routes->addWidget(kroute_randomizer[5]);
    connect(kroute_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kroute_randomizer.append(new QCheckBox("Only Paradox", route_Settings_Group));
    row2_routes->addWidget(kroute_randomizer[6]);
    connect(kroute_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kroute_randomizer.append(new QCheckBox("Only Legends and Paradox", route_Settings_Group));
    row2_routes->addWidget(kroute_randomizer[7]);
    connect(kroute_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    routesTrainersLayout->addLayout(row2_routes);

    // -------------- New Row --------------
    kroute_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", route_Settings_Group));
    row3_routes->addWidget(kroute_randomizer[8]);
    connect(kroute_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kroute_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", route_Settings_Group));
    row3_routes->addWidget(kroute_randomizer[9]);
    connect(kroute_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kroute_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", route_Settings_Group));
    row3_routes->addWidget(kroute_randomizer[10]);
    connect(kroute_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kroute_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", route_Settings_Group));
    row3_routes->addWidget(kroute_randomizer[11]);
    connect(kroute_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    routesTrainersLayout->addLayout(row3_routes);


    formLayout->addRow(route_Settings_Group);
    //--------------Route Settings End--------------
    //--------------Gym Settings Start--------------
    QGroupBox *gym_Settings_Group = new QGroupBox("Ogre Clan Settings Section");
    QPushButton *toggleGymsSettingGroup = new QPushButton("Toggle Ogre Clan Settings Section", kitakamiTrainersWidget);
    toggleGymsSettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleGymsSettingGroup, &QPushButton::clicked, [gym_Settings_Group]() {
        gym_Settings_Group->setVisible(!gym_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleGymsSettingGroup);

    QVBoxLayout *gymTrainersLayout = new QVBoxLayout(gym_Settings_Group);
    QHBoxLayout *row0_gym = new QHBoxLayout();
    QHBoxLayout *row1_gym = new QHBoxLayout();
    QHBoxLayout *row2_gym = new QHBoxLayout();
    QHBoxLayout *row3_gym = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();

    // -------------- New Row --------------
    kogre_clan_randomizer.append(new QCheckBox("Force 6 Pokemon", gym_Settings_Group));
    row0_gym->addWidget(kogre_clan_randomizer[0]);
    connect(kogre_clan_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kogre_clan_randomizer.append(new QCheckBox("Give Extra Pokemon", gym_Settings_Group));
    row0_gym->addWidget(kogre_clan_randomizer[1]);
    connect(kogre_clan_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kogre_clan_randomizer.append(new QCheckBox("Allow Terastalization", gym_Settings_Group));
    row0_gym->addWidget(kogre_clan_randomizer[2]);
    connect(kogre_clan_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    gymTrainersLayout->addLayout(row0_gym);

    // -------------- New Row --------------
    kogre_clan_randomizer.append(new QCheckBox("Force Perfect IVs", gym_Settings_Group));
    row1_gym->addWidget(kogre_clan_randomizer[3]);
    connect(kogre_clan_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kogre_clan_randomizer.append(new QCheckBox("Make AI Smart", gym_Settings_Group));
    row1_gym->addWidget(kogre_clan_randomizer[4]);
    connect(kogre_clan_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    gymTrainersLayout->addLayout(row1_gym);

    // -------------- New Row --------------
    kogre_clan_randomizer.append(new QCheckBox("Only Legends", gym_Settings_Group));
    row2_gym->addWidget(kogre_clan_randomizer[5]);
    connect(kogre_clan_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kogre_clan_randomizer.append(new QCheckBox("Only Paradox", gym_Settings_Group));
    row2_gym->addWidget(kogre_clan_randomizer[6]);
    connect(kogre_clan_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kogre_clan_randomizer.append(new QCheckBox("Only Legends and Paradox", gym_Settings_Group));
    row2_gym->addWidget(kogre_clan_randomizer[7]);
    connect(kogre_clan_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    gymTrainersLayout->addLayout(row2_gym);

    // -------------- New Row --------------
    kogre_clan_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", gym_Settings_Group));
    row3_gym->addWidget(kogre_clan_randomizer[8]);
    connect(kogre_clan_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kogre_clan_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", gym_Settings_Group));
    row3_gym->addWidget(kogre_clan_randomizer[9]);
    connect(kogre_clan_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kogre_clan_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", gym_Settings_Group));
    row3_gym->addWidget(kogre_clan_randomizer[10]);
    connect(kogre_clan_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kogre_clan_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", gym_Settings_Group));
    row3_gym->addWidget(kogre_clan_randomizer[11]);
    connect(kogre_clan_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    gymTrainersLayout->addLayout(row3_gym);


    formLayout->addRow(gym_Settings_Group);
    //--------------Gym Settings End--------------
    //--------------E4 Settings Start--------------
    QGroupBox *e4_Settings_Group = new QGroupBox("Raid Trainers Settings Section");
    QPushButton *toggleE4SettingGroup = new QPushButton("Toggle Raid Trainers Settings Section", kitakamiTrainersWidget);
    toggleE4SettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleE4SettingGroup, &QPushButton::clicked, [e4_Settings_Group]() {
        e4_Settings_Group->setVisible(!e4_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleE4SettingGroup);

    QVBoxLayout *e4TrainersLayout = new QVBoxLayout(e4_Settings_Group);
    QHBoxLayout *row0_e4 = new QHBoxLayout();
    QHBoxLayout *row1_e4 = new QHBoxLayout();
    QHBoxLayout *row2_e4 = new QHBoxLayout();
    QHBoxLayout *row3_e4 = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();

    // -------------- New Row --------------
    kraid_randomizer.append(new QCheckBox("Force 6 Pokemon", e4_Settings_Group));
    row0_e4->addWidget(kraid_randomizer[0]);
    connect(kraid_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraid_randomizer.append(new QCheckBox("Give Extra Pokemon", e4_Settings_Group));
    row0_e4->addWidget(kraid_randomizer[1]);
    connect(kraid_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraid_randomizer.append(new QCheckBox("Allow Terastalization", e4_Settings_Group));
    row0_e4->addWidget(kraid_randomizer[2]);
    connect(kraid_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    e4TrainersLayout->addLayout(row0_e4);

    // -------------- New Row --------------
    kraid_randomizer.append(new QCheckBox("Force Perfect IVs", e4_Settings_Group));
    row1_e4->addWidget(kraid_randomizer[3]);
    connect(kraid_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraid_randomizer.append(new QCheckBox("Make AI Smart", e4_Settings_Group));
    row1_e4->addWidget(kraid_randomizer[4]);
    connect(kraid_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    e4TrainersLayout->addLayout(row1_e4);

    // -------------- New Row --------------
    kraid_randomizer.append(new QCheckBox("Only Legends", e4_Settings_Group));
    row2_e4->addWidget(kraid_randomizer[5]);
    connect(kraid_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraid_randomizer.append(new QCheckBox("Only Paradox", e4_Settings_Group));
    row2_e4->addWidget(kraid_randomizer[6]);
    connect(kraid_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraid_randomizer.append(new QCheckBox("Only Legends and Paradox", e4_Settings_Group));
    row2_e4->addWidget(kraid_randomizer[7]);
    connect(kraid_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    e4TrainersLayout->addLayout(row2_e4);

    // -------------- New Row --------------
    kraid_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", e4_Settings_Group));
    row3_e4->addWidget(kraid_randomizer[8]);
    connect(kraid_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraid_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", e4_Settings_Group));
    row3_e4->addWidget(kraid_randomizer[9]);
    connect(kraid_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraid_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", e4_Settings_Group));
    row3_e4->addWidget(kraid_randomizer[10]);
    connect(kraid_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    kraid_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", e4_Settings_Group));
    row3_e4->addWidget(kraid_randomizer[11]);
    connect(kraid_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    e4TrainersLayout->addLayout(row3_e4);


    formLayout->addRow(e4_Settings_Group);
    //--------------E4 Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(kitakamiTrainersWidget);
    return scrollArea;
}

QWidget* SVRandomizerWindow::setupTrainersBerryWidget(){
    QWidget *blueberryTrainersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(blueberryTrainersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    //--------------All Trainers Settings Start--------------
    QGroupBox *allTrainers_Settings_Group = new QGroupBox("General Settings Section");
    QPushButton *toggleAllTrainersSettingGroup = new QPushButton("Toggle General Settings Section", blueberryTrainersWidget);
    toggleAllTrainersSettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleAllTrainersSettingGroup, &QPushButton::clicked, [allTrainers_Settings_Group]() {
        allTrainers_Settings_Group->setVisible(!allTrainers_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleAllTrainersSettingGroup);

    QVBoxLayout *allTrainersLayout = new QVBoxLayout(allTrainers_Settings_Group);
    QHBoxLayout *row0_all = new QHBoxLayout();
    QHBoxLayout *row1_all = new QHBoxLayout();
    QHBoxLayout *row2_all = new QHBoxLayout();
    // QHBoxLayout *row3 = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();
    QHBoxLayout *generation_trainers_header = new QHBoxLayout();
    QHBoxLayout *generation_trainers_selection = new QHBoxLayout();

    // -------------- New Row --------------
    randomize_blueberry_trainers = new QCheckBox("Randomize Trainers", allTrainers_Settings_Group);
    row0_all->addWidget(randomize_blueberry_trainers);
    connect(randomize_blueberry_trainers, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    bglobal_trainer_randomizer_settings.append(new QCheckBox("Allow All Trainers to Tera", allTrainers_Settings_Group));
    row0_all->addWidget(bglobal_trainer_randomizer_settings[0]);
    connect(bglobal_trainer_randomizer_settings[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    bglobal_trainer_randomizer_settings.append(new QCheckBox("Randomize Tera Types", allTrainers_Settings_Group));
    row0_all->addWidget(bglobal_trainer_randomizer_settings[1]);
    connect(bglobal_trainer_randomizer_settings[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    bglobal_trainer_randomizer_settings.append(new QCheckBox("Allow Shiny Pokemon", allTrainers_Settings_Group));
    row0_all->addWidget(bglobal_trainer_randomizer_settings[2]);
    connect(bglobal_trainer_randomizer_settings[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    allTrainersLayout->addLayout(row0_all);

    // -------------- New Row --------------
    bglobal_trainer_randomizer_settings.append(new QCheckBox("Randomnly Choose Single/Double Battles", allTrainers_Settings_Group));
    row1_all->addWidget(bglobal_trainer_randomizer_settings[3]);
    connect(bglobal_trainer_randomizer_settings[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    bglobal_trainer_randomizer_settings.append(new QCheckBox("Only Double Battles", allTrainers_Settings_Group));
    row1_all->addWidget(bglobal_trainer_randomizer_settings[4]);
    connect(bglobal_trainer_randomizer_settings[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    allTrainersLayout->addLayout(row1_all);

    // -------------- New Row --------------
    bglobal_trainer_randomizer_settings.append(new QCheckBox("Use Rival settings for All", allTrainers_Settings_Group));
    row2_all->addWidget(bglobal_trainer_randomizer_settings[5]);
    connect(bglobal_trainer_randomizer_settings[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    allTrainersLayout->addLayout(row2_all);

    // -------------- New Row --------------
    QLabel *startsSectionHeader = new QLabel("Allowed Generations (empty means all are allowed)", allTrainers_Settings_Group);
    startsSectionHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generation_trainers_header->addWidget(startsSectionHeader);

    for(int i =0; i<9; i++){
        btrainersgeneration.append(new QCheckBox(QStringLiteral("%1").arg(i+1), allTrainers_Settings_Group));
        generation_trainers_selection->addWidget(btrainersgeneration[i]);
        connect(btrainersgeneration[i], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    }
    allTrainersLayout->addLayout(generation_trainers_header);
    allTrainersLayout->addLayout(generation_trainers_selection);

    formLayout->addRow(allTrainers_Settings_Group);
    //--------------All Trainers Settings End--------------
    //--------------Rivals Settings Start--------------
    QGroupBox *rivals_Settings_Group = new QGroupBox("Rival Settings Section");
    QPushButton *toggleRivalsSettingGroup = new QPushButton("Toggle Rival Settings Section", blueberryTrainersWidget);
    toggleRivalsSettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleRivalsSettingGroup, &QPushButton::clicked, [rivals_Settings_Group]() {
        rivals_Settings_Group->setVisible(!rivals_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleRivalsSettingGroup);

    QVBoxLayout *rivalsTrainersLayout = new QVBoxLayout(rivals_Settings_Group);
    QHBoxLayout *row0_rivals = new QHBoxLayout();
    QHBoxLayout *row1_rivals = new QHBoxLayout();
    QHBoxLayout *row2_rivals = new QHBoxLayout();
    QHBoxLayout *row3_rivals = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();

    // -------------- New Row --------------
    brival_randomizer.append(new QCheckBox("Force 6 Pokemon", rivals_Settings_Group));
    row0_rivals->addWidget(brival_randomizer[0]);
    connect(brival_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    brival_randomizer.append(new QCheckBox("Give Extra Pokemon", rivals_Settings_Group));
    row0_rivals->addWidget(brival_randomizer[1]);
    connect(brival_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    brival_randomizer.append(new QCheckBox("Allow Terastalization", rivals_Settings_Group));
    row0_rivals->addWidget(brival_randomizer[2]);
    connect(brival_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    rivalsTrainersLayout->addLayout(row0_rivals);

    // -------------- New Row --------------
    brival_randomizer.append(new QCheckBox("Force Perfect IVs", rivals_Settings_Group));
    row1_rivals->addWidget(brival_randomizer[3]);
    connect(brival_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    brival_randomizer.append(new QCheckBox("Make AI Smart", rivals_Settings_Group));
    row1_rivals->addWidget(brival_randomizer[4]);
    connect(brival_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    rivalsTrainersLayout->addLayout(row1_rivals);

    // -------------- New Row --------------
    brival_randomizer.append(new QCheckBox("Only Legends", rivals_Settings_Group));
    row2_rivals->addWidget(brival_randomizer[5]);
    connect(brival_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    brival_randomizer.append(new QCheckBox("Only Paradox", rivals_Settings_Group));
    row2_rivals->addWidget(brival_randomizer[6]);
    connect(brival_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    brival_randomizer.append(new QCheckBox("Only Legends and Paradox", rivals_Settings_Group));
    row2_rivals->addWidget(brival_randomizer[7]);
    connect(brival_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    rivalsTrainersLayout->addLayout(row2_rivals);

    // -------------- New Row --------------
    brival_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", rivals_Settings_Group));
    row3_rivals->addWidget(brival_randomizer[8]);
    connect(brival_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    brival_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", rivals_Settings_Group));
    row3_rivals->addWidget(brival_randomizer[9]);
    connect(brival_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    brival_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", rivals_Settings_Group));
    row3_rivals->addWidget(brival_randomizer[10]);
    connect(brival_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    brival_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", rivals_Settings_Group));
    row3_rivals->addWidget(brival_randomizer[11]);
    connect(brival_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    rivalsTrainersLayout->addLayout(row3_rivals);


    formLayout->addRow(rivals_Settings_Group);
    //--------------Rivals Settings End--------------
    //--------------Route Settings Start--------------
    QGroupBox *route_Settings_Group = new QGroupBox("Routes Settings Section");
    QPushButton *toggleRoutesSettingGroup = new QPushButton("Toggle Routes Settings Section", blueberryTrainersWidget);
    toggleRoutesSettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleRoutesSettingGroup, &QPushButton::clicked, [route_Settings_Group]() {
        route_Settings_Group->setVisible(!route_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleRoutesSettingGroup);

    QVBoxLayout *routesTrainersLayout = new QVBoxLayout(route_Settings_Group);
    QHBoxLayout *row0_routes = new QHBoxLayout();
    QHBoxLayout *row1_routes = new QHBoxLayout();
    QHBoxLayout *row2_routes = new QHBoxLayout();
    QHBoxLayout *row3_routes = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();

    // -------------- New Row --------------
    broute_randomizer.append(new QCheckBox("Force 6 Pokemon", route_Settings_Group));
    row0_routes->addWidget(broute_randomizer[0]);
    connect(broute_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    broute_randomizer.append(new QCheckBox("Give Extra Pokemon", route_Settings_Group));
    row0_routes->addWidget(broute_randomizer[1]);
    connect(broute_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    broute_randomizer.append(new QCheckBox("Allow Terastalization", route_Settings_Group));
    row0_routes->addWidget(broute_randomizer[2]);
    connect(broute_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    routesTrainersLayout->addLayout(row0_routes);

    // -------------- New Row --------------
    broute_randomizer.append(new QCheckBox("Force Perfect IVs", route_Settings_Group));
    row1_routes->addWidget(broute_randomizer[3]);
    connect(broute_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    broute_randomizer.append(new QCheckBox("Make AI Smart", route_Settings_Group));
    row1_routes->addWidget(broute_randomizer[4]);
    connect(broute_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    routesTrainersLayout->addLayout(row1_routes);

    // -------------- New Row --------------
    broute_randomizer.append(new QCheckBox("Only Legends", route_Settings_Group));
    row2_routes->addWidget(broute_randomizer[5]);
    connect(broute_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    broute_randomizer.append(new QCheckBox("Only Paradox", route_Settings_Group));
    row2_routes->addWidget(broute_randomizer[6]);
    connect(broute_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    broute_randomizer.append(new QCheckBox("Only Legends and Paradox", route_Settings_Group));
    row2_routes->addWidget(broute_randomizer[7]);
    connect(broute_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    routesTrainersLayout->addLayout(row2_routes);

    // -------------- New Row --------------
    broute_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", route_Settings_Group));
    row3_routes->addWidget(broute_randomizer[8]);
    connect(broute_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    broute_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", route_Settings_Group));
    row3_routes->addWidget(broute_randomizer[9]);
    connect(broute_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    broute_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", route_Settings_Group));
    row3_routes->addWidget(broute_randomizer[10]);
    connect(broute_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    broute_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", route_Settings_Group));
    row3_routes->addWidget(broute_randomizer[11]);
    connect(broute_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    routesTrainersLayout->addLayout(row3_routes);


    formLayout->addRow(route_Settings_Group);
    //--------------Route Settings End--------------
    //--------------Gym Settings Start--------------
    QGroupBox *gym_Settings_Group = new QGroupBox("BBClub Settings Section");
    QPushButton *toggleGymsSettingGroup = new QPushButton("Toggle BBClub Settings Section", blueberryTrainersWidget);
    toggleGymsSettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleGymsSettingGroup, &QPushButton::clicked, [gym_Settings_Group]() {
        gym_Settings_Group->setVisible(!gym_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleGymsSettingGroup);

    QVBoxLayout *gymTrainersLayout = new QVBoxLayout(gym_Settings_Group);
    QHBoxLayout *row0_gym = new QHBoxLayout();
    QHBoxLayout *row1_gym = new QHBoxLayout();
    QHBoxLayout *row2_gym = new QHBoxLayout();
    QHBoxLayout *row3_gym = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();

    // -------------- New Row --------------
    b_bb4_randomizer.append(new QCheckBox("Force 6 Pokemon", gym_Settings_Group));
    row0_gym->addWidget(b_bb4_randomizer[0]);
    connect(b_bb4_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    b_bb4_randomizer.append(new QCheckBox("Give Extra Pokemon", gym_Settings_Group));
    row0_gym->addWidget(b_bb4_randomizer[1]);
    connect(b_bb4_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    b_bb4_randomizer.append(new QCheckBox("Allow Terastalization", gym_Settings_Group));
    row0_gym->addWidget(b_bb4_randomizer[2]);
    connect(b_bb4_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    gymTrainersLayout->addLayout(row0_gym);

    // -------------- New Row --------------
    b_bb4_randomizer.append(new QCheckBox("Force Perfect IVs", gym_Settings_Group));
    row1_gym->addWidget(b_bb4_randomizer[3]);
    connect(b_bb4_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    b_bb4_randomizer.append(new QCheckBox("Make AI Smart", gym_Settings_Group));
    row1_gym->addWidget(b_bb4_randomizer[4]);
    connect(b_bb4_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    gymTrainersLayout->addLayout(row1_gym);

    // -------------- New Row --------------
    b_bb4_randomizer.append(new QCheckBox("Only Legends", gym_Settings_Group));
    row2_gym->addWidget(b_bb4_randomizer[5]);
    connect(b_bb4_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    b_bb4_randomizer.append(new QCheckBox("Only Paradox", gym_Settings_Group));
    row2_gym->addWidget(b_bb4_randomizer[6]);
    connect(b_bb4_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    b_bb4_randomizer.append(new QCheckBox("Only Legends and Paradox", gym_Settings_Group));
    row2_gym->addWidget(b_bb4_randomizer[7]);
    connect(b_bb4_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    gymTrainersLayout->addLayout(row2_gym);

    // -------------- New Row --------------
    b_bb4_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", gym_Settings_Group));
    row3_gym->addWidget(b_bb4_randomizer[8]);
    connect(b_bb4_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    b_bb4_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", gym_Settings_Group));
    row3_gym->addWidget(b_bb4_randomizer[9]);
    connect(b_bb4_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    b_bb4_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", gym_Settings_Group));
    row3_gym->addWidget(b_bb4_randomizer[10]);
    connect(b_bb4_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    b_bb4_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", gym_Settings_Group));
    row3_gym->addWidget(b_bb4_randomizer[11]);
    connect(b_bb4_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    gymTrainersLayout->addLayout(row3_gym);


    formLayout->addRow(gym_Settings_Group);
    //--------------Gym Settings End--------------
    //--------------E4 Settings Start--------------
    QGroupBox *e4_Settings_Group = new QGroupBox("Raid Trainers Settings Section");
    QPushButton *toggleE4SettingGroup = new QPushButton("Toggle Raid Trainers Settings Section", blueberryTrainersWidget);
    toggleE4SettingGroup->setStyleSheet(
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 10px;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #1c6692;"
        "}"
        );

    // Connect to Hide the section if needed
    connect(toggleE4SettingGroup, &QPushButton::clicked, [e4_Settings_Group]() {
        e4_Settings_Group->setVisible(!e4_Settings_Group->isVisible());
    });

    formLayout->addRow(toggleE4SettingGroup);

    QVBoxLayout *e4TrainersLayout = new QVBoxLayout(e4_Settings_Group);
    QHBoxLayout *row0_e4 = new QHBoxLayout();
    QHBoxLayout *row1_e4 = new QHBoxLayout();
    QHBoxLayout *row2_e4 = new QHBoxLayout();
    QHBoxLayout *row3_e4 = new QHBoxLayout();
    // QHBoxLayout *row4 = new QHBoxLayout();

    // -------------- New Row --------------
    braid_randomizer.append(new QCheckBox("Force 6 Pokemon", e4_Settings_Group));
    row0_e4->addWidget(braid_randomizer[0]);
    connect(braid_randomizer[0], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braid_randomizer.append(new QCheckBox("Give Extra Pokemon", e4_Settings_Group));
    row0_e4->addWidget(braid_randomizer[1]);
    connect(braid_randomizer[1], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braid_randomizer.append(new QCheckBox("Allow Terastalization", e4_Settings_Group));
    row0_e4->addWidget(braid_randomizer[2]);
    connect(braid_randomizer[2], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    e4TrainersLayout->addLayout(row0_e4);

    // -------------- New Row --------------
    braid_randomizer.append(new QCheckBox("Force Perfect IVs", e4_Settings_Group));
    row1_e4->addWidget(braid_randomizer[3]);
    connect(braid_randomizer[3], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braid_randomizer.append(new QCheckBox("Make AI Smart", e4_Settings_Group));
    row1_e4->addWidget(braid_randomizer[4]);
    connect(braid_randomizer[4], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);


    e4TrainersLayout->addLayout(row1_e4);

    // -------------- New Row --------------
    braid_randomizer.append(new QCheckBox("Only Legends", e4_Settings_Group));
    row2_e4->addWidget(braid_randomizer[5]);
    connect(braid_randomizer[5], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braid_randomizer.append(new QCheckBox("Only Paradox", e4_Settings_Group));
    row2_e4->addWidget(braid_randomizer[6]);
    connect(braid_randomizer[6], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braid_randomizer.append(new QCheckBox("Only Legends and Paradox", e4_Settings_Group));
    row2_e4->addWidget(braid_randomizer[7]);
    connect(braid_randomizer[7], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    e4TrainersLayout->addLayout(row2_e4);

    // -------------- New Row --------------
    braid_randomizer.append(new QCheckBox("Ban Stage 1 Pokemon", e4_Settings_Group));
    row3_e4->addWidget(braid_randomizer[8]);
    connect(braid_randomizer[8], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braid_randomizer.append(new QCheckBox("Ban Stage 2 Pokemon", e4_Settings_Group));
    row3_e4->addWidget(braid_randomizer[9]);
    connect(braid_randomizer[9], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braid_randomizer.append(new QCheckBox("Ban Stage 3 Pokemon", e4_Settings_Group));
    row3_e4->addWidget(braid_randomizer[10]);
    connect(braid_randomizer[10], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    braid_randomizer.append(new QCheckBox("Ban 1 Stage Pokemon", e4_Settings_Group));
    row3_e4->addWidget(braid_randomizer[11]);
    connect(braid_randomizer[11], &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    e4TrainersLayout->addLayout(row3_e4);


    formLayout->addRow(e4_Settings_Group);
    //--------------E4 Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(blueberryTrainersWidget);
    return scrollArea;
}

void SVRandomizerWindow::showRestartWidget() {
    QWidget *restartWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(restartWidget);

    QLabel *label = new QLabel("Program has finished running.\nRestarting for next use...");
    layout->addWidget(label);

    QPushButton *restartButton = new QPushButton("Restart Now");
    layout->addWidget(restartButton);

    QObject::connect(restartButton, &QPushButton::clicked, [=]() {
        // Restart the application
        QString program = QCoreApplication::applicationFilePath();
        QProcess::startDetached(program, QStringList());
        QApplication::exit(); // Exit the current instance
    });

    restartWidget->show();
}

bool SVRandomizerWindow::checkAndDeleteFile(std::string filePath) {
    try {
        // Check if the file exists
        if (fs::exists(filePath)) {
            // Delete the file
            if (fs::remove(filePath)) {
                qDebug() << "File deleted successfully: " << filePath;
                return true; // File existed and was deleted
            } else {
                std::cerr << "Failed to delete the file: " << filePath << std::endl;
                return false;
            }
        } else {
            qDebug()  << "File does not exist: " << filePath;
            return false; // File does not exist
        }
    } catch (const fs::filesystem_error& e) {
        // Handle filesystem-related exceptions
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        // Handle other exceptions
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

void SVRandomizerWindow::addToFavorites()
{
    for(int i = 1; i< randomizer.bulk_amount+1; i++){
        std::string outputKey = "output";

        std::string dirPath = fs::absolute(outputKey).string();
        if(fs::exists(dirPath)){
            fs::remove_all(dirPath);
        }

        randomizer.createFolderHierarchy(outputKey+"/romfs");

        std::string baseAddress = fs::absolute("SV_FLATBUFFERS").string();
        QString QBaseAddress = QString::fromStdString(baseAddress);
        QDir qBaseDir(QBaseAddress);

        int hash = 0;
        if(!randomizer.seed.isEmpty()){
            for (const QChar &ch : randomizer.seed) {
                hash = hash * 31 + ch.unicode(); // Accumulate a numeric value
            }
        }

        if(randomizer.svRandomizerStarters.enable_starters == true){
            randomizer.svRandomizeStarters(randomizer.svRandomizerStarters, hash, i);

            // Set up Scenes for Starters
            try {
                randomizer.createFolderHierarchy(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/");
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_always_0.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_always_0.trsog").string());
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_always_1.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_always_1.trsog").string());
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_main_0.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_main_0.trsog").string());
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_main_1.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_main_1.trsog").string());

                randomizer.createFolderHierarchy(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0070_/");
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0070_always_0.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0070_/common_0070_always_0.trsog").string());
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0070_always_1.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0070_/common_0070_always_1.trsog").string());


                randomizer.createFolderHierarchy(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0088_/");
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0088_always_0.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0088_/common_0088_always_0.trsog").string());
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0088_always_1.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0088_/common_0088_always_1.trsog").string());

                randomizer.createFolderHierarchy(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0090_/");
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0090_main_0.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0090_/common_0090_main_0.trsog").string());
                fs::copy(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0090_main_1.trsog").toStdString(),
                         fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0090_/common_0090_main_1.trsog").string());

            } catch (const fs::filesystem_error& e) {
                qDebug() << "Error copying file: " << e.what();
            }

        }

        if(randomizer.svRandomizerStarters.enable_gifts == true){
            randomizer.svRandomizeGifts(randomizer.svRandomizerStarters, hash, i);
        }

        if(randomizer.svRandomizerStarters.enable_starters == true || randomizer.svRandomizerStarters.enable_gifts == true){
            randomizer.generateBinary(qBaseDir.filePath("SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.bfbs").toStdString(),
                                      qBaseDir.filePath(+"SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.json").toStdString(),
                                      "world/data/event/event_add_pokemon/eventAddPokemon/", true);
        }

        // Stats Changes
        if(randomizer.svRandomizerStats.randomize_abilities == true || randomizer.svRandomizerStats.randomize_types == true
            ||randomizer.svRandomizerStats.randomize_moveset == true || randomizer.svRandomizerStats.randomize_bst == true
            || randomizer.svRandomizerStats.randomize_evolutions == true || randomizer.svRandomizerStats.enable_TMs == true){

            randomizer.svRandomizeStats(randomizer.svRandomizerStats, hash, i);

            randomizer.generateBinary(qBaseDir.filePath("SV_PERSONAL/personal_array.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_PERSONAL/personal_array.json").toStdString(),
                                      "avalon/data/", true);

            if(randomizer.svRandomizerStats.enable_TMs == true){
                randomizer.generateBinary(qBaseDir.filePath("SV_PERSONAL/itemdata_array.bfbs").toStdString(),
                                          qBaseDir.filePath(+"SV_PERSONAL/itemdata_array.json").toStdString(),
                                          "world/data/item/itemdata/", true);
            }

            if(randomizer.svRandomizerStats.allow_moves_without_animation == true){
                randomizer.generateBinary(qBaseDir.filePath("SV_PERSONAL/Waza.fbs").toStdString(),
                                          qBaseDir.filePath(+"SV_PERSONAL/waza_array.json").toStdString(),
                                          "avalon/data/", true);
            }
        }


        // Items Changes
        if(randomizer.svRandomizerItems.randomize_ball_items == true || randomizer.svRandomizerItems.randomize_hidden_items == true
            || randomizer.svRandomizerItems.randomize_pickup_items == true || randomizer.svRandomizerItems.randomize_synchro_items == true
            || randomizer.svRandomizerItems.randomize_pokemon_drops == true){

            randomizer.svRandomizeItem(randomizer.svRandomizerItems, hash, i);

            if(randomizer.svRandomizerItems.randomize_hidden_items == true){
                randomizer.generateBinary(qBaseDir.filePath("SV_ITEMS/hiddenItemDataTable_array.bfbs").toStdString(),
                                          qBaseDir.filePath(+"SV_ITEMS/hiddenItemDataTable_array.json").toStdString(),
                                          "world/data/item/hiddenItemDataTable/", true);

                randomizer.generateBinary(qBaseDir.filePath("SV_ITEMS/hiddenItemDataTable_su1_array.bfbs").toStdString(),
                                          qBaseDir.filePath(+"SV_ITEMS/hiddenItemDataTable_su1_array.json").toStdString(),
                                          "world/data/item/hiddenItemDataTable_su1/", true);

                randomizer.generateBinary(qBaseDir.filePath("SV_ITEMS/hiddenItemDataTable_su2_array.bfbs").toStdString(),
                                          qBaseDir.filePath(+"SV_ITEMS/hiddenItemDataTable_su2_array.json").toStdString(),
                                          "world/data/item/hiddenItemDataTable_su2/", true);

                randomizer.generateBinary(qBaseDir.filePath("SV_ITEMS/hiddenItemDataTable_lc_array.bfbs").toStdString(),
                                          qBaseDir.filePath(+"SV_ITEMS/hiddenItemDataTable_lc_array.json").toStdString(),
                                          "world/data/item/hiddenItemDataTable_lc/", true);
            }

            if(randomizer.svRandomizerItems.randomize_pickup_items == true){
                randomizer.generateBinary(qBaseDir.filePath("SV_ITEMS/monohiroiItemData_array.bfbs").toStdString(),
                                          qBaseDir.filePath(+"SV_ITEMS/monohiroiItemData_array.json").toStdString(),
                                          "world/data/item/monohiroilItemData/", true);
            }

            if(randomizer.svRandomizerItems.randomize_synchro_items == true){
                randomizer.generateBinary(qBaseDir.filePath("SV_ITEMS/rummagingItemDataTable_array.bfbs").toStdString(),
                                          qBaseDir.filePath(+"SV_ITEMS/rummagingItemDataTable_array.json").toStdString(),
                                          "world/data/item/rummagingItemDataTable/", true);
            }

            if(randomizer.svRandomizerItems.randomize_pokemon_drops == true){
                randomizer.generateBinary(qBaseDir.filePath("SV_ITEMS/dropitemdata_array.bfbs").toStdString(),
                                          qBaseDir.filePath(+"SV_ITEMS/dropitemdata_array.json").toStdString(),
                                          "world/data/item/dropitemdata/", true);
            }
        }

        // Wild Areas Save
        if(randomizer.svRandomizerWilds.randomize_paldea_wild == true || randomizer.svRandomizerWilds.randomize_kitakami_wild == true
            || randomizer.svRandomizerWilds.randomize_blueberry_wild == true){

            randomizer.svRandomizeWilds(randomizer.svRandomizerWilds, hash, i);

            if(randomizer.svRandomizerWilds.paldea_Settings_for_all_wild == true){
                if(randomizer.svRandomizerWilds.randomize_paldea_wild == true){
                    randomizer.generateBinary(qBaseDir.filePath("SV_WILDS/pokedata_array.bfbs").toStdString(),
                                              qBaseDir.filePath(+"SV_WILDS/pokedata_array.json").toStdString(),
                                              "world/data/encount/pokedata/pokedata/", true);

                    randomizer.generateBinary(qBaseDir.filePath("SV_WILDS/fixed_symbol_table_array.bfbs").toStdString(),
                                              qBaseDir.filePath(+"SV_WILDS/fixed_symbol_table_array.json").toStdString(),
                                              "world/data/field/fixed_symbol/fixed_symbol_table/", true);

                    randomizer.generateBinary(qBaseDir.filePath("SV_WILDS/pokedata_su1_array.bfbs").toStdString(),
                                              qBaseDir.filePath(+"SV_WILDS/pokedata_su1_array.json").toStdString(),
                                              "world/data/encount/pokedata/pokedata_su1/", true);

                    randomizer.generateBinary(qBaseDir.filePath("SV_WILDS/pokedata_su2_array.bfbs").toStdString(),
                                              qBaseDir.filePath(+"SV_WILDS/pokedata_su2_array.json").toStdString(),
                                              "world/data/encount/pokedata/pokedata_su2/", true);
                }
            }else{
                if(randomizer.svRandomizerWilds.randomize_paldea_wild == true){
                    randomizer.generateBinary(qBaseDir.filePath("SV_WILDS/pokedata_array.bfbs").toStdString(),
                                              qBaseDir.filePath(+"SV_WILDS/pokedata_array.json").toStdString(),
                                              "world/data/encount/pokedata/pokedata/", true);

                    randomizer.generateBinary(qBaseDir.filePath("SV_WILDS/fixed_symbol_table_array.bfbs").toStdString(),
                                              qBaseDir.filePath(+"SV_WILDS/fixed_symbol_table_array.json").toStdString(),
                                              "world/data/field/fixed_symbol/fixed_symbol_table/", true);
                }
                if(randomizer.svRandomizerWilds.randomize_kitakami_wild == true){
                    randomizer.generateBinary(qBaseDir.filePath("SV_WILDS/pokedata_su1_array.bfbs").toStdString(),
                                              qBaseDir.filePath(+"SV_WILDS/pokedata_su1_array.json").toStdString(),
                                              "world/data/encount/pokedata/pokedata_su1/", true);
                }
                if(randomizer.svRandomizerWilds.randomize_blueberry_wild == true){
                    randomizer.generateBinary(qBaseDir.filePath("SV_WILDS/pokedata_su2_array.bfbs").toStdString(),
                                              qBaseDir.filePath(+"SV_WILDS/pokedata_su2_array.json").toStdString(),
                                              "world/data/encount/pokedata/pokedata_su2/", true);
                }
            }
        }

        // Raids Save
        if(randomizer.svRandomizerRaids.praids_randomize == true || randomizer.svRandomizerRaids.kraids_randomize == true
                || randomizer.svRandomizerRaids.braids_randomize == true){

            randomizer.svRandomizeRaids(randomizer.svRandomizerRaids, hash, i);

            if(randomizer.svRandomizerRaids.paldea_Settings_for_all_raids == true){
                for(int j = 1; j<=6; j++){
                    std::string ogName = "raid_enemy_0" + std::to_string(j) + "_array.json";
                    std::string bfbsName = "raid_enemy_0" + std::to_string(j) + "_array.bfbs";

                    randomizer.generateBinary(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(bfbsName)).toStdString(),
                                              qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(ogName)).toStdString(),
                                              "world/data/raid/raid_enemy_0"+std::to_string(j)+"/", true);
                }
                for(int j = 1; j<=6; j++){
                    std::string ogName = "su1_raid_enemy_0" + std::to_string(j) + "_array.json";
                    std::string bfbsName = "su1_raid_enemy_0" + std::to_string(j) + "_array.bfbs";

                    randomizer.generateBinary(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(bfbsName)).toStdString(),
                                              qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(ogName)).toStdString(),
                                              "world/data/raid/su1_raid_enemy_0"+std::to_string(j)+"/", true);
                }
                for(int j = 1; j<=6; j++){
                    std::string ogName = "su2_raid_enemy_0" + std::to_string(j) + "_array.json";
                    std::string bfbsName = "su2_raid_enemy_0" + std::to_string(j) + "_array.bfbs";

                    randomizer.generateBinary(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(bfbsName)).toStdString(),
                                              qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(ogName)).toStdString(),
                                              "world/data/raid/su2_raid_enemy_0"+std::to_string(j)+"/", true);
                }
            }else{
                if(randomizer.svRandomizerRaids.praids_randomize == true){
                    for(int j = 1; j<=6; j++){
                        std::string ogName = "raid_enemy_0" + std::to_string(j) + "_array.json";
                        std::string bfbsName = "raid_enemy_0" + std::to_string(j) + "_array.bfbs";

                        randomizer.generateBinary(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(bfbsName)).toStdString(),
                                                  qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(ogName)).toStdString(),
                                                  "world/data/raid/raid_enemy_0"+std::to_string(j)+"/", true);
                    }
                }

                if(randomizer.svRandomizerRaids.kraids_randomize == true){
                    for(int j = 1; j<=6; j++){
                        std::string ogName = "su1_raid_enemy_0" + std::to_string(j) + "_array.json";
                        std::string bfbsName = "su1_raid_enemy_0" + std::to_string(j) + "_array.bfbs";

                        randomizer.generateBinary(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(bfbsName)).toStdString(),
                                                  qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(ogName)).toStdString(),
                                                  "world/data/raid/su1_raid_enemy_0"+std::to_string(j)+"/", true);
                    }
                }

                if(randomizer.svRandomizerRaids.braids_randomize == true){
                    for(int j = 1; j<=6; j++){
                        std::string ogName = "su2_raid_enemy_0" + std::to_string(j) + "_array.json";
                        std::string bfbsName = "su2_raid_enemy_0" + std::to_string(j) + "_array.bfbs";

                        randomizer.generateBinary(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(bfbsName)).toStdString(),
                                                  qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(ogName)).toStdString(),
                                                  "world/data/raid/su2_raid_enemy_0"+std::to_string(j)+"/", true);
                    }
                }
            }

            // Fixed and Lottery

        }

        // Trainers Save
        if(randomizer.svRandomizerTrainers.randomize_paldea_trainers == true || randomizer.svRandomizerTrainers.randomize_kitakami_trainers == true
                || randomizer.svRandomizerTrainers.randomize_blueberry_trainers == true){
            randomizer.svRandomizeTrainers(randomizer.svRandomizerTrainers, hash, i);
            qDebug()<<"Made it here";
            randomizer.generateBinary(qBaseDir.filePath("SV_TRAINERS/trdata_array.bfbs").toStdString(),
                                      qBaseDir.filePath(+"SV_TRAINERS/trdata_array.json").toStdString(),
                                      "world/data/trainer/trdata/", true);
        }

        if(randomizer.auto_patch == true){
            randomizer.patchFileDescriptor();
            randomizer.generateBinary(qBaseDir.filePath("SV_DATA_FLATBUFFERS/data.fbs").toStdString(),
                                      qBaseDir.filePath("SV_DATA_FLATBUFFERS/data.json").toStdString(),
                                      "arc/", true);
        }

        try {
            std::string copiedDir = "Randomizer-" + std::to_string(i);
            if(fs::exists(copiedDir)){
                fs::remove_all(copiedDir);
            }

            fs::copy(fs::absolute(outputKey).string(), fs::absolute(copiedDir).string(), fs::copy_options::recursive);
            qDebug() << "Directory copied successfully.";
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error copying directory: " << e.what() << std::endl;
        }

        // Delete files for next generation
            for(int j = 1; j<=6; j++){
                std::string ogName = "su2_raid_enemy_0" + std::to_string(j) + "_array.json";

                checkAndDeleteFile(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(ogName)).toStdString());
            }
            for(int j = 1; j<=6; j++){
                std::string ogName = "su1_raid_enemy_0" + std::to_string(j) + "_array.json";

                checkAndDeleteFile(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(ogName)).toStdString());
            }
            for(int j = 1; j<=6; j++){
                std::string ogName = "raid_enemy_0" + std::to_string(j) + "_array.json";

                checkAndDeleteFile(qBaseDir.filePath("SV_RAIDS/"+QString::fromStdString(ogName)).toStdString());
            }
            checkAndDeleteFile(qBaseDir.filePath("SV_DATA_FLATBUFFERS/data.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_WILDS/pokedata_su2_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_WILDS/pokedata_su1_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_WILDS/fixed_symbol_table_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_WILDS/pokedata_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_ITEMS/dropitemdata_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_ITEMS/rummagingItemDataTable_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_ITEMS/monohiroiItemData_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_ITEMS/hiddenItemDataTable_lc_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_ITEMS/hiddenItemDataTable_su2_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_ITEMS/hiddenItemDataTable_su1_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_ITEMS/hiddenItemDataTable_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_PERSONAL/waza_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_PERSONAL/itemdata_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_PERSONAL/personal_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath(+"SV_STARTERS_FLATBUFFERS/eventAddPokemon_array.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0090_main_1.trsog").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0090_main_0.trsog").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0088_always_1.trsog").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0088_always_0.trsog").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0070_always_1.trsog").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0070_always_0.trsog").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_main_1.trsog").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_main_0.trsog").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_always_1.trsog").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_always_0.trsog").toStdString());

        // Delete Output file (again)
        if(fs::exists(dirPath)){
            fs::remove_all(dirPath);
        }
    }
}

void SVRandomizerWindow::switchTabs(int index) {
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
    } else {
        qWarning() << "Invalid tab index:" << index;
    }
}

void SVRandomizerWindow::switchInnerWildTabs(int index) {
    if (index >= 0 && index < stackedWidget->count()) {
        innerWildStackWidget->setCurrentIndex(index);
    } else {
        qWarning() << "Invalid tab index:" << index;
    }
}

void SVRandomizerWindow::switchInnerRaidTabs(int index) {
    if (index >= 0 && index < stackedWidget->count()) {
        innerRaidStackWidget->setCurrentIndex(index);
    } else {
        qWarning() << "Invalid tab index:" << index;
    }
}

void SVRandomizerWindow::switchInnerTrainerTabs(int index) {
    if (index >= 0 && index < stackedWidget->count()) {
        innerTrainerStackWidget->setCurrentIndex(index);
    } else {
        qWarning() << "Invalid tab index:" << index;
    }
}

// Work on saves after all Tab UIs are done
void SVRandomizerWindow::saveSpinBoxValue() {
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(sender());
    if (!spinBox) return;  // Exit if sender is not a QSpinBox

    if (spinBox == shiny_starter_rate) {
        randomizer.svRandomizerStarters.shiny_starter_rate = shiny_starter_rate->value();
    } else if (spinBox == shiny_static_rate) {
        randomizer.svRandomizerStarters.shiny_starter_rate = shiny_static_rate->value();
    }else if (spinBox == bulk_amount) {
        randomizer.bulk_amount = bulk_amount->value();
    }else if (spinBox == praids_shiny_chance) {
        randomizer.svRandomizerRaids.praids_shiny_chance = praids_shiny_chance->value();
    }else if (spinBox == kraids_shiny_chance) {
        randomizer.svRandomizerRaids.kraids_shiny_chance = kraids_shiny_chance->value();
    } else if (spinBox == braids_shiny_chance) {
        randomizer.svRandomizerRaids.braids_shiny_chance = braids_shiny_chance->value();
    }
}

void SVRandomizerWindow::saveComboInput() {
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if(!comboBox)
        return;

    if(comboBox == nullptr){

    }else{
        int index = 0;
        index = starters_form.indexOf(comboBox);
        if(index !=-1){
            int setForm = 0;
            for(int i =0; i< randomizer.pokemonFormsInGame[randomizer.svRandomizerStarters.starters[index]].size(); i++){
                if(comboBox->currentText() == randomizer.pokemonFormsInGame[randomizer.svRandomizerStarters.starters[index]][i]){
                    setForm = i;
                }
            }

            randomizer.svRandomizerStarters.starters_forms[index] = setForm;
            return;
        }

        index = starters_gender.indexOf(comboBox);
        if(index !=-1){
            randomizer.svRandomizerStarters.starters_gender[index] = comboBox->currentText();
            return;
        }

        index = starters_pokeball.indexOf(comboBox);
        if(index !=-1){
            randomizer.svRandomizerStarters.starters_pokeball[index] = comboBox->currentText();
            return;
        }
    }
}

void SVRandomizerWindow::saveCheckboxState() {
    QCheckBox *checkBox = qobject_cast<QCheckBox *>(sender());
    if (!checkBox) return;  // Exit if sender is not a QCheckBox

    // Check if it was the single checkbox
    if (checkBox == auto_patch) {
        randomizer.auto_patch = auto_patch->isChecked();
        //qDebug() << "Saving state from auto_patch:" << (randomizer.auto_patch ? "Checked" : "Unchecked");
    }
    else if (checkBox == kaizo_mode) {
        randomizer.kaizo_mode = kaizo_mode->isChecked();
    }
    else if(checkBox == force_shiny_starter){
        randomizer.svRandomizerStarters.force_shiny_starter = force_shiny_starter->isChecked();
    }
    else if(checkBox == all_starters_shiny){
        randomizer.svRandomizerStarters.all_starters_shiny = all_starters_shiny->isChecked();
    }
    else if(checkBox == show_starters_in_overworld){
        randomizer.svRandomizerStarters.show_starters_in_overworld = show_starters_in_overworld->isChecked();
    }
    else if(checkBox == randomize_starters_tera_types){
        randomizer.svRandomizerStarters.randomize_starters_tera_types = randomize_starters_tera_types->isChecked();
    }
    else if(checkBox == ban_stage_1_pokemon_starters){
        randomizer.svRandomizerStarters.ban_stage_1_pokemon_starters = ban_stage_1_pokemon_starters->isChecked();
    }
    else if(checkBox == ban_stage_2_pokemon_starters){
        randomizer.svRandomizerStarters.ban_stage_2_pokemon_starters = ban_stage_2_pokemon_starters->isChecked();
    }
    else if(checkBox == ban_stage_3_pokemon_starters){
        randomizer.svRandomizerStarters.ban_stage_3_pokemon_starters = ban_stage_3_pokemon_starters->isChecked();
    }
    else if(checkBox == ban_single_stage_pokemon_starters){
        randomizer.svRandomizerStarters.ban_single_stage_pokemon_starters = ban_single_stage_pokemon_starters->isChecked();
    }
    else if(checkBox == only_legendary_pokemon_starters){
        randomizer.svRandomizerStarters.only_legendary_pokemon_starters = only_legendary_pokemon_starters->isChecked();
    }
    else if(checkBox == only_paradox_pokemon_starters){
        randomizer.svRandomizerStarters.only_paradox_pokemon_starters = only_paradox_pokemon_starters->isChecked();
    }
    else if(checkBox == only_legends_and_paradox_starters){
        randomizer.svRandomizerStarters.only_legends_and_paradox_starters = only_legends_and_paradox_starters->isChecked();
    }
    else if(checkBox == randomize_starters_tera_types){
        randomizer.svRandomizerStarters.randomize_starters_tera_types = randomize_starters_tera_types->isChecked();
    }
    else if(checkBox == enable_gifts){
        randomizer.svRandomizerStarters.enable_gifts = enable_gifts->isChecked();
    }
    else if(checkBox == enable_starters){
        randomizer.svRandomizerStarters.enable_starters = enable_starters->isChecked();
    }
    else if(checkBox == randomize_tera_types){
        randomizer.svRandomizerStarters.randomize_tera_types = randomize_tera_types->isChecked();
    } // Stats
    else if(checkBox == randomize_abilities){
        randomizer.svRandomizerStats.randomize_abilities = randomize_abilities->isChecked();
    }
    else if(checkBox == randomize_types){
        randomizer.svRandomizerStats.randomize_types = randomize_types->isChecked();
    }
    else if(checkBox == give_extra_types){
        randomizer.svRandomizerStats.give_extra_types = give_extra_types->isChecked();
    }
    else if(checkBox == ban_wonder_guard){
        randomizer.svRandomizerStats.ban_wonder_guard = ban_wonder_guard->isChecked();
    }
    else if(checkBox == randomize_moveset){
        randomizer.svRandomizerStats.randomize_moveset = randomize_moveset->isChecked();
    }
    else if(checkBox == moveset_same_as_type){
        randomizer.svRandomizerStats.moveset_same_as_type = moveset_same_as_type->isChecked();
    }
    else if(checkBox == randomize_bst){
        randomizer.svRandomizerStats.randomize_bst = randomize_bst->isChecked();
    }
    else if(checkBox == keep_same_bst){
        randomizer.svRandomizerStats.keep_same_bst = keep_same_bst->isChecked();
    }
    else if(checkBox == randomize_evolutions){
        randomizer.svRandomizerStats.randomize_evolutions = randomize_evolutions->isChecked();
    }
    else if(checkBox == force_unobtainable_evolutions_at_night){
        randomizer.svRandomizerStats.force_unobtainable_evolutions_at_night = force_unobtainable_evolutions_at_night->isChecked();
    }
    else if(checkBox == evolve_every_5_levels){
        randomizer.svRandomizerStats.evolve_every_5_levels = evolve_every_5_levels->isChecked();
    }
    else if(checkBox == force_tera_blast_every_5_levels){
        randomizer.svRandomizerStats.force_tera_blast_every_5_levels = force_tera_blast_every_5_levels->isChecked();
    }
    else if(checkBox == enable_TMs){
        randomizer.svRandomizerStats.enable_TMs = enable_TMs->isChecked();
        qDebug()<<"Enable TMs: "<<enable_TMs->isChecked();
    }
    else if(checkBox == randomize_tms){
        randomizer.svRandomizerStats.randomize_tms = randomize_tms->isChecked();
    }
    else if(checkBox == randomize_pokemon_tms){
        randomizer.svRandomizerStats.randomize_pokemon_tms = randomize_pokemon_tms->isChecked();
    }
    else if(checkBox == allow_moves_without_animation){
        randomizer.svRandomizerStats.allow_moves_without_animation = allow_moves_without_animation->isChecked();
        qDebug()<<"All TMs: "<<allow_moves_without_animation->isChecked();
    } // Items
    else if(checkBox == randomize_hidden_items){
        randomizer.svRandomizerItems.randomize_hidden_items = randomize_hidden_items->isChecked();
    }
    else if(checkBox == randomize_ball_items){
        randomizer.svRandomizerItems.randomize_ball_items = randomize_ball_items->isChecked();
    }
    else if(checkBox == randomize_pickup_items){
        randomizer.svRandomizerItems.randomize_pickup_items = randomize_pickup_items->isChecked();
    }
    else if(checkBox == randomize_pokemon_drops){
        randomizer.svRandomizerItems.randomize_pokemon_drops = randomize_pokemon_drops->isChecked();
    }
    else if(checkBox == randomize_synchro_items){
        randomizer.svRandomizerItems.randomize_synchro_items = randomize_synchro_items->isChecked();
    }// Wilds and Statics
    else if(checkBox == randomize_paldea_wild){
        randomizer.svRandomizerWilds.randomize_paldea_wild = randomize_paldea_wild->isChecked();
    }
    else if(checkBox == paldea_ExcludeLegends){
        randomizer.svRandomizerWilds.paldea_ExcludeLegends = paldea_ExcludeLegends->isChecked();
    }
    else if(checkBox == paldea_OnlyLegends){
        randomizer.svRandomizerWilds.paldea_OnlyLegends = paldea_OnlyLegends->isChecked();
    }
    else if(checkBox == paldea_OnlyParadox){
        randomizer.svRandomizerWilds.paldea_OnlyParadox = paldea_OnlyParadox->isChecked();
    }
    else if(checkBox == paldea_OnlyLegendsAndParadox){
        randomizer.svRandomizerWilds.paldea_OnlyLegendsAndParadox = paldea_OnlyLegendsAndParadox->isChecked();
    }
    else if(checkBox == paldea_BanStage1){
        randomizer.svRandomizerWilds.paldea_BanStage1 = paldea_BanStage1->isChecked();
    }
    else if(checkBox == paldea_BanStage2){
        randomizer.svRandomizerWilds.paldea_BanStage2 = paldea_BanStage2->isChecked();
    }
    else if(checkBox == paldea_BanStage3){
        randomizer.svRandomizerWilds.paldea_BanStage3 = paldea_BanStage3->isChecked();
    }
    else if(checkBox == paldea_Ban1Stage){
        randomizer.svRandomizerWilds.paldea_Ban1Stage = paldea_Ban1Stage->isChecked();
    }
    else if(checkBox == paldea_Settings_for_all_wild){
        randomizer.svRandomizerWilds.paldea_Settings_for_all_wild = paldea_Settings_for_all_wild->isChecked();
    }
    else if(checkBox == paldea_BalanceAreaPerBST){
        randomizer.svRandomizerWilds.paldea_BalanceAreaPerBST = paldea_BalanceAreaPerBST->isChecked();
    }
    //Wilds Kitakami
    else if(checkBox == randomize_kitakami_wild){
        randomizer.svRandomizerWilds.randomize_kitakami_wild = randomize_kitakami_wild->isChecked();
    }
    else if(checkBox == kitakami_ExcludeLegends){
        randomizer.svRandomizerWilds.kitakami_ExcludeLegends = kitakami_ExcludeLegends->isChecked();
    }
    else if(checkBox == kitakami_OnlyLegends){
        randomizer.svRandomizerWilds.kitakami_OnlyLegends = kitakami_OnlyLegends->isChecked();
    }
    else if(checkBox == kitakami_OnlyParadox){
        randomizer.svRandomizerWilds.kitakami_OnlyParadox = kitakami_OnlyParadox->isChecked();
    }
    else if(checkBox == kitakami_OnlyLegendsAndParadox){
        randomizer.svRandomizerWilds.kitakami_OnlyLegendsAndParadox = kitakami_OnlyLegendsAndParadox->isChecked();
    }
    else if(checkBox == kitakami_BanStage1){
        randomizer.svRandomizerWilds.kitakami_BanStage1 = kitakami_BanStage1->isChecked();
    }
    else if(checkBox == kitakami_BanStage2){
        randomizer.svRandomizerWilds.kitakami_BanStage2 = kitakami_BanStage2->isChecked();
    }
    else if(checkBox == kitakami_BanStage3){
        randomizer.svRandomizerWilds.kitakami_BanStage3 = kitakami_BanStage3->isChecked();
    }
    else if(checkBox == kitakami_Ban1Stage){
        randomizer.svRandomizerWilds.kitakami_Ban1Stage = kitakami_Ban1Stage->isChecked();
    }
    else if(checkBox == kitakami_BalanceAreaPerBST){
        randomizer.svRandomizerWilds.kitakami_BalanceAreaPerBST = kitakami_BalanceAreaPerBST->isChecked();
    }//Wilds Blueberry
    else if(checkBox == randomize_blueberry_wild){
        randomizer.svRandomizerWilds.randomize_blueberry_wild = randomize_blueberry_wild->isChecked();
    }
    else if(checkBox == blueberry_ExcludeLegends){
        randomizer.svRandomizerWilds.blueberry_ExcludeLegends = blueberry_ExcludeLegends->isChecked();
    }
    else if(checkBox == blueberry_OnlyLegends){
        randomizer.svRandomizerWilds.blueberry_OnlyLegends = blueberry_OnlyLegends->isChecked();
    }
    else if(checkBox == blueberry_OnlyParadox){
        randomizer.svRandomizerWilds.blueberry_OnlyParadox = blueberry_OnlyParadox->isChecked();
    }
    else if(checkBox == blueberry_OnlyLegendsAndParadox){
        randomizer.svRandomizerWilds.blueberry_OnlyLegendsAndParadox = blueberry_OnlyLegendsAndParadox->isChecked();
    }
    else if(checkBox == blueberry_BanStage1){
        randomizer.svRandomizerWilds.blueberry_BanStage1 = blueberry_BanStage1->isChecked();
    }
    else if(checkBox == blueberry_BanStage2){
        randomizer.svRandomizerWilds.blueberry_BanStage2 = blueberry_BanStage2->isChecked();
    }
    else if(checkBox == blueberry_BanStage3){
        randomizer.svRandomizerWilds.blueberry_BanStage3 = blueberry_BanStage3->isChecked();
    }
    else if(checkBox == blueberry_Ban1Stage){
        randomizer.svRandomizerWilds.blueberry_Ban1Stage = blueberry_Ban1Stage->isChecked();
    }
    else if(checkBox == blueberry_BalanceAreaPerBST){
        randomizer.svRandomizerWilds.blueberry_BalanceAreaPerBST = blueberry_BalanceAreaPerBST->isChecked();
    } // Raids
    else if(checkBox == praids_randomize){
        randomizer.svRandomizerRaids.praids_randomize = praids_randomize ->isChecked();
    }
    else if(checkBox == praids_randomize_per_star){
        randomizer.svRandomizerRaids.praids_randomize_per_star = praids_randomize_per_star ->isChecked();
    }
    else if(checkBox == praids_force_shiny){
        randomizer.svRandomizerRaids.praids_force_shiny = praids_force_shiny ->isChecked();
    }
    else if(checkBox == praids_onlyLegends){
        randomizer.svRandomizerRaids.praids_onlyLegends = praids_onlyLegends ->isChecked();
    }
    else if(checkBox == praids_onlyParadox){
        randomizer.svRandomizerRaids.praids_onlyParadox = praids_onlyParadox ->isChecked();
    }
    else if(checkBox == praids_onlyLegendsandParadox){
        randomizer.svRandomizerRaids.praids_onlyLegendsandParadox = praids_onlyLegendsandParadox ->isChecked();
    }
    else if(checkBox == praids_BanStage1){
        randomizer.svRandomizerRaids.praids_BanStage1 = praids_BanStage1 ->isChecked();
    }
    else if(checkBox == praids_BanStage2){
        randomizer.svRandomizerRaids.praids_BanStage2 = praids_BanStage2 ->isChecked();
    }
    else if(checkBox == praids_BanStage3){
        randomizer.svRandomizerRaids.praids_BanStage3 = praids_BanStage3 ->isChecked();
    }
    else if(checkBox == praids_Ban1Stage){
        randomizer.svRandomizerRaids.praids_Ban1Stage = praids_Ban1Stage ->isChecked();
    }
    else if(checkBox == praidsBalanceAreaPerBST){
        randomizer.svRandomizerRaids.praidsBalanceAreaPerBST = praidsBalanceAreaPerBST ->isChecked();
    }
    else if(checkBox == paldea_Settings_for_all_raids){
        randomizer.svRandomizerRaids.paldea_Settings_for_all_raids = paldea_Settings_for_all_raids->isChecked();
    }
    // Raids Kitakami
    else if(checkBox == kraids_randomize){
        randomizer.svRandomizerRaids.kraids_randomize = kraids_randomize ->isChecked();
    }
    else if(checkBox == kraids_randomize_per_star){
        randomizer.svRandomizerRaids.kraids_randomize_per_star = kraids_randomize_per_star ->isChecked();
    }
    else if(checkBox == kraids_force_shiny){
        randomizer.svRandomizerRaids.kraids_force_shiny = kraids_force_shiny ->isChecked();
    }
    else if(checkBox == kraids_onlyLegends){
        randomizer.svRandomizerRaids.kraids_onlyLegends = kraids_onlyLegends ->isChecked();
    }
    else if(checkBox == kraids_onlyParadox){
        randomizer.svRandomizerRaids.kraids_onlyParadox = kraids_onlyParadox ->isChecked();
    }
    else if(checkBox == kraids_onlyLegendsandParadox){
        randomizer.svRandomizerRaids.kraids_onlyLegendsandParadox = kraids_onlyLegendsandParadox ->isChecked();
    }
    else if(checkBox == kraids_BanStage1){
        randomizer.svRandomizerRaids.kraids_BanStage1 = kraids_BanStage1 ->isChecked();
    }
    else if(checkBox == kraids_BanStage2){
        randomizer.svRandomizerRaids.kraids_BanStage2 = kraids_BanStage2 ->isChecked();
    }
    else if(checkBox == kraids_BanStage3){
        randomizer.svRandomizerRaids.kraids_BanStage3 = kraids_BanStage3 ->isChecked();
    }
    else if(checkBox == kraids_Ban1Stage){
        randomizer.svRandomizerRaids.kraids_Ban1Stage = kraids_Ban1Stage ->isChecked();
    }
    else if(checkBox == kraidsBalanceAreaPerBST){
        randomizer.svRandomizerRaids.kraidsBalanceAreaPerBST = kraidsBalanceAreaPerBST ->isChecked();
    } // Raids Blueberry
    else if(checkBox == braids_randomize){
        randomizer.svRandomizerRaids.braids_randomize = braids_randomize ->isChecked();
    }
    else if(checkBox == braids_randomize_per_star){
        randomizer.svRandomizerRaids.braids_randomize_per_star = braids_randomize_per_star ->isChecked();
    }
    else if(checkBox == braids_force_shiny){
        randomizer.svRandomizerRaids.braids_force_shiny = braids_force_shiny ->isChecked();
    }
    else if(checkBox == braids_onlyLegends){
        randomizer.svRandomizerRaids.braids_onlyLegends = braids_onlyLegends ->isChecked();
    }
    else if(checkBox == braids_onlyParadox){
        randomizer.svRandomizerRaids.braids_onlyParadox = braids_onlyParadox ->isChecked();
    }
    else if(checkBox == braids_onlyLegendsandParadox){
        randomizer.svRandomizerRaids.braids_onlyLegendsandParadox = braids_onlyLegendsandParadox ->isChecked();
    }
    else if(checkBox == braids_BanStage1){
        randomizer.svRandomizerRaids.braids_BanStage1 = braids_BanStage1 ->isChecked();
    }
    else if(checkBox == braids_BanStage2){
        randomizer.svRandomizerRaids.braids_BanStage2 = braids_BanStage2 ->isChecked();
    }
    else if(checkBox == braids_BanStage3){
        randomizer.svRandomizerRaids.braids_BanStage3 = braids_BanStage3 ->isChecked();
    }
    else if(checkBox == braids_Ban1Stage){
        randomizer.svRandomizerRaids.braids_Ban1Stage = braids_Ban1Stage ->isChecked();
    }
    else if(checkBox == braidsBalanceAreaPerBST){
        randomizer.svRandomizerRaids.braidsBalanceAreaPerBST = braidsBalanceAreaPerBST ->isChecked();
    }// Trainers
    else if(checkBox == randomize_paldea_trainers){
        randomizer.svRandomizerTrainers.randomize_paldea_trainers = randomize_paldea_trainers ->isChecked();
    }
    else if(checkBox == randomize_kitakami_trainers){
        randomizer.svRandomizerTrainers.randomize_kitakami_trainers = randomize_kitakami_trainers ->isChecked();
    }
    else if(checkBox == randomize_blueberry_trainers){
        randomizer.svRandomizerTrainers.randomize_blueberry_trainers = randomize_blueberry_trainers ->isChecked();
    }
    else if (checkBox == use_trainer_paldea_for_all){
        randomizer.svRandomizerTrainers.use_trainer_paldea_for_all = use_trainer_paldea_for_all ->isChecked();
    }
    else{
        int index = 0;
        index = generations_starters.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerStarters.generations_starters[index] = generations_starters[index]->isChecked();
            return;
        }

        index = starters_shiny.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerStarters.starters_shiny[index] = starters_shiny[index]->isChecked();
            return;
        }

        index = generation_gifts.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerStarters.generation_gifts[index] = generation_gifts[index]->isChecked();
            return;
        }

        index = generation_stats.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerStats.generation_stats[index] = generation_stats[index]->isChecked();
            return;
        }

        index = generation_paldea_wild.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerWilds.generation_paldea_wild[index] = generation_paldea_wild[index]->isChecked();
            return;
        }

        index = generation_kitakami_wild.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerWilds.generation_kitakami_wild[index] = generation_kitakami_wild[index]->isChecked();
            return;
        }

        index = generation_blueberry_wild.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerWilds.generation_blueberry_wild[index] = generation_blueberry_wild[index]->isChecked();
            return;
        }

        index = praidsgeneration.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerRaids.praidsgeneratio[index] = praidsgeneration[index]->isChecked();
            return;
        }
        index = kraidsgeneration.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerRaids.kraidsgeneration[index] = kraidsgeneration[index]->isChecked();
            return;
        }
        index = braidsgeneration.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerRaids.braidsgeneration[index] = braidsgeneration[index]->isChecked();
            return;
        }

        // Trainers
        index = prival_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.prival_randomizer[index] = prival_randomizer[index]->isChecked();
            return;
        }
        index = proute_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.proute_randomizer[index] = proute_randomizer[index]->isChecked();
            return;
        }
        index = pgym_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.pgym_randomizer[index] = pgym_randomizer[index]->isChecked();
            return;
        }
        index = pelite4_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.pelite4_randomizer[index] = pelite4_randomizer[index]->isChecked();
            return;
        }
        index = pchampion_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.pchampion_randomizer[index] = pchampion_randomizer[index]->isChecked();
            return;
        }
        index = praid_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.praid_randomizer[index] = praid_randomizer[index]->isChecked();
            return;
        }
        index = pglobal_trainer_randomizer_settings.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.pglobal_trainer_randomizer_settings[index] = pglobal_trainer_randomizer_settings[index]->isChecked();
            return;
        }
        index = ptrainersgeneration.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.ptrainersgeneration[index] = ptrainersgeneration[index]->isChecked();
            return;
        }
        index = krival_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.krival_randomizer[index] = krival_randomizer[index]->isChecked();
            return;
        }
        index = kroute_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.kroute_randomizer[index] = kroute_randomizer[index]->isChecked();
            return;
        }
        index = kogre_clan_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.kogre_clan_randomizer[index] = kogre_clan_randomizer[index]->isChecked();
            return;
        }
        index = kraid_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.kraid_randomizer[index] = kraid_randomizer[index]->isChecked();
            return;
        }
        index = kglobal_trainer_randomizer_settings.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.kglobal_trainer_randomizer_settings[index] = kglobal_trainer_randomizer_settings[index]->isChecked();
            return;
        }
        index = ktrainersgeneration.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.ktrainersgeneration[index] = ktrainersgeneration[index]->isChecked();
            return;
        }
        index = brival_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.brival_randomizer[index] = brival_randomizer[index]->isChecked();
            return;
        }
        index = broute_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.broute_randomizer[index] = broute_randomizer[index]->isChecked();
            return;
        }
        index = b_bb4_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.b_bb4_randomizer[index] = b_bb4_randomizer[index]->isChecked();
            return;
        }
        index = braid_randomizer.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.braid_randomizer[index] = braid_randomizer[index]->isChecked();
            return;
        }
        index = bglobal_trainer_randomizer_settings.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.bglobal_trainer_randomizer_settings[index] = bglobal_trainer_randomizer_settings[index]->isChecked();
            return;
        }
        index = btrainersgeneration.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerTrainers.btrainersgeneration[index] = btrainersgeneration[index]->isChecked();
            return;
        }
        // same exact code but for the second array
    }
}

void SVRandomizerWindow::saveStringInput() {
    QLineEdit *senderLineEdit = qobject_cast<QLineEdit *>(sender());
    if (!senderLineEdit) {
        return;  // If the sender is not a QLineEdit, exit
    }

    // Determine which QLineEdit called the function
    if (senderLineEdit == nullptr) {
        qDebug() << "Saving input from starters_poke_ball:" << senderLineEdit->text();
        // Save or process text from starters_poke_ball
    }else{
        int index = 0;
        index = starters.indexOf(senderLineEdit);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerStarters.starters[index] = senderLineEdit->text();
            return;
        }
    }
}

void SVRandomizerWindow::updateComboBoxForms(QComboBox *comboBox, const QString &text) {
    comboBox->clear();
    for(int i =0; i< randomizer.pokemonFormsInGame[text].size(); i++){
        comboBox->addItem(randomizer.pokemonFormsInGame[text][i]);
    }
}

void SVRandomizerWindow::updateComboBoxGender(QComboBox *comboBox, const QString &text) {
    comboBox->clear();
    comboBox->addItem("MALE");
    comboBox->addItem("FEMALE");
    comboBox->addItem("DEFAULT");
    qDebug()<<text;
}

void SVRandomizerWindow::updatePreview(const QString &text) {
    previewLabel->setText("Preview: " + text);  // Update the preview label with the current text
}
