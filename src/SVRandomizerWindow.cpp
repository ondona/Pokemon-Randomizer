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
#include <QFileDialog>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


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
        connect(importButton, &QPushButton::clicked, this, &SVRandomizerWindow::importSettings);

        settings->addWidget(importButton);

        exportButton = new QPushButton("Export Settings and Seed", generalGroup);
        connect(exportButton, &QPushButton::clicked, this, &SVRandomizerWindow::exportSettings);

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
        connect(startRandomizer, &QPushButton::clicked, this, &SVRandomizerWindow::runRandomizer);
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
    topBar->addTab("Bosses");
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
    stackedWidget->addWidget(setupScenesWidget());
    //stackedWidget->addWidget(setupShopWidget());

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // Apply the main layout to RandomizerWindow
    setLayout(mainLayout);
}

void SVRandomizerWindow::MonLimiterSection(QVBoxLayout *OuterBox, QGroupBox *LimiterGroup, LimiterDetails *Details)
{	
    LimiterGroup = new QGroupBox("Available Mons (WIP)");
	OuterBox->addWidget(LimiterGroup);
	
    QVBoxLayout *LimiterSettings = new QVBoxLayout(LimiterGroup);
	
    QHBoxLayout *StagesSettings = new QHBoxLayout();
    QHBoxLayout *ClassSettings = new QHBoxLayout();
    QHBoxLayout *ExcludeSetting = new QHBoxLayout();
    QHBoxLayout *IncludeSetting = new QHBoxLayout();
    QHBoxLayout *GenerationSetting = new QHBoxLayout();
    QHBoxLayout *GenLegendsSetting = new QHBoxLayout();

    LimiterSettings->addLayout(GenerationSetting);
    LimiterSettings->addLayout(GenLegendsSetting);
    LimiterSettings->addLayout(StagesSettings);
    LimiterSettings->addLayout(ClassSettings);
    LimiterSettings->addLayout(ExcludeSetting);
    LimiterSettings->addLayout(IncludeSetting);

    QCheckBox *Stage_1 = new QCheckBox("Stage 1", LimiterGroup);
    Stage_1->setChecked(true);
    StagesSettings->addWidget(Stage_1);
    QString _key = "1";
    connect(Stage_1, &QCheckBox::toggled, this, [=]() { saveLimiterState(Details, _key);});

    QCheckBox *Stage_2 = new QCheckBox("Stage 2", LimiterGroup);
    Stage_2->setChecked(true);
    StagesSettings->addWidget(Stage_2);
    _key = "2";
    connect(Stage_2, &QCheckBox::toggled, this, [=]() { saveLimiterState(Details, _key);});

    QCheckBox *Stage_3 = new QCheckBox("Stage 3", LimiterGroup);
    Stage_3->setChecked(true);
    StagesSettings->addWidget(Stage_3);
    _key = "3";
    connect(Stage_3, &QCheckBox::toggled, this, [=]() { saveLimiterState(Details, _key);});

    QCheckBox *Single_stage = new QCheckBox("Single stage", LimiterGroup);
    Single_stage->setChecked(true);
    StagesSettings->addWidget(Single_stage);
    _key = "Single";
    connect(Single_stage, &QCheckBox::toggled, this, [=]() { saveLimiterState(Details, _key);});
	
    QCheckBox *Paradox = new QCheckBox("Paradox", LimiterGroup);
    Paradox->setChecked(true);
    ClassSettings->addWidget(Paradox);
    _key = "Paradox";
    connect(Paradox, &QCheckBox::toggled, this, [=]() { saveLimiterState(Details, _key);});
	
	/*
    QCheckBox *Ultrabeasts = new QCheckBox("Ultra Beasts", LimiterGroup);
    Ultrabeasts->setChecked(true);
    ClassSettings->addWidget(Ultrabeasts);
    _key = "UB";
    connect(Ultrabeasts, &QCheckBox::toggled, this, [=]() { saveLimiterState(Details, _key);});
	
	QLineEdit *Excludes = new QLineEdit(LimiterGroup);
	connect(Excludes, &QLineEdit::textChanged, this, &SVRandomizerWindow::saveStringInput);

	QLabel *ExcludesLabel = new QLabel("Force exclude", LimiterGroup);
	ExcludesLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	ExcludeSetting->addWidget(ExcludesLabel);

	Excludes->setFixedWidth(348);
	Excludes->setPlaceholderText("Enter names of mons to exclude.");
	ExcludeSetting->addWidget(Excludes);
	
	QLineEdit *Includes = new QLineEdit(LimiterGroup);
	connect(Includes, &QLineEdit::textChanged, this, &SVRandomizerWindow::saveStringInput);

	QLabel *IncludesLabel = new QLabel("Force allowed", LimiterGroup);
	IncludesLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	IncludeSetting->addWidget(IncludesLabel);

	Includes->setFixedWidth(348);
	Includes->setPlaceholderText("Enter names of mons to allow.");
	IncludeSetting->addWidget(Includes);*/
	
    QVector<QCheckBox*> generationslist;

    QLabel *GenerationsHeader = new QLabel("Allowed Generations", LimiterGroup);
    GenerationsHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    GenerationSetting->addWidget(GenerationsHeader);

    for(int i =0; i<9; i++){
        generationslist.append(new QCheckBox(QStringLiteral("%1").arg(i+1), LimiterGroup));
        generationslist[i]->setChecked(true);
        GenerationSetting->addWidget(generationslist[i]);
    connect(generationslist[i], &QCheckBox::toggled, this, [=]() { saveLimiterState(Details, "G" + QString::number(i));});
    }

    QVector<QCheckBox*> GenLegendslist;
    QLabel *GenLegendsHeader = new QLabel("Allowed Legendaries", LimiterGroup);
    GenLegendsHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    GenLegendsSetting->addWidget(GenLegendsHeader);

    for(int i =0; i<9; i++){
        GenLegendslist.append(new QCheckBox(QStringLiteral("%1").arg(i+1), LimiterGroup));
        GenLegendslist[i]->setChecked(true);
        GenLegendsSetting->addWidget(GenLegendslist[i]);
    connect(GenLegendslist[i], &QCheckBox::toggled, this, [=]() { saveLimiterState(Details, "L" + QString::number(i));});
    }
}

bool SVRandomizerWindow::Combobox_listfill(QComboBox *Combobox, QList<QString> List){
    for (int i = 0; i < int (List.count()); ++i) {
            Combobox->addItem(List[i]); // Add items from List to the dropdown
    }
    return true;
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
    connect(starters_gender[0], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    QLabel *pokeball_s1 = new QLabel("Pokeball ", startersGroupSettings);
    pokeball_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    startersRow_Q1->addWidget(pokeball_s1);

    starters_pokeball.append(new QComboBox());
    Combobox_listfill(starters_pokeball[0],  Pokeball_Names);
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
    connect(starters_gender[1], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    QLabel *pokeball_s2 = new QLabel("Pokeball ", startersGroupSettings);
    pokeball_s2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    startersRow_Q2->addWidget(pokeball_s2);
    starters_pokeball.append(new QComboBox());
    Combobox_listfill(starters_pokeball[1],  Pokeball_Names);
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
    connect(starters_gender[2], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    QLabel *pokeball_s3 = new QLabel("Pokeball ", startersGroupSettings);
    pokeball_s3->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    startersRow_Q3->addWidget(pokeball_s3);
    starters_pokeball.append(new QComboBox());
    Combobox_listfill(starters_pokeball[2],  Pokeball_Names);
    starters_pokeball[2]->setFixedSize(100, 25);
    connect(starters_pokeball[2], QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SVRandomizerWindow::saveComboInput);

    startersRow_Q3->addWidget(starters_pokeball[2]);

    startersSettingsLayout->addLayout(startersRow_Q3);
    startersRow_Q3->addStretch(1);  // Pushes widgets to the left
    
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
	
    MonLimiterSection(startersSettingsLayout, StartersLimiterGroup, &randomizer.svRandomizerStarters.StartersLimiter);
	
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

    MonLimiterSection(giftsSettingsLayout, GiftsLimiterGroup, &randomizer.svRandomizerStarters.GiftsLimiter);
	
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

    // force_tera_blast_every_5_levels = new QCheckBox("Force Tera Blast Every 5 levels", statsSettingsGroup);
    // row2->addWidget(force_tera_blast_every_5_levels);
    // connect(force_tera_blast_every_5_levels, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

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

    randomize_bosses =new QCheckBox("Randomize Bosses (Only Paldea ones)", bossWidget);
    row0->addWidget(randomize_bosses);
    connect(randomize_bosses, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    formLayout->addRow(row0);

    // -------------- New Row --------------

    QLabel *comingsoon = new QLabel("More Options Coming Soon", bossWidget);
    comingsoon->setStyleSheet("font-weight: bold; padding: 0px 0;");

    formLayout->addRow(comingsoon);
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
	
    MonLimiterSection(mainLayout, BossLimiterGroup, &randomizer.svRandomizerBoss.BossLimiter);

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

    let_ogre_pagos_spawn = new QCheckBox("Let Ogrepon \& Terapagos Spawn (Do not tera if caught in wild)", paldeaWildWidget);
    row0->addWidget(let_ogre_pagos_spawn);
    connect(let_ogre_pagos_spawn, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row0);

    // -------------- New Row --------------
    paldea_Settings_for_all_wild = new QCheckBox("Use Paldea Settings for all other wilds", paldeaWildWidget);
    row3->addWidget(paldea_Settings_for_all_wild);
    connect(paldea_Settings_for_all_wild, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

    paldea_BalanceAreaPerBST = new QCheckBox("Balance Area per BST (useless for now)", paldeaWildWidget);
    row3->addWidget(paldea_BalanceAreaPerBST);
    connect(paldea_BalanceAreaPerBST, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row3);

    //--------------Wild Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    MonLimiterSection(mainLayout, PaldeaWildLimiterGroup, &randomizer.svRandomizerWilds.PaldeaLimiter);

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

    // -------------- New Row --------------

    kitakami_BalanceAreaPerBST = new QCheckBox("Balance Area per BST (useless for now)", kitakamiWildWidget);
    row3->addWidget(kitakami_BalanceAreaPerBST);
    connect(kitakami_BalanceAreaPerBST, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row3);

    //--------------Wild Settings End--------------

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    MonLimiterSection(mainLayout, KitakamiWildLimiterGroup, &randomizer.svRandomizerWilds.KitakamiLimiter);

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

    // -------------- New Row --------------

    blueberry_BalanceAreaPerBST = new QCheckBox("Balance Area per BST (useless for now)", blueberryWildWidget);
    row3->addWidget(blueberry_BalanceAreaPerBST);
    connect(blueberry_BalanceAreaPerBST, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);
    formLayout->addRow(row3);

    //--------------Wild Settings End--------------

    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    MonLimiterSection(mainLayout, BlueberryWildLimiterGroup, &randomizer.svRandomizerWilds.BlueberryLimiter);

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
	
    //--------------Raid Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    MonLimiterSection(mainLayout, RaidsPaldeaLimiterGroup, &randomizer.svRandomizerRaids.RaidsPaldeaLimiter);

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

    //--------------Raid Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    MonLimiterSection(mainLayout, RaidsKitakamiLimiterGroup, &randomizer.svRandomizerRaids.RaidsKitakamiLimiter);

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

    //--------------Raid Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);
    MonLimiterSection(mainLayout, RaidsBlueberryLimiterGroup, &randomizer.svRandomizerRaids.RaidsBlueberryLimiter);

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

	//Allowed Mons Section
	MonLimiterSection(rivalsTrainersLayout, PRivalLimiterGroup, &randomizer.svRandomizerTrainers.PRivalLimiter);
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

		//Allowed Mons Section
		MonLimiterSection(routesTrainersLayout, PRouteLimiterGroup, &randomizer.svRandomizerTrainers.PRouteLimiter);
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

		//Allowed Mons Section
		MonLimiterSection(gymTrainersLayout, PGymLimiterGroup, &randomizer.svRandomizerTrainers.PGymLimiter);
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
		
		//Allowed Mons Section
		MonLimiterSection(e4TrainersLayout, PE4LimiterGroup, &randomizer.svRandomizerTrainers.PE4Limiter);
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

		//Allowed Mons Section
		MonLimiterSection(champTrainersLayout, PChampionLimiterGroup, &randomizer.svRandomizerTrainers.PChampionLimiter);
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

		//Allowed Mons Section
		MonLimiterSection(raidTrainersLayout, PRaidLimiterGroup, &randomizer.svRandomizerTrainers.PRaidLimiter);
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

	//Allowed Mons Section
	MonLimiterSection(rivalsTrainersLayout, KRivalLimiterGroup, &randomizer.svRandomizerTrainers.KRivalLimiter);
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

	//Allowed Mons Section
	MonLimiterSection(routesTrainersLayout, KRouteLimiterGroup, &randomizer.svRandomizerTrainers.KRouteLimiter);
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

	//Allowed Mons Section
	MonLimiterSection(gymTrainersLayout, KOCLimiterGroup, &randomizer.svRandomizerTrainers.KOCLimiter);
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

	//Allowed Mons Section
	MonLimiterSection(e4TrainersLayout, KRaidLimiterGroup, &randomizer.svRandomizerTrainers.KRaidLimiter);
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

	//Allowed Mons Section
	MonLimiterSection(rivalsTrainersLayout, BRivalLimiterGroup, &randomizer.svRandomizerTrainers.BRivalLimiter);
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
	
	//Allowed Mons Section
	MonLimiterSection(routesTrainersLayout, BRouteLimiterGroup, &randomizer.svRandomizerTrainers.BRouteLimiter);
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

	//Allowed Mons Section
	MonLimiterSection(gymTrainersLayout, BBB4LimiterGroup, &randomizer.svRandomizerTrainers.BBB4Limiter);
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

	//Allowed Mons Section
	MonLimiterSection(e4TrainersLayout, BRaidLimiterGroup, &randomizer.svRandomizerTrainers.BRaidLimiter);
    formLayout->addRow(e4_Settings_Group);
    //--------------E4 Settings End--------------
    // Set form layout to main layout
    mainLayout->addLayout(formLayout);

    // Add giftWidget to scroll area and set scrollArea as the widget
    scrollArea->setWidget(blueberryTrainersWidget);
    return scrollArea;
}

bool SVRandomizerWindow::checkAndDeleteFile(std::string filePath) {

    size_t pos = filePath.find("SV_FLATBUFFERS");
    std::string keyword = "SV_FLATBUFFERS";

    std::string result;
    if(pos != std::string::npos){
        result = filePath.substr(pos + keyword.length() + 1);
    }

    try {
        // Check if the file exists
        if (fs::exists(filePath)) {
            // Delete the file
            if (fs::remove(filePath)) {
                qDebug() << "File deleted successfully: " << result;
                return true; // File existed and was deleted
            } else {
                std::cerr << "Failed to delete the file: " << result << std::endl;
                return false;
            }
        } else {
            qDebug()  << "File does not exist: " << result;
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

void showMessage(const QString &message) {
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.raise();
    msgBox.activateWindow();
    QApplication::beep();
    msgBox.exec(); // Blocks until the user clicks OK
}

void SVRandomizerWindow::runRandomizer()
{
	//check allowedlists
	int starterscount = SVShared::GenerateAllowedMons(randomizer.svRandomizerStarters.StartersLimiter, randomizer.svRandomizerStarters.allowedPokemon);
	int giftscount = SVShared::GenerateAllowedMons(randomizer.svRandomizerStarters.GiftsLimiter, randomizer.svRandomizerStarters.allowedPokemon_gifts);
	
	int raidspcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerRaids.RaidsPaldeaLimiter, randomizer.svRandomizerRaids.RaidsPaldeaAllowed);
	int raidskcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerRaids.RaidsKitakamiLimiter, randomizer.svRandomizerRaids.RaidsKitakamiAllowed);
	int raidsbcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerRaids.RaidsBlueberryLimiter, randomizer.svRandomizerRaids.RaidsBlueberryAllowed);
	
	int wildspcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerWilds.PaldeaLimiter, randomizer.svRandomizerWilds.WildPaldeaAllowed);
	int wildskcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerWilds.KitakamiLimiter, randomizer.svRandomizerWilds.WildKitakamiAllowed);
	int wildsbcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerWilds.BlueberryLimiter, randomizer.svRandomizerWilds.WildBlueberryAllowed);
	int wildsfcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerWilds.PaldeaLimiter, randomizer.svRandomizerWilds.FixedSymbolAllowed);//using paldea for now not FixedSymbolLimiter
	
	int PTrainersRicount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.PRivalLimiter, randomizer.svRandomizerTrainers.PRivalAllowedPokemon);
	int PTrainersRocount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.PRouteLimiter, randomizer.svRandomizerTrainers.PRouteAllowedPokemon);
	int PTrainersGycount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.PGymLimiter, randomizer.svRandomizerTrainers.PGymAllowedPokemon);
	int PTrainersE4count = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.PE4Limiter, randomizer.svRandomizerTrainers.PE4AllowedPokemon);
	int PTrainersRacount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.PRaidLimiter, randomizer.svRandomizerTrainers.PRaidAllowedPokemon);
	int PTrainersChcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.PChampionLimiter, randomizer.svRandomizerTrainers.PChampionAllowedPokemon);
	
	int KTrainersRicount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.KRivalLimiter, randomizer.svRandomizerTrainers.KRivalAllowedPokemon);
	int KTrainersRocount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.KRouteLimiter, randomizer.svRandomizerTrainers.KRouteAllowedPokemon);
	int KTrainersOCcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.KOCLimiter, randomizer.svRandomizerTrainers.KOCAllowedPokemon);
	int KTrainersRacount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.KRaidLimiter, randomizer.svRandomizerTrainers.KRaidAllowedPokemon);
	
	int BTrainersRicount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.BRivalLimiter, randomizer.svRandomizerTrainers.BRivalAllowedPokemon);
	int BTrainersRocount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.BRouteLimiter, randomizer.svRandomizerTrainers.BRouteAllowedPokemon);
	int BTrainersBBcount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.BBB4Limiter, randomizer.svRandomizerTrainers.BBB4AllowedPokemon);
	int BTrainersRacount = SVShared::GenerateAllowedMons(randomizer.svRandomizerTrainers.BRaidLimiter, randomizer.svRandomizerTrainers.BRaidAllowedPokemon);

	int Bosscount = SVShared::GenerateAllowedMons(randomizer.svRandomizerBoss.BossLimiter, randomizer.svRandomizerBoss.BossAllowedPokemon);
		
	qDebug()<<QString("Starters: %1, Gifts: %2, Raids P: %3, Raids K: %4, Raids B: %5, Wilds P: %6, Wilds K: %7, Wilds B: %8, Wilds Fixed: %9, Boss: %10")
				.arg(starterscount)
				.arg(giftscount)
				.arg(raidspcount)
				.arg(raidskcount)
				.arg(raidsbcount)
				.arg(wildspcount)
				.arg(wildskcount)
				.arg(wildsbcount)
				.arg(wildsfcount)	
				.arg(Bosscount);	
				
	qDebug()<<QString("Paldea Rivals: %1, Routes: %2, Gyms: %3, Elite4: %4, Raids: %5, Champion: %6")
				.arg(PTrainersRicount)
				.arg(PTrainersRocount)
				.arg(PTrainersGycount)
				.arg(PTrainersE4count)
				.arg(PTrainersRacount)
				.arg(PTrainersChcount);
				
	qDebug()<<QString("Kitakami Rivals: %1, Routes: %2, Ogre Clan: %3, Raids: %4")
				.arg(KTrainersRicount)
				.arg(KTrainersRocount)
				.arg(KTrainersOCcount)
				.arg(KTrainersRacount);
				
	qDebug()<<QString("Blueberry Rivals: %1, Routes: %2, Blueberry League: %3, Raids: %4")
				.arg(BTrainersRicount)
				.arg(BTrainersRocount)
				.arg(BTrainersBBcount)
				.arg(BTrainersRacount);
				

//add a check to make sure there r usable mons for all sections
    if ((starterscount <= 0) or (giftscount <= 0) or (raidspcount <= 0) or (raidskcount <= 0) or (raidsbcount <= 0) or (wildspcount <= 0) or (wildskcount <= 0) or (wildsbcount <= 0) or (wildsfcount <= 0) or (PTrainersRicount <= 0) or (PTrainersRocount <= 0) or (PTrainersGycount <= 0) or (PTrainersE4count <= 0) or (PTrainersRacount <= 0) or (PTrainersChcount <= 0) or (KTrainersRicount <= 0) or (KTrainersRocount <= 0) or (KTrainersOCcount <= 0) or (KTrainersRacount <= 0) or (BTrainersRicount <= 0) or (BTrainersRocount <= 0) or (BTrainersBBcount <= 0) or (BTrainersRacount <= 0) or (Bosscount <= 0))
	{//send user message to say it cant randomize with 0 pokemon
		showMessage("Unable to use allowed pokemon selection for a section, it results in 0 allowed. Randomization Aborted");
        return;
	}

	hash = 0;
    bool randomizerIsNum = false;
    randomizer.seed.toUInt(&randomizerIsNum);
    if(randomizerIsNum == false){
        if(!randomizer.seed.isEmpty()){
            for (const QChar &ch : randomizer.seed) {
                hash = hash * 31 + ch.unicode(); // Accumulate a numeric value
            }
        }
    }else{
        hash = randomizer.seed.toUInt();
    }
    for(unsigned int i = 1; i< randomizer.bulk_amount+1; i++){
        if(hash == 0){
            std::random_device rd;
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = now.time_since_epoch();
            hash = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() ^ rd());
        }

        srand(hash*i);
        qDebug()<<"Seed is: "<<randomizer.seed;
        qDebug()<<QStringLiteral("Numerical is: %1").arg(hash*i);

        std::string outputKey = "output";

        std::string dirPath = fs::absolute(outputKey).string();
        if(fs::exists(dirPath)){
            fs::remove_all(dirPath);
        }

        randomizer.createFolderHierarchy(outputKey+"/romfs");

        std::string baseAddress = fs::absolute("SV_FLATBUFFERS").string();
        QString QBaseAddress = QString::fromStdString(baseAddress);
        QDir qBaseDir(QBaseAddress);

        if(randomizer.svRandomizerStarters.enable_starters == true){
            randomizer.svRandomizeStarters(randomizer.svRandomizerStarters);

            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0060_always_0.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0060_/", true);
            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0060_always_1.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0060_/", true);

            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0060_main_0.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0060_/", true);
            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0060_main_1.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0060_/", true);

            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0070_always_0.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0070_/", true);
            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0070_always_1.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0070_/", true);

            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0088_always_0.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0088_/", true);
            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0088_always_1.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0088_/", true);

            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0090_main_0.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0090_/", true);
            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/TrinitySceneObjectTemplate.fbs").toStdString(),
                                      qBaseDir.filePath(+"SV_SCENES/SV_STARTERS_SCENES/common_0090_main_1.json").toStdString(),
                                      "world/scene/parts/event/event_scenario/main_scenario/common_0090_/", true);

            try {
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_always_0.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_always_0.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_always_1.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_always_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_main_0.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_main_0.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_main_1.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0060_/common_0060_main_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0070_/common_0070_always_0.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0070_/common_0070_always_0.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0070_/common_0070_always_1.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0070_/common_0070_always_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0088_/common_0088_always_0.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0088_/common_0088_always_0.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0088_/common_0088_always_1.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0088_/common_0088_always_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0090_/common_0090_main_0.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0090_/common_0090_main_0.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0090_/common_0090_main_1.bin").string(),
                fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0090_/common_0090_main_1.trsog").string());

            } catch (const fs::filesystem_error& e) {
                qDebug() << "Error renaming file: " << e.what();
            }

        }

        if(randomizer.svRandomizerStarters.enable_gifts == true){
            randomizer.svRandomizeGifts(randomizer.svRandomizerStarters);
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

            randomizer.svRandomizeStats(randomizer.svRandomizerStats);

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

            randomizer.svRandomizeItem(randomizer.svRandomizerItems);

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

            randomizer.svRandomizeWilds(randomizer.svRandomizerWilds);

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

            randomizer.svRandomizeRaids(randomizer.svRandomizerRaids);

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
            randomizer.svRandomizeTrainers(randomizer.svRandomizerTrainers);
            qDebug()<<"Made it here";
            randomizer.generateBinary(qBaseDir.filePath("SV_TRAINERS/trdata_array.bfbs").toStdString(),
                                      qBaseDir.filePath(+"SV_TRAINERS/trdata_array.json").toStdString(),
                                      "world/data/trainer/trdata/", true);
        }

        if(randomizer.svRandomizerBoss.randomize_bosses == true){
            randomizer.svRandomizeBoss(randomizer.svRandomizerBoss, qBaseDir);

            randomizer.generateBinary(qBaseDir.filePath("SV_SCENES/eventBattlePokemon_array.bfbs").toStdString(),
                           qBaseDir.filePath(+"SV_SCENES/eventBattlePokemon_array.json").toStdString(),
                           "world/data/battle/eventBattlePokemon/");

            try{
                // Lechonk
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0100_/common_0100_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0100_/common_0100_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0100_/common_0100_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0100_/common_0100_main_1.trsog").string());

                // Treasure of Ruin
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_017_/sub_017_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_017_/sub_017_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_017_/sub_017_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_017_/sub_017_main_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_016_/sub_016_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_016_/sub_016_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_016_/sub_016_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_016_/sub_016_main_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_015_/sub_015_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_015_/sub_015_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_015_/sub_015_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_015_/sub_015_main_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_014_/sub_014_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_014_/sub_014_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_014_/sub_014_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_014_/sub_014_main_1.trsog").string());

                // GreatIron Titan
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_041_/sub_041_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_041_/sub_041_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_041_/sub_041_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_041_/sub_041_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_020_/nushi_jimen_fp_1090_020_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_020_/nushi_jimen_fp_1090_020_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_020_/nushi_jimen_fp_1090_020_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_020_/nushi_jimen_fp_1090_020_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_010_/nushi_jimen_fp_1090_010_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_010_/nushi_jimen_fp_1090_010_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_010_/nushi_jimen_fp_1090_010_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1090_010_/nushi_jimen_fp_1090_010_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_010_/nushi_jimen_fp_1082_010_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_010_/nushi_jimen_fp_1082_010_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_010_/nushi_jimen_fp_1082_010_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_010_/nushi_jimen_fp_1082_010_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_020_/nushi_jimen_fp_1082_020_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_020_/nushi_jimen_fp_1082_020_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_020_/nushi_jimen_fp_1082_020_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/jimen/nushi_jimen_fp_1082_020_/nushi_jimen_fp_1082_020_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_020_/nushi_jimen_020_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_020_/nushi_jimen_020_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_020_/nushi_jimen_020_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_020_/nushi_jimen_020_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_010_/nushi_jimen_010_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_010_/nushi_jimen_010_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_010_/nushi_jimen_010_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_jimen_010_/nushi_jimen_010_pre_start_1.trsog").string());

                //Klawf
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_037_/sub_037_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_037_/sub_037_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_037_/sub_037_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_037_/sub_037_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_020_/nushi_iwa_fp_1066_020_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_020_/nushi_iwa_fp_1066_020_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_020_/nushi_iwa_fp_1066_020_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_020_/nushi_iwa_fp_1066_020_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_/nushi_iwa_fp_1066_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_/nushi_iwa_fp_1066_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_/nushi_iwa_fp_1066_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/iwa/nushi_iwa_fp_1066_/nushi_iwa_fp_1066_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_020_/nushi_iwa_020_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_020_/nushi_iwa_020_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_020_/nushi_iwa_020_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_020_/nushi_iwa_020_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_010_/nushi_iwa_010_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_010_/nushi_iwa_010_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_010_/nushi_iwa_010_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_iwa_010_/nushi_iwa_010_pre_start_1.trsog").string());

                //Bombirdier
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_038_/sub_038_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_038_/sub_038_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_038_/sub_038_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_038_/sub_038_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_020_/nushi_hiko_fp_1063_020_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_020_/nushi_hiko_fp_1063_020_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_020_/nushi_hiko_fp_1063_020_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_020_/nushi_hiko_fp_1063_020_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_010_/nushi_hiko_fp_1063_010_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_010_/nushi_hiko_fp_1063_010_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_010_/nushi_hiko_fp_1063_010_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hiko/nushi_hiko_fp_1063_010_/nushi_hiko_fp_1063_010_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/nushi_hikou_020_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/nushi_hikou_020_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/nushi_hikou_020_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/nushi_hikou_020_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/nushi_hikou_020_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/nushi_hikou_020_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/nushi_hikou_020_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_020_/nushi_hikou_020_main_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_010_/nushi_hikou_010_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_010_/nushi_hikou_010_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_010_/nushi_hikou_010_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hikou_010_/nushi_hikou_010_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/field/field_contents/nushi/hiko/HikoNushi_/HikoNushi_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/field/field_contents/nushi/hiko/HikoNushi_/HikoNushi_0.trscn").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/field/field_contents/nushi/hiko/HikoNushi_/HikoNushi_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/field/field_contents/nushi/hiko/HikoNushi_/HikoNushi_1.trscn").string());

                //Orthworm
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_039_/sub_039_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_039_/sub_039_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_039_/sub_039_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_039_/sub_039_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_020_/nushi_hagane_fp_1048_020_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_020_/nushi_hagane_fp_1048_020_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_020_/nushi_hagane_fp_1048_020_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_020_/nushi_hagane_fp_1048_020_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_010_/nushi_hagane_fp_1048_010_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_010_/nushi_hagane_fp_1048_010_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_010_/nushi_hagane_fp_1048_010_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/hagane/nushi_hagane_fp_1048_010_/nushi_hagane_fp_1048_010_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_020_/nushi_hagane_020_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_020_/nushi_hagane_020_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_020_/nushi_hagane_020_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_020_/nushi_hagane_020_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_010_/nushi_hagane_010_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_010_/nushi_hagane_010_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_010_/nushi_hagane_010_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_hagane_010_/nushi_hagane_010_pre_start_1.trsog").string());

                // Tatsugiri
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_040_/sub_040_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_040_/sub_040_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_040_/sub_040_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/sub_scenario/sub_040_/sub_040_pre_start_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_020_/nushi_dragon_fp_1056_020_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_020_/nushi_dragon_fp_1056_020_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_020_/nushi_dragon_fp_1056_020_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_020_/nushi_dragon_fp_1056_020_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_010_/nushi_dragon_fp_1056_010_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_010_/nushi_dragon_fp_1056_010_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_010_/nushi_dragon_fp_1056_010_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1056_010_/nushi_dragon_fp_1056_010_1.trsot").string());

                // Dondozo
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_020_/nushi_dragon_fp_1035_020_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_020_/nushi_dragon_fp_1035_020_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_020_/nushi_dragon_fp_1035_020_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_020_/nushi_dragon_fp_1035_020_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_010_/nushi_dragon_fp_1035_010_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_010_/nushi_dragon_fp_1035_010_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_010_/nushi_dragon_fp_1035_010_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/nushi/dragon/nushi_dragon_fp_1035_010_/nushi_dragon_fp_1035_010_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/nushi_dragon_010_always_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/nushi_dragon_010_always_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/nushi_dragon_010_always_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/nushi_dragon_010_always_1.trsog").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_020_/nushi_dragon_020_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_020_/nushi_dragon_020_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_020_/nushi_dragon_020_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_020_/nushi_dragon_020_pre_start_1.trsog").string());

                // Sunflora
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/gym_kusa_poke_finding_/pokes_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/gym_kusa_poke_finding_/pokes_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/gym_kusa_poke_finding_/pokes_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/gym_kusa_poke_finding_/pokes_1.trsot").string());

                // Houndoom
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0150_/common_0150_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0150_/common_0150_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0150_/common_0150_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_0150_/common_0150_main_1.trsog").string());

                // Gimmighoul
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/coin_symbol/coin_symbol_box_/coin_symbol_box_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/coin_symbol/coin_symbol_box_/coin_symbol_box_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/coin_symbol/coin_symbol_box_/coin_symbol_box_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/coin_symbol/coin_symbol_box_/coin_symbol_box_1.trsot").string());

                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/coin_symbol/coin_symbol_walk_/coin_symbol_walk_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/coin_symbol/coin_symbol_walk_/coin_symbol_walk_0.trsot").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/obj_template/parts/coin_symbol/coin_symbol_walk_/coin_symbol_walk_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/obj_template/parts/coin_symbol/coin_symbol_walk_/coin_symbol_walk_1.trsot").string());

                // Way Home
                // Paradox Circle
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1170_/common_1170_always_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1170_/common_1170_always_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1170_/common_1170_always_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1170_/common_1170_always_1.trsog").string());

                // Tusk/Treads
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1095_/common_1095_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1095_/common_1095_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1095_/common_1095_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1095_/common_1095_main_1.trsog").string());

                // Tails/Bundle
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1075_/common_1075_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1075_/common_1075_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1075_/common_1075_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1075_/common_1075_main_1.trsog").string());

                // Glimmora
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1055_/common_1055_main_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1055_/common_1055_main_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1055_/common_1055_main_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/common_1055_/common_1055_main_1.trsog").string());

                //Testing
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/nushi_dragon_010_pre_start_0.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/nushi_dragon_010_pre_start_0.trsog").string());
                fs::rename(fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/nushi_dragon_010_pre_start_1.bin").string(),
                           fs::absolute(outputKey+"/romfs/world/scene/parts/event/event_scenario/main_scenario/nushi_dragon_010_/nushi_dragon_010_pre_start_1.trsog").string());
            }catch (const fs::filesystem_error& e) {
                qFatal() << "Error renaming file: " << e.what();
            }
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

        //ondo spoiler placeholder

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
            checkAndDeleteFile(qBaseDir.filePath(+"SV_TRAINERS/trdata_array.json").toStdString());
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
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0090_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0090_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0088_always_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0088_always_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0070_always_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0070_always_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_always_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_STARTERS_SCENES/common_0060_always_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_FIRST_ROUTE/common_0100_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_FIRST_ROUTE/common_0100_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/MISC/Gimmighoul/coin_symbol_box_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/MISC/Gimmighoul/coin_symbol_box_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/MISC/Gimmighoul/coin_symbol_walk_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/MISC/Gimmighoul/coin_symbol_walk_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/MISC/Houndoom/common_0150_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/MISC/Houndoom/common_0150_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/MISC/Sunflora/pokes_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/MISC/Sunflora/pokes_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1055_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1075_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1095_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1170_always_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1055_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1075_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1095_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_AZ/WayHome/common_1170_always_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_014_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_015_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_016_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_017_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_014_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_015_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_016_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_LEGENDS/Treasures/sub_017_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/HikoNushi_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hiko_fp_1063_010_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hiko_fp_1063_020_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hikou_010_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hikou_020_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hikou_020_main_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/sub_038_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_always_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_010_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_020_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_010_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_020_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/sub_040_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_010_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_020_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1082_010_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1082_020_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1090_010_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1090_020_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/sub_041_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_010_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_020_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_fp_1066_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_fp_1066_020_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/sub_037_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_010_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_020_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_fp_1048_010_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_fp_1048_020_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/sub_039_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_020_pre_start_0.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_pre_start_0.json").toStdString());

            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/HikoNushi_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hiko_fp_1063_010_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hiko_fp_1063_020_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hikou_010_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hikou_020_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/nushi_hikou_020_main_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Bombirdier/sub_038_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_always_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_010_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1035_020_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_010_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_fp_1056_020_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/sub_040_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_010_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_020_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1082_010_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1082_020_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1090_010_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/nushi_jimen_fp_1090_020_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Great_Iron/sub_041_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_010_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_020_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_fp_1066_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/nushi_iwa_fp_1066_020_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Klawf/sub_037_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_010_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_020_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_fp_1048_010_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/nushi_hagane_fp_1048_020_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Orthworm/sub_039_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_020_pre_start_1.json").toStdString());
            checkAndDeleteFile(qBaseDir.filePath("SV_SCENES/SV_TITANS/Dondozo_Tatsu/nushi_dragon_010_pre_start_1.json").toStdString());
        // Delete Output file (again)
        if(fs::exists(dirPath)){
            fs::remove_all(dirPath);
        }
    }
    forceExportSettings();
    showMessage("Finished Randomizer");
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
    else if(checkBox == let_ogre_pagos_spawn){
        randomizer.svRandomizerWilds.let_oger_pagos_spawn = let_ogre_pagos_spawn->isChecked();
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
    }
    // Stats
    else if(checkBox == randomize_abilities){
        randomizer.svRandomizerStats.randomize_abilities = randomize_abilities->isChecked();
    }
    else if(checkBox == randomize_types){
        randomizer.svRandomizerStats.randomize_types = randomize_types->isChecked();
        randomizer.svRandomizerWilds.types_changed = randomize_types->isChecked();
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
    }

    // Wilds and Statics
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
    }

    // Raids
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
    else if(checkBox == randomize_bosses){
        randomizer.svRandomizerBoss.randomize_bosses = randomize_bosses->isChecked();
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

        index = boss_settings.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerBoss.boss_settings[index] = boss_settings[index]->isChecked();
            return;
        }

        index = boss_generation.indexOf(checkBox);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerBoss.boss_generation[index] = boss_generation[index]->isChecked();
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
    if (senderLineEdit == seed) {
        randomizer.seed = senderLineEdit->text();
    }else{
        int index = 0;
        index = starters.indexOf(senderLineEdit);
        if (index != -1) {  // Ensure the checkbox was found in the array
            randomizer.svRandomizerStarters.starters[index] = senderLineEdit->text();
            return;
        }
    }
}

void SVRandomizerWindow::saveLimiterState(LimiterDetails *Details, QString opt) {
    QCheckBox *checkBox = qobject_cast<QCheckBox *>(sender());
    //qDebug() << opt;
    if (!checkBox) return;  // Exit if sender is not a QCheckBox
    //NOTE: must make sure no opts start with G
    const QString zerovalue = "0";
    if (opt[0] == "G")
    {
        int index = opt[1].digitValue() - zerovalue[0].digitValue();
        Details->Gens[index] = checkBox->isChecked();
        return;
    }
    if (opt[0] == "L")
    {
        int index = opt[1].digitValue() - zerovalue[0].digitValue();
        Details->GenLegends[index] = checkBox->isChecked();
        return;
    }
    //if it isnt the gen lists then its a checkbox
    QList<QString> options = {"1", "2", "3", "Single", "Paradox", "UB"};
	int selected = options.indexOf(opt);
	switch (selected)
	{
		break;
        case 0://"1"
        Details->Stage1 = checkBox->isChecked();
		break;
		case 1://"2"
        Details->Stage2 = checkBox->isChecked();
		break;
		case 2://"3"
        Details->Stage3 = checkBox->isChecked();
		break;
		case 3://"Single"
        Details->SingleStage = checkBox->isChecked();
        break;
        case 4://"Paradox"
        Details->Paradox = checkBox->isChecked();
		break;
        case 5://"UB"
        Details->UltraBeast = checkBox->isChecked();
		break;
		default:
		qDebug() << "Ondo: How here?" << opt;
		break;
	}
    //qDebug() << Details->toString();
}

void SVRandomizerWindow::updateComboBoxForms(QComboBox *comboBox, const QString &text) {
    comboBox->clear();
    for(int i =0; i< randomizer.pokemonFormsInGame[text].size(); i++){
        comboBox->addItem(randomizer.pokemonFormsInGame[text][i]);
    }
}

void SVRandomizerWindow::updateComboBoxGender(QComboBox *comboBox, const QString &text) {
    comboBox->clear();
    if(randomizer.genderForms.contains(text)){
        comboBox->addItem("DEFAULT");
    }else if(randomizer.femaleOnlyPokemon.contains(text)){
        comboBox->addItem("FEMALE");
    }else if(randomizer.maleOnlyPokemon.contains(text)){
        comboBox->addItem("MALE");
    } else if(randomizer.genderlessPokemon.contains(text)){
            comboBox->addItem("GENDERLESS");
    } else{
        comboBox->addItem("MALE");
        comboBox->addItem("FEMALE");
    }
}

void SVRandomizerWindow::updatePreview(const QString &text) {
    previewLabel->setText("Preview: " + text);  // Update the preview label with the current text
}

void SVRandomizerWindow::updateCheckboxImport(QCheckBox *checkbox, const QString &key, QJsonObject &listOfChanges){
    checkbox->setChecked(listOfChanges[key].toBool());
}

void SVRandomizerWindow::updateQLineEditImport(QLineEdit *lineEdit, const QString& key, QJsonObject &listOfChanges){
    lineEdit ->setText(listOfChanges[key].toString());
}

void SVRandomizerWindow::updateComboBoxImport(QComboBox *comboBox, const QString& key, QJsonObject &listOfChanges, int index){
    if(comboBox == starters_pokeball[index]){
        comboBox->setCurrentIndex(randomizer.pokeballIndex[listOfChanges[key].toString()]);
    }
    else if(comboBox == starters_gender[index]){
        if(randomizer.maleOnlyPokemon.contains(randomizer.svRandomizerStarters.starters[index]) ||
            randomizer.femaleOnlyPokemon.contains(randomizer.svRandomizerStarters.starters[index]) ||
            randomizer.genderlessPokemon.contains(randomizer.svRandomizerStarters.starters[index])){
            comboBox ->setCurrentIndex(0);
        }else{
            if(listOfChanges[key].toString() == "FEMALE"){
                comboBox ->setCurrentIndex(1);
            }else{
                comboBox ->setCurrentIndex(0);
            }
        }
    }
    else{
        comboBox ->setCurrentIndex(listOfChanges[key].toInt());
    }
}

void SVRandomizerWindow::updateCheckboxArrayImportGenerations(QVector<QCheckBox*> gens, QJsonObject &listOfChanges){
    for(int i = 1; i<=9; i++){
        gens[i-1]->setChecked(listOfChanges[QString::number(i)].toBool());
    }
}

void SVRandomizerWindow::updateSpinBoxImport(QSpinBox* spinBox, const QString& key, QJsonObject &listOfChanges){
    spinBox->setValue(listOfChanges[key].toInt());
}

void SVRandomizerWindow::setUpUISettings(){
    /*
     * DO NOT MAKE SUB FUNCTIONS FOR THINGS HERE - THIS WAY WE CAN EASILYTELL
     * WHAT IS WRONG IN IMPORTING WITHOUT HAVING TO DIVE 3 FUNCTIONS DEEP
     */

    // Global Settings Done
    auto_patch->setChecked(settings["Auto-Patch"].toBool());
    seed ->setText(settings["Seed"].toString());
    bulk_amount->setValue(settings["Batch"].toInt());
    if(settings["Seed Numerical"].toInt() != 0){
        randomizer.seed = settings["Seed Numerical"].toString();
    }
    kaizo_mode->setChecked(settings["Kaizo Mode"].toBool());

    // Starters
    QJsonObject startersJson = settings["Starters"].toObject();
    updateCheckboxImport(enable_starters, "Randomize", startersJson);
    updateCheckboxImport(force_shiny_starter, "One Shiny", startersJson);
    updateCheckboxImport(all_starters_shiny, "All Shiny", startersJson);
    updateCheckboxImport(show_starters_in_overworld, "Show Starters in Overworld", startersJson);
    updateCheckboxImport(randomize_starters_tera_types, "Tera Types", startersJson);
    updateCheckboxImport(ban_stage_1_pokemon_starters, "Ban Stage 1", startersJson);
    updateCheckboxImport(ban_stage_2_pokemon_starters, "Ban Stage 2", startersJson);
    updateCheckboxImport(ban_stage_3_pokemon_starters, "Ban Stage 3", startersJson);
    updateCheckboxImport(ban_single_stage_pokemon_starters, "Ban 1 Stage", startersJson);
    updateCheckboxImport(only_legendary_pokemon_starters, "Only Legend", startersJson);
    updateCheckboxImport(only_paradox_pokemon_starters, "Only Paradox", startersJson);
    updateCheckboxImport(only_legends_and_paradox_starters, "Only Legends and Paradox", startersJson);

    QJsonObject starterIndividual = startersJson["Fuecoco"].toObject();
    updateCheckboxImport(starters_shiny[1], "Shiny", starterIndividual);
    updateQLineEditImport(starters[1], "Name", starterIndividual);
    updateComboBoxImport(starters_form[1], "Form", starterIndividual, 0);
    updateComboBoxImport(starters_pokeball[1], "Poke Ball", starterIndividual, 1);
    updateComboBoxImport(starters_gender[1], "Gender", starterIndividual, 1);

    starterIndividual = startersJson["Quaxly"].toObject();
    updateCheckboxImport(starters_shiny[2], "Shiny", starterIndividual);
    updateQLineEditImport(starters[2], "Name", starterIndividual);
    updateComboBoxImport(starters_form[2], "Form", starterIndividual, 0);
    updateComboBoxImport(starters_pokeball[2], "Poke Ball", starterIndividual, 2);
    updateComboBoxImport(starters_gender[2], "Gender", starterIndividual, 2);

    starterIndividual = startersJson["Sprigatito"].toObject();
    updateCheckboxImport(starters_shiny[0], "Shiny", starterIndividual);
    updateQLineEditImport(starters[0], "Name", starterIndividual);
    updateComboBoxImport(starters_form[0], "Form", starterIndividual, 0);
    updateComboBoxImport(starters_pokeball[0], "Poke Ball", starterIndividual, 0);
    updateComboBoxImport(starters_gender[0], "Gender", starterIndividual, 0);

    QJsonObject generations = startersJson["Generation"].toObject();
    updateCheckboxArrayImportGenerations(generations_starters, generations);

    updateSpinBoxImport(shiny_starter_rate, "Shiny Rate", startersJson);

    // Gifts
    QJsonObject giftsJson = settings["Gifts"].toObject();
    updateCheckboxImport(enable_gifts, "Randomize", giftsJson);
    updateCheckboxImport(randomize_tera_types, "Tera Types", giftsJson);
    updateSpinBoxImport(shiny_static_rate, "Shiny Rate", giftsJson);

    generations = giftsJson["Generation"].toObject();
    updateCheckboxArrayImportGenerations(generation_gifts, generations);

    // Stats
    QJsonObject statsJson = settings["Stats"].toObject();
    QJsonObject insideStats = statsJson["Abilities"].toObject();
    updateCheckboxImport(randomize_abilities, "Randomize", insideStats);
    updateCheckboxImport(ban_wonder_guard, "Ban Wonder Guard", insideStats);

    insideStats = statsJson["Base Stats"].toObject();
    updateCheckboxImport(randomize_bst, "Randomize", insideStats);
    updateCheckboxImport(keep_same_bst, "Keep Same BST", insideStats);

    insideStats = statsJson["Evolutions"].toObject();
    updateCheckboxImport(randomize_evolutions, "Randomize", insideStats);
    updateCheckboxImport(force_unobtainable_evolutions_at_night, "Fixed Regional Evolutions", insideStats);
    updateCheckboxImport(evolve_every_5_levels, "Evolve Every Level", insideStats);

    insideStats = statsJson["Moves"].toObject();
    updateCheckboxImport(randomize_moveset, "Randomize", insideStats);

    insideStats = statsJson["TMs"].toObject();
    updateCheckboxImport(enable_TMs, "Randomize", insideStats);
    updateCheckboxImport(allow_moves_without_animation, "Allow Moves w/out Animation", insideStats);

    insideStats = statsJson["Types"].toObject();
    updateCheckboxImport(randomize_types, "Randomize", insideStats);
    updateCheckboxImport(give_extra_types, "Grand Extra Types", insideStats);

    // Items
    QJsonObject itemsJson = settings["Items"].toObject();
    updateCheckboxImport(randomize_hidden_items, "Randomize Hidden Items", itemsJson);
    updateCheckboxImport(randomize_pickup_items, "Randomize PickUp Items", itemsJson);
    updateCheckboxImport(randomize_pokemon_drops, "Randomize Pokemon Drops", itemsJson);
    updateCheckboxImport(randomize_synchro_items, "Randomize Synchro Items", itemsJson);

    // Wilds
    QJsonObject wildsJson = settings["Wild Paldea"].toObject();
    updateCheckboxImport(randomize_paldea_wild, "Randomize", wildsJson);
    updateCheckboxImport(paldea_ExcludeLegends, "Exclude Legendary", wildsJson);
    updateCheckboxImport(paldea_OnlyLegends, "Only Legend", wildsJson);
    updateCheckboxImport(paldea_OnlyParadox, "Only Paradox", wildsJson);
    updateCheckboxImport(paldea_OnlyLegendsAndParadox, "Only Legends and Paradox", wildsJson);
    updateCheckboxImport(paldea_BanStage1, "Ban Stage 1", wildsJson);
    updateCheckboxImport(paldea_BanStage2, "Ban Stage 2", wildsJson);
    updateCheckboxImport(paldea_BanStage3, "Ban Stage 3", wildsJson);
    updateCheckboxImport(paldea_Ban1Stage, "Ban 1 Stage", wildsJson);
    updateCheckboxImport(paldea_Settings_for_all_wild, "Wild Paldea settings for All", wildsJson);
    updateCheckboxImport(let_ogre_pagos_spawn, "Ogerpon/Terapos Spawn", wildsJson);
    generations = wildsJson["Generation"].toObject();
    updateCheckboxArrayImportGenerations(generation_paldea_wild, generations);

    wildsJson = settings["Wild Kitakami"].toObject();
    updateCheckboxImport(randomize_kitakami_wild, "Randomize", wildsJson);
    updateCheckboxImport(kitakami_ExcludeLegends, "Exclude Legendary", wildsJson);
    updateCheckboxImport(kitakami_OnlyLegends, "Only Legend", wildsJson);
    updateCheckboxImport(kitakami_OnlyParadox, "Only Paradox", wildsJson);
    updateCheckboxImport(kitakami_OnlyLegendsAndParadox, "Only Legends and Paradox", wildsJson);
    updateCheckboxImport(kitakami_BanStage1, "Ban Stage 1", wildsJson);
    updateCheckboxImport(kitakami_BanStage2, "Ban Stage 2", wildsJson);
    updateCheckboxImport(kitakami_BanStage3, "Ban Stage 3", wildsJson);
    updateCheckboxImport(kitakami_Ban1Stage, "Ban 1 Stage", wildsJson);
    generations = wildsJson["Generation"].toObject();
    updateCheckboxArrayImportGenerations(generation_kitakami_wild, generations);

    wildsJson = settings["Wild Blueberry"].toObject();
    updateCheckboxImport(randomize_blueberry_wild, "Randomize", wildsJson);
    updateCheckboxImport(blueberry_ExcludeLegends, "Exclude Legendary", wildsJson);
    updateCheckboxImport(blueberry_OnlyLegends, "Only Legend", wildsJson);
    updateCheckboxImport(blueberry_OnlyParadox, "Only Paradox", wildsJson);
    updateCheckboxImport(blueberry_OnlyLegendsAndParadox, "Only Legends and Paradox", wildsJson);
    updateCheckboxImport(blueberry_BanStage1, "Ban Stage 1", wildsJson);
    updateCheckboxImport(blueberry_BanStage2, "Ban Stage 2", wildsJson);
    updateCheckboxImport(blueberry_BanStage3, "Ban Stage 3", wildsJson);
    updateCheckboxImport(blueberry_Ban1Stage, "Ban 1 Stage", wildsJson);
    generations = wildsJson["Generation"].toObject();
    updateCheckboxArrayImportGenerations(generation_blueberry_wild, generations);

    // Raids
    QJsonObject raidsJson = settings["Raids Paldea"].toObject();
    updateCheckboxImport(praids_randomize, "Randomize", raidsJson);
    updateCheckboxImport(praids_onlyLegends, "Only Legend", raidsJson);
    updateCheckboxImport(praids_onlyParadox, "Only Paradox", raidsJson);
    updateCheckboxImport(praids_onlyLegendsandParadox, "Only Legends and Paradox", raidsJson);
    updateCheckboxImport(praids_BanStage1, "Ban Stage 1", raidsJson);
    updateCheckboxImport(praids_BanStage2, "Ban Stage 2", raidsJson);
    updateCheckboxImport(praids_BanStage3, "Ban Stage 3", raidsJson);
    updateCheckboxImport(praids_Ban1Stage, "Ban 1 Stage", raidsJson);
    updateCheckboxImport(paldea_Settings_for_all_raids, "Raid Paldea settings for All", raidsJson);
    updateCheckboxImport(praids_force_shiny, "Force Shiny", raidsJson);
    updateSpinBoxImport(praids_shiny_chance, "Shiny Rate", raidsJson);
    generations = raidsJson["Generation"].toObject();
    updateCheckboxArrayImportGenerations(praidsgeneration, generations);

    raidsJson = settings["Raids Kitakami"].toObject();
    updateCheckboxImport(kraids_randomize, "Randomize", raidsJson);
    updateCheckboxImport(kraids_onlyLegends, "Only Legend", raidsJson);
    updateCheckboxImport(kraids_onlyParadox, "Only Paradox", raidsJson);
    updateCheckboxImport(kraids_onlyLegendsandParadox, "Only Legends and Paradox", raidsJson);
    updateCheckboxImport(kraids_BanStage1, "Ban Stage 1", raidsJson);
    updateCheckboxImport(kraids_BanStage2, "Ban Stage 2", raidsJson);
    updateCheckboxImport(kraids_BanStage3, "Ban Stage 3", raidsJson);
    updateCheckboxImport(kraids_Ban1Stage, "Ban 1 Stage", raidsJson);
    updateCheckboxImport(kraids_force_shiny, "Force Shiny", raidsJson);
    updateSpinBoxImport(kraids_shiny_chance, "Shiny Rate", raidsJson);
    generations = raidsJson["Generation"].toObject();
    updateCheckboxArrayImportGenerations(kraidsgeneration, generations);

    raidsJson = settings["Raids Blueberry"].toObject();
    updateCheckboxImport(braids_randomize, "Randomize", raidsJson);
    updateCheckboxImport(braids_onlyLegends, "Only Legend", raidsJson);
    updateCheckboxImport(braids_onlyParadox, "Only Paradox", raidsJson);
    updateCheckboxImport(braids_onlyLegendsandParadox, "Only Legends and Paradox", raidsJson);
    updateCheckboxImport(braids_BanStage1, "Ban Stage 1", raidsJson);
    updateCheckboxImport(braids_BanStage2, "Ban Stage 2", raidsJson);
    updateCheckboxImport(braids_BanStage3, "Ban Stage 3", raidsJson);
    updateCheckboxImport(braids_Ban1Stage, "Ban 1 Stage", raidsJson);
    updateCheckboxImport(braids_force_shiny, "Force Shiny", raidsJson);
    updateSpinBoxImport(braids_shiny_chance, "Shiny Rate", raidsJson);
    generations = raidsJson["Generation"].toObject();
    updateCheckboxArrayImportGenerations(braidsgeneration, generations);

    //Bosses
    QJsonObject bossesJson = settings["Bosses"].toObject();
    updateCheckboxImport(randomize_bosses, "Randomize", bossesJson);
    updateCheckboxImport(boss_settings[0], "Only Legend", bossesJson);
    updateCheckboxImport(boss_settings[1], "Only Paradox", bossesJson);
    updateCheckboxImport(boss_settings[2], "Only Legends and Paradox", bossesJson);
    updateCheckboxImport(boss_settings[3], "Ban Stage 1", bossesJson);
    updateCheckboxImport(boss_settings[4], "Ban Stage 2", bossesJson);
    updateCheckboxImport(boss_settings[5], "Ban Stage 3", bossesJson);
    updateCheckboxImport(boss_settings[6], "Ban 1 Stage", bossesJson);
    generations = bossesJson["Generation"].toObject();
    updateCheckboxArrayImportGenerations(boss_generation, generations);

    // Trainers
    QJsonObject trainersJson = settings["Trainers Paldea"].toObject();
    updateCheckboxImport(use_trainer_paldea_for_all, "Trainer Paldea Settings for All", trainersJson);

    QJsonObject nonGlobalTrainer = trainersJson["Champion"].toObject();
    updateCheckboxImport(pchampion_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(pchampion_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Elite 4"].toObject();
    updateCheckboxImport(pelite4_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(pelite4_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Gym"].toObject();
    updateCheckboxImport(pgym_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(pgym_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Raids"].toObject();
    updateCheckboxImport(praid_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(praid_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Rivals"].toObject();
    updateCheckboxImport(prival_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(prival_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Routes"].toObject();
    updateCheckboxImport(proute_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(proute_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    QJsonObject globalTrainer = trainersJson["Global"].toObject();
    updateCheckboxImport(pglobal_trainer_randomizer_settings[0], "Allow All to Tera", globalTrainer);
    updateCheckboxImport(pglobal_trainer_randomizer_settings[2], "Allow Shinies", globalTrainer);
    updateCheckboxImport(pglobal_trainer_randomizer_settings[4], "Doubles Only", globalTrainer);
    updateCheckboxImport(randomize_paldea_trainers, "Randomize", globalTrainer);
    updateCheckboxImport(pglobal_trainer_randomizer_settings[5], "Rival Settings for All", globalTrainer);
    updateCheckboxImport(pglobal_trainer_randomizer_settings[3], "Singles or Doubles", globalTrainer);
    updateCheckboxImport(pglobal_trainer_randomizer_settings[1], "Tera Types", globalTrainer);
    generations = globalTrainer["Generation"].toObject();
    updateCheckboxArrayImportGenerations(ptrainersgeneration, generations);

    // Trainers Kitakami
    trainersJson = settings["Trainers Kitakami"].toObject();

    nonGlobalTrainer = trainersJson["Rivals"].toObject();
    updateCheckboxImport(krival_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(krival_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Routes"].toObject();
    updateCheckboxImport(kroute_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(kroute_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Raids"].toObject();
    updateCheckboxImport(kraid_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(kraid_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Ogre Clan"].toObject();
    updateCheckboxImport(kogre_clan_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(kogre_clan_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    globalTrainer = trainersJson["Global"].toObject();
    updateCheckboxImport(kglobal_trainer_randomizer_settings[0], "Allow All to Tera", globalTrainer);
    updateCheckboxImport(kglobal_trainer_randomizer_settings[2], "Allow Shinies", globalTrainer);
    updateCheckboxImport(kglobal_trainer_randomizer_settings[4], "Doubles Only", globalTrainer);
    updateCheckboxImport(randomize_kitakami_trainers, "Randomize", globalTrainer);
    updateCheckboxImport(kglobal_trainer_randomizer_settings[5], "Rival Settings for All", globalTrainer);
    updateCheckboxImport(kglobal_trainer_randomizer_settings[3], "Singles or Doubles", globalTrainer);
    updateCheckboxImport(kglobal_trainer_randomizer_settings[1], "Tera Types", globalTrainer);
    generations = globalTrainer["Generation"].toObject();
    updateCheckboxArrayImportGenerations(ktrainersgeneration, generations);

    // Trainers Blueberry
    trainersJson = settings["Trainers Blueberry"].toObject();

    nonGlobalTrainer = trainersJson["Rivals"].toObject();
    updateCheckboxImport(brival_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(brival_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Routes"].toObject();
    updateCheckboxImport(broute_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(broute_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["Raids"].toObject();
    updateCheckboxImport(braid_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(braid_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    nonGlobalTrainer = trainersJson["BBClub"].toObject();
    updateCheckboxImport(b_bb4_randomizer[2], "Allow Tera", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[8], "Ban 1 Stage", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[9], "Ban Stage 1", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[10], "Ban Stage 2", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[11], "Ban Stage 3", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[3], "Force 6 IVs", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[0], "Force 6 Pokemon", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[1], "Give Extra Pokemon", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[4], "Make AI Smart", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[5], "Only Legend", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[6], "Only Paradox", nonGlobalTrainer);
    updateCheckboxImport(b_bb4_randomizer[7], "Only Legends and Paradox", nonGlobalTrainer);

    globalTrainer = trainersJson["Global"].toObject();
    updateCheckboxImport(bglobal_trainer_randomizer_settings[0], "Allow All to Tera", globalTrainer);
    updateCheckboxImport(bglobal_trainer_randomizer_settings[2], "Allow Shinies", globalTrainer);
    updateCheckboxImport(bglobal_trainer_randomizer_settings[4], "Doubles Only", globalTrainer);
    updateCheckboxImport(randomize_blueberry_trainers, "Randomize", globalTrainer);
    updateCheckboxImport(bglobal_trainer_randomizer_settings[5], "Rival Settings for All", globalTrainer);
    updateCheckboxImport(bglobal_trainer_randomizer_settings[3], "Singles or Doubles", globalTrainer);
    updateCheckboxImport(bglobal_trainer_randomizer_settings[1], "Tera Types", globalTrainer);
    generations = globalTrainer["Generation"].toObject();
    updateCheckboxArrayImportGenerations(btrainersgeneration, generations);
}

void SVRandomizerWindow::importSettings(){
    // Step 1: Open a dialog to select the JSON file to import
    QString filePath = QFileDialog::getOpenFileName(nullptr, "Select Settings File", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) {
        qDebug() << "No file selected.";
        return;
    }

    // Step 2: Open the file and read its contents
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading.";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // Step 3: Parse the JSON data and check validity
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qDebug() << "Invalid JSON format.";
        return;
    }

    settings = doc.object();
    setUpUISettings();
}

void SVRandomizerWindow::exportSettings(){
    // Step 1: Let the user select a directory to save the file
    QString defaultDir = QDir::currentPath();  // Get the current working directory
    QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                    "Choose File Name",
                                                    defaultDir + "/SVRandomizerSettings.json",
                                                    "JSON Files (*.json)");

    setUpJSONSettings();

    // Step 2: Write settings to the chosen file
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(settings);
        file.write(doc.toJson());
        file.close();
    } else {
        qDebug() << "Failed to open file for writing.";
    }
}

void SVRandomizerWindow::forceExportSettings(){
    // Step 1: Let the user select a directory to save the file

    setUpJSONSettings();
    QString logFilePath = "logs/settings_debug.json";

    // Step 2: Write settings to the chosen file
    QFile file(logFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(settings);
        file.write(doc.toJson());
        file.close();
    } else {
        qDebug() << "Failed to open file for writing.";
    }
}

void SVRandomizerWindow::setUpJSONSettings(){
    QJsonObject generation;
    /*
     * EARLY VERSION - WILL WORK ON IT TO IMPROVE LOGIC LATER ON
     */
    settings["Auto-Patch"] = randomizer.auto_patch;
    settings["Kaizo Mode"] = randomizer.kaizo_mode;
    settings["Seed"] = randomizer.seed;
    settings["Seed Numerical"] = QStringLiteral("%1").arg(hash);
    settings["Batch"] = bulk_amount->value();
    QJsonObject starters;
    starters["Randomize"] = randomizer.svRandomizerStarters.enable_starters;
    starters["Tera Types"] = randomizer.svRandomizerStarters.randomize_starters_tera_types;
    starters["All Shiny"] = randomizer.svRandomizerStarters.all_starters_shiny;
    starters["One Shiny"] = randomizer.svRandomizerStarters.force_shiny_starter;
    QJsonObject starterAlone;
    starterAlone["Shiny"] = randomizer.svRandomizerStarters.starters_shiny[0];
    starterAlone["Name"] = randomizer.svRandomizerStarters.starters[0];
    starterAlone["Form"] = randomizer.svRandomizerStarters.starters_forms[0];
    starterAlone["Gender"] = randomizer.svRandomizerStarters.starters_gender[0];
    starterAlone["Poke Ball"] = randomizer.svRandomizerStarters.starters_pokeball[0];
    starters["Sprigatito"] = starterAlone;
    starterAlone["Shiny"] = randomizer.svRandomizerStarters.starters_shiny[1];
    starterAlone["Name"] = randomizer.svRandomizerStarters.starters[1];
    starterAlone["Form"] = randomizer.svRandomizerStarters.starters_forms[1];
    starterAlone["Gender"] = randomizer.svRandomizerStarters.starters_gender[1];
    starterAlone["Poke Ball"] = randomizer.svRandomizerStarters.starters_pokeball[1];
    starters["Fuecoco"] = starterAlone;
    starterAlone["Shiny"] = randomizer.svRandomizerStarters.starters_shiny[2];
    starterAlone["Name"] = randomizer.svRandomizerStarters.starters[2];
    starterAlone["Form"] = randomizer.svRandomizerStarters.starters_forms[2];
    starterAlone["Gender"] = randomizer.svRandomizerStarters.starters_gender[2];
    starterAlone["Poke Ball"] = randomizer.svRandomizerStarters.starters_pokeball[2];
    starters["Quaxly"] = starterAlone;
    starters["Ban Stage 1"] = randomizer.svRandomizerStarters.ban_stage_1_pokemon_starters;
    starters["Ban Stage 2"] = randomizer.svRandomizerStarters.ban_stage_2_pokemon_starters;
    starters["Ban Stage 3"] = randomizer.svRandomizerStarters.ban_stage_3_pokemon_starters;
    starters["Ban 1 Stage"] = randomizer.svRandomizerStarters.ban_single_stage_pokemon_starters;
    starters["Only Legend"] = randomizer.svRandomizerStarters.only_legendary_pokemon_starters;
    starters["Only Paradox"] = randomizer.svRandomizerStarters.only_paradox_pokemon_starters;
    starters["Only Legends and Paradox"] =  randomizer.svRandomizerStarters.only_legends_and_paradox_starters;
    starters["Show Starters in Overworld"] = randomizer.svRandomizerStarters.show_starters_in_overworld;
    starters["Shiny Rate"] = shiny_starter_rate->value();
    generation["1"] = randomizer.svRandomizerStarters.generations_starters[0];
    generation["2"] = randomizer.svRandomizerStarters.generations_starters[1];
    generation["3"] = randomizer.svRandomizerStarters.generations_starters[2];
    generation["4"] = randomizer.svRandomizerStarters.generations_starters[3];
    generation["5"] = randomizer.svRandomizerStarters.generations_starters[4];
    generation["6"] = randomizer.svRandomizerStarters.generations_starters[5];
    generation["7"] = randomizer.svRandomizerStarters.generations_starters[6];
    generation["8"] = randomizer.svRandomizerStarters.generations_starters[7];
    generation["9"] = randomizer.svRandomizerStarters.generations_starters[8];
    starters["Generation"] = generation;
    settings["Starters"] = starters;

    QJsonObject gifts;
    gifts["Randomize"] = randomizer.svRandomizerStarters.enable_gifts;
    gifts["Tera Types"] = randomizer.svRandomizerStarters.randomize_tera_types;;
    gifts["Shiny Rate"] = randomizer.svRandomizerStarters.shiny_static_rate;
    // gifts["Ban Stage 1"] = false;
    // gifts["Ban Stage 2"] = false;
    // gifts["Ban Stage 3"] = false;
    // gifts["Ban 1 Stage"] = false;
    // gifts["Only Legend"] = false;
    // gifts["Only Paradox"] = false;
    // gifts["Only Legends and Paradox"] = false;
    generation["1"] = randomizer.svRandomizerStarters.generation_gifts[0];
    generation["2"] = randomizer.svRandomizerStarters.generation_gifts[1];
    generation["3"] = randomizer.svRandomizerStarters.generation_gifts[2];
    generation["4"] = randomizer.svRandomizerStarters.generation_gifts[3];
    generation["5"] = randomizer.svRandomizerStarters.generation_gifts[4];
    generation["6"] = randomizer.svRandomizerStarters.generation_gifts[5];
    generation["7"] = randomizer.svRandomizerStarters.generation_gifts[6];
    generation["8"] = randomizer.svRandomizerStarters.generation_gifts[7];
    generation["9"] = randomizer.svRandomizerStarters.generation_gifts[8];
    gifts["Generation"] = generation;
    settings["Gifts"] = gifts;

    QJsonObject stats;
    QJsonObject abilities;
    abilities["Randomize"] = randomizer.svRandomizerStats.randomize_abilities;
    abilities["Ban Wonder Guard"] = randomizer.svRandomizerStats.ban_wonder_guard;
    stats["Abilities"] = abilities;

    QJsonObject types;
    types["Randomize"] = randomizer.svRandomizerStats.randomize_types;
    types["Grand Extra Types"] = randomizer.svRandomizerStats.give_extra_types;
    stats["Types"] = types;

    QJsonObject moves;
    moves["Randomize"] = randomizer.svRandomizerStats.randomize_moveset;
    //moves["Allow Moves w/out Animation"] = false;
    moves["Match Moves to Types"] = randomizer.svRandomizerStats.moveset_same_as_type;
    stats["Moves"] = moves;

    QJsonObject bst;
    bst["Randomize"] = randomizer.svRandomizerStats.randomize_bst;
    bst["Keep Same BST"] = randomizer.svRandomizerStats.keep_same_bst;
    stats["Base Stats"] = bst;

    QJsonObject evos;
    evos["Randomize"] = randomizer.svRandomizerStats.randomize_evolutions;
    evos["Evolve Every Level"] = randomizer.svRandomizerStats.evolve_every_5_levels;
    evos["Fix Regional Evolutions"] = randomizer.svRandomizerStats.force_unobtainable_evolutions_at_night;
    //evos["Fix Trade Evolutions"] = false;
    stats["Evolutions"] = evos;

    QJsonObject tms;
    tms["Randomize"] = randomizer.svRandomizerStats.enable_TMs;
    tms["Allow Moves w/out Animation"] = randomizer.svRandomizerStats.allow_moves_without_animation;
    stats["TMs"] = tms;

    settings["Stats"] = stats;

    QJsonObject items;
    items["Randomize Hidden Items"] = randomizer.svRandomizerItems.randomize_hidden_items;
    items["Randomize PickUp Items"] = randomizer.svRandomizerItems.randomize_pickup_items;
    items["Randomize Synchro Items"] = randomizer.svRandomizerItems.randomize_synchro_items;
    items["Randomize Pokemon Drops"] = randomizer.svRandomizerItems.randomize_pokemon_drops;
    //items["Randomize Pokeball Items"] = false;
    settings["Items"] = items;

    QJsonObject wild;

    //Kitakami Wild
    wild["Randomize"] = randomizer.svRandomizerWilds.randomize_kitakami_wild;
    wild["Exclude Legendary"] = randomizer.svRandomizerWilds.kitakami_ExcludeLegends;
    wild["Ban Stage 1"] = randomizer.svRandomizerWilds.kitakami_BanStage1;
    wild["Ban Stage 2"] = randomizer.svRandomizerWilds.kitakami_BanStage2;
    wild["Ban Stage 3"] = randomizer.svRandomizerWilds.kitakami_BanStage3;
    wild["Ban 1 Stage"] = randomizer.svRandomizerWilds.kitakami_Ban1Stage;
    wild["Only Legend"] = randomizer.svRandomizerWilds.kitakami_OnlyLegends;
    wild["Only Paradox"] = randomizer.svRandomizerWilds.kitakami_OnlyParadox;
    wild["Only Legends and Paradox"] = randomizer.svRandomizerWilds.kitakami_OnlyLegendsAndParadox;
    //wild["Balance Area per BST"] = false;
    generation["1"] = randomizer.svRandomizerWilds.generation_kitakami_wild[0];
    generation["2"] = randomizer.svRandomizerWilds.generation_kitakami_wild[1];
    generation["3"] = randomizer.svRandomizerWilds.generation_kitakami_wild[2];
    generation["4"] = randomizer.svRandomizerWilds.generation_kitakami_wild[3];
    generation["5"] = randomizer.svRandomizerWilds.generation_kitakami_wild[4];
    generation["6"] = randomizer.svRandomizerWilds.generation_kitakami_wild[5];
    generation["7"] = randomizer.svRandomizerWilds.generation_kitakami_wild[6];
    generation["8"] = randomizer.svRandomizerWilds.generation_kitakami_wild[7];
    generation["9"] = randomizer.svRandomizerWilds.generation_kitakami_wild[8];
    wild["Generation"] = generation;
    settings["Wild Kitakami"] = wild;

    // Blueberry Wild
    wild["Randomize"] = randomizer.svRandomizerWilds.randomize_blueberry_wild;
    wild["Exclude Legendary"] = randomizer.svRandomizerWilds.blueberry_ExcludeLegends;
    wild["Ban Stage 1"] = randomizer.svRandomizerWilds.blueberry_BanStage1;
    wild["Ban Stage 2"] = randomizer.svRandomizerWilds.blueberry_BanStage2;
    wild["Ban Stage 3"] = randomizer.svRandomizerWilds.blueberry_BanStage3;
    wild["Ban 1 Stage"] = randomizer.svRandomizerWilds.blueberry_Ban1Stage;
    wild["Only Legend"] = randomizer.svRandomizerWilds.blueberry_OnlyLegends;
    wild["Only Paradox"] = randomizer.svRandomizerWilds.blueberry_OnlyParadox;
    wild["Only Legends and Paradox"] = randomizer.svRandomizerWilds.blueberry_OnlyLegendsAndParadox;
    //wild["Balance Area per BST"] = false;
    generation["1"] = randomizer.svRandomizerWilds.generation_blueberry_wild[0];
    generation["2"] = randomizer.svRandomizerWilds.generation_blueberry_wild[1];
    generation["3"] = randomizer.svRandomizerWilds.generation_blueberry_wild[2];
    generation["4"] = randomizer.svRandomizerWilds.generation_blueberry_wild[3];
    generation["5"] = randomizer.svRandomizerWilds.generation_blueberry_wild[4];
    generation["6"] = randomizer.svRandomizerWilds.generation_blueberry_wild[5];
    generation["7"] = randomizer.svRandomizerWilds.generation_blueberry_wild[6];
    generation["8"] = randomizer.svRandomizerWilds.generation_blueberry_wild[7];
    generation["9"] = randomizer.svRandomizerWilds.generation_blueberry_wild[8];
    wild["Generation"] = generation;
    settings["Wild Blueberry"] = wild;

    // Paldea Wild
    wild["Randomize"] = randomizer.svRandomizerWilds.randomize_paldea_wild;
    wild["Ogerpon/Terapos Spawn"] = randomizer.svRandomizerWilds.let_oger_pagos_spawn;
    wild["Exclude Legendary"] = randomizer.svRandomizerWilds.paldea_ExcludeLegends;
    wild["Ban Stage 1"] = randomizer.svRandomizerWilds.paldea_BanStage1;
    wild["Ban Stage 2"] = randomizer.svRandomizerWilds.paldea_BanStage2;
    wild["Ban Stage 3"] = randomizer.svRandomizerWilds.paldea_BanStage3;
    wild["Ban 1 Stage"] = randomizer.svRandomizerWilds.paldea_Ban1Stage;
    wild["Only Legend"] = randomizer.svRandomizerWilds.paldea_OnlyLegends;
    wild["Only Paradox"] = randomizer.svRandomizerWilds.paldea_OnlyParadox;
    wild["Only Legends and Paradox"] = randomizer.svRandomizerWilds.paldea_OnlyLegendsAndParadox;
    //wild["Balance Area per BST"] = false;
    generation["1"] = randomizer.svRandomizerWilds.generation_paldea_wild[0];
    generation["2"] = randomizer.svRandomizerWilds.generation_paldea_wild[1];
    generation["3"] = randomizer.svRandomizerWilds.generation_paldea_wild[2];
    generation["4"] = randomizer.svRandomizerWilds.generation_paldea_wild[3];
    generation["5"] = randomizer.svRandomizerWilds.generation_paldea_wild[4];
    generation["6"] = randomizer.svRandomizerWilds.generation_paldea_wild[5];
    generation["7"] = randomizer.svRandomizerWilds.generation_paldea_wild[6];
    generation["8"] = randomizer.svRandomizerWilds.generation_paldea_wild[7];
    generation["9"] = randomizer.svRandomizerWilds.generation_paldea_wild[8];
    wild["Generation"] = generation;
    wild["Wild Paldea settings for All"] = randomizer.svRandomizerWilds.paldea_Settings_for_all_wild;
    settings["Wild Paldea"] = wild;

    // For Statics
    // wild["Shiny Rate"] = shiny_static_rate->value();
    // settings["Statics"] = wild;

    QJsonObject raid;

    // Kitakami Raids
    raid["Randomize"] = randomizer.svRandomizerRaids.kraids_randomize;
    raid["Shiny Rate"] = praids_shiny_chance->value();
    raid["Ban Stage 1"] = randomizer.svRandomizerRaids.kraids_BanStage1;
    raid["Ban Stage 2"] = randomizer.svRandomizerRaids.kraids_BanStage2;
    raid["Ban Stage 3"] = randomizer.svRandomizerRaids.kraids_BanStage3;
    raid["Ban 1 Stage"] = randomizer.svRandomizerRaids.kraids_Ban1Stage;
    raid["Only Legend"] = randomizer.svRandomizerRaids.kraids_onlyLegends;
    raid["Only Paradox"] = randomizer.svRandomizerRaids.kraids_onlyParadox;
    raid["Only Legends and Paradox"] = randomizer.svRandomizerRaids.kraids_onlyLegendsandParadox;
    raid["Force Shiny"] = randomizer.svRandomizerRaids.kraids_force_shiny;
    generation["1"] = randomizer.svRandomizerRaids.kraidsgeneration[0];
    generation["2"] = randomizer.svRandomizerRaids.kraidsgeneration[1];
    generation["3"] = randomizer.svRandomizerRaids.kraidsgeneration[2];
    generation["4"] = randomizer.svRandomizerRaids.kraidsgeneration[3];
    generation["5"] = randomizer.svRandomizerRaids.kraidsgeneration[4];
    generation["6"] = randomizer.svRandomizerRaids.kraidsgeneration[5];
    generation["7"] = randomizer.svRandomizerRaids.kraidsgeneration[6];
    generation["8"] = randomizer.svRandomizerRaids.kraidsgeneration[7];
    generation["9"] = randomizer.svRandomizerRaids.kraidsgeneration[8];
    raid["Generation"] = generation;
    settings["Raids Kitakami"] = raid;

    // Blueberry Raid
    raid["Randomize"] = randomizer.svRandomizerRaids.braids_randomize;
    raid["Shiny Rate"] = braids_shiny_chance->value();
    raid["Ban Stage 1"] = randomizer.svRandomizerRaids.braids_BanStage1;
    raid["Ban Stage 2"] = randomizer.svRandomizerRaids.braids_BanStage2;
    raid["Ban Stage 3"] = randomizer.svRandomizerRaids.braids_BanStage3;
    raid["Ban 1 Stage"] = randomizer.svRandomizerRaids.braids_Ban1Stage;
    raid["Only Legend"] = randomizer.svRandomizerRaids.braids_onlyLegends;
    raid["Only Paradox"] = randomizer.svRandomizerRaids.braids_onlyParadox;
    raid["Only Legends and Paradox"] = randomizer.svRandomizerRaids.braids_onlyLegendsandParadox;
    raid["Force Shiny"] = randomizer.svRandomizerRaids.braids_force_shiny;
    generation["1"] = randomizer.svRandomizerRaids.braidsgeneration[0];
    generation["2"] = randomizer.svRandomizerRaids.braidsgeneration[1];
    generation["3"] = randomizer.svRandomizerRaids.braidsgeneration[2];
    generation["4"] = randomizer.svRandomizerRaids.braidsgeneration[3];
    generation["5"] = randomizer.svRandomizerRaids.braidsgeneration[4];
    generation["6"] = randomizer.svRandomizerRaids.braidsgeneration[5];
    generation["7"] = randomizer.svRandomizerRaids.braidsgeneration[6];
    generation["8"] = randomizer.svRandomizerRaids.braidsgeneration[7];
    generation["9"] = randomizer.svRandomizerRaids.braidsgeneration[8];
    raid["Generation"] = generation;
    settings["Raids Blueberry"] = raid;

    // Paldea Raid
    raid["Raid Paldea settings for All"] = false;
    raid["Randomize"] = randomizer.svRandomizerRaids.praids_randomize;
    raid["Shiny Rate"] = praids_shiny_chance->value();
    raid["Ban Stage 1"] = randomizer.svRandomizerRaids.praids_BanStage1;
    raid["Ban Stage 2"] = randomizer.svRandomizerRaids.praids_BanStage2;
    raid["Ban Stage 3"] = randomizer.svRandomizerRaids.praids_BanStage3;
    raid["Ban 1 Stage"] = randomizer.svRandomizerRaids.praids_Ban1Stage;
    raid["Only Legend"] = randomizer.svRandomizerRaids.praids_onlyLegends;
    raid["Only Paradox"] = randomizer.svRandomizerRaids.praids_onlyParadox;
    raid["Only Legends and Paradox"] = randomizer.svRandomizerRaids.praids_onlyLegendsandParadox;
    raid["Force Shiny"] = randomizer.svRandomizerRaids.praids_force_shiny;
    generation["1"] = randomizer.svRandomizerRaids.praidsgeneratio[0];
    generation["2"] = randomizer.svRandomizerRaids.praidsgeneratio[1];
    generation["3"] = randomizer.svRandomizerRaids.praidsgeneratio[2];
    generation["4"] = randomizer.svRandomizerRaids.praidsgeneratio[3];
    generation["5"] = randomizer.svRandomizerRaids.praidsgeneratio[4];
    generation["6"] = randomizer.svRandomizerRaids.praidsgeneratio[5];
    generation["7"] = randomizer.svRandomizerRaids.praidsgeneratio[6];
    generation["8"] = randomizer.svRandomizerRaids.praidsgeneratio[7];
    generation["9"] = randomizer.svRandomizerRaids.praidsgeneratio[8];
    raid["Generation"] = generation;
    settings["Raids Paldea"] = raid;

    QJsonObject trainers_global;
    QJsonObject trainers_paldea;
    QJsonObject trainers_kitakami;
    QJsonObject trainers_blueberry;
    QJsonObject trainers_settings;

    // Paldea Trainers
    trainers_global["Randomize"] = randomizer.svRandomizerTrainers.randomize_paldea_trainers;
    trainers_global["Allow All to Tera"] = randomizer.svRandomizerTrainers.pglobal_trainer_randomizer_settings[0];
    trainers_global["Tera Types"] = randomizer.svRandomizerTrainers.pglobal_trainer_randomizer_settings[1];
    trainers_global["Allow Shinies"] = randomizer.svRandomizerTrainers.pglobal_trainer_randomizer_settings[2];
    trainers_global["Singles or Doubles"] = randomizer.svRandomizerTrainers.pglobal_trainer_randomizer_settings[3];
    trainers_global["Doubles Only"] = randomizer.svRandomizerTrainers.pglobal_trainer_randomizer_settings[4];
    trainers_global["Rival Settings for All"] = randomizer.svRandomizerTrainers.pglobal_trainer_randomizer_settings[5];
    generation["1"] = randomizer.svRandomizerTrainers.ptrainersgeneration[0];
    generation["2"] = randomizer.svRandomizerTrainers.ptrainersgeneration[1];
    generation["3"] = randomizer.svRandomizerTrainers.ptrainersgeneration[2];
    generation["4"] = randomizer.svRandomizerTrainers.ptrainersgeneration[3];
    generation["5"] = randomizer.svRandomizerTrainers.ptrainersgeneration[4];
    generation["6"] = randomizer.svRandomizerTrainers.ptrainersgeneration[5];
    generation["7"] = randomizer.svRandomizerTrainers.ptrainersgeneration[6];
    generation["8"] = randomizer.svRandomizerTrainers.ptrainersgeneration[7];
    generation["9"] = randomizer.svRandomizerTrainers.ptrainersgeneration[8];
    trainers_global["Generation"] = generation;
    trainers_paldea["Global"] = trainers_global;
    trainers_paldea["Trainer Paldea Settings for All"] = randomizer.svRandomizerTrainers.use_trainer_paldea_for_all;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.prival_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.prival_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.prival_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.prival_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.prival_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.prival_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.prival_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.prival_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.prival_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.prival_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.prival_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.prival_randomizer[7];
    trainers_paldea["Rivals"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.proute_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.proute_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.proute_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.proute_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.proute_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.proute_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.proute_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.proute_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.proute_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.proute_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.proute_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.proute_randomizer[7];
    trainers_paldea["Routes"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.pgym_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.pgym_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.pgym_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.pgym_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.pgym_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.pgym_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.pgym_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.pgym_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.pgym_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.pgym_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.pgym_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.pgym_randomizer[7];
    trainers_paldea["Gym"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.pelite4_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.pelite4_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.pelite4_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.pelite4_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.pelite4_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.pelite4_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.pelite4_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.pelite4_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.pelite4_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.pelite4_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.pelite4_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.pelite4_randomizer[7];
    trainers_paldea["Elite 4"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.pchampion_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.pchampion_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.pchampion_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.pchampion_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.pchampion_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.pchampion_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.pchampion_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.pchampion_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.pchampion_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.pchampion_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.pchampion_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.pchampion_randomizer[7];
    trainers_paldea["Champion"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.praid_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.praid_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.praid_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.praid_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.praid_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.praid_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.praid_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.praid_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.praid_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.praid_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.praid_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.praid_randomizer[7];
    trainers_paldea["Raids"] = trainers_settings;

    // Kitakami Trainers

    trainers_global["Randomize"] = randomizer.svRandomizerTrainers.randomize_kitakami_trainers;
    trainers_global["Allow All to Tera"] = randomizer.svRandomizerTrainers.kglobal_trainer_randomizer_settings[0];
    trainers_global["Tera Types"] = randomizer.svRandomizerTrainers.kglobal_trainer_randomizer_settings[1];
    trainers_global["Allow Shinies"] = randomizer.svRandomizerTrainers.kglobal_trainer_randomizer_settings[2];
    trainers_global["Singles or Doubles"] = randomizer.svRandomizerTrainers.kglobal_trainer_randomizer_settings[3];
    trainers_global["Doubles Only"] = randomizer.svRandomizerTrainers.kglobal_trainer_randomizer_settings[4];
    trainers_global["Rival Settings for All"] = randomizer.svRandomizerTrainers.kglobal_trainer_randomizer_settings[5];
    generation["1"] = randomizer.svRandomizerTrainers.ktrainersgeneration[0];
    generation["2"] = randomizer.svRandomizerTrainers.ktrainersgeneration[1];
    generation["3"] = randomizer.svRandomizerTrainers.ktrainersgeneration[2];
    generation["4"] = randomizer.svRandomizerTrainers.ktrainersgeneration[3];
    generation["5"] = randomizer.svRandomizerTrainers.ktrainersgeneration[4];
    generation["6"] = randomizer.svRandomizerTrainers.ktrainersgeneration[5];
    generation["7"] = randomizer.svRandomizerTrainers.ktrainersgeneration[6];
    generation["8"] = randomizer.svRandomizerTrainers.ktrainersgeneration[7];
    generation["9"] = randomizer.svRandomizerTrainers.ktrainersgeneration[8];
    trainers_global["Generation"] = generation;
    trainers_kitakami["Global"] = trainers_global;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.krival_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.krival_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.krival_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.krival_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.krival_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.krival_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.krival_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.krival_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.krival_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.krival_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.krival_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.krival_randomizer[7];
    trainers_kitakami["Rivals"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.kroute_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.kroute_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.kroute_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.kroute_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.kroute_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.kroute_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.kroute_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.kroute_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.kroute_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.kroute_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.kroute_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.kroute_randomizer[7];
    trainers_kitakami["Routes"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.kogre_clan_randomizer[7];
    trainers_kitakami["Ogre Clan"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.kraid_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.kraid_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.kraid_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.kraid_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.kraid_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.kraid_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.kraid_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.kraid_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.kraid_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.kraid_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.kraid_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.kraid_randomizer[7];
    trainers_kitakami["Raids"] = trainers_settings;

    // Blueberry Trainers
    trainers_global["Randomize"] = randomizer.svRandomizerTrainers.randomize_blueberry_trainers;
    trainers_global["Allow All to Tera"] = randomizer.svRandomizerTrainers.bglobal_trainer_randomizer_settings[0];
    trainers_global["Tera Types"] = randomizer.svRandomizerTrainers.bglobal_trainer_randomizer_settings[1];
    trainers_global["Allow Shinies"] = randomizer.svRandomizerTrainers.bglobal_trainer_randomizer_settings[2];
    trainers_global["Singles or Doubles"] = randomizer.svRandomizerTrainers.bglobal_trainer_randomizer_settings[3];
    trainers_global["Doubles Only"] = randomizer.svRandomizerTrainers.bglobal_trainer_randomizer_settings[4];
    trainers_global["Rival Settings for All"] = randomizer.svRandomizerTrainers.bglobal_trainer_randomizer_settings[5];
    generation["1"] = randomizer.svRandomizerTrainers.btrainersgeneration[0];
    generation["2"] = randomizer.svRandomizerTrainers.btrainersgeneration[1];
    generation["3"] = randomizer.svRandomizerTrainers.btrainersgeneration[2];
    generation["4"] = randomizer.svRandomizerTrainers.btrainersgeneration[3];
    generation["5"] = randomizer.svRandomizerTrainers.btrainersgeneration[4];
    generation["6"] = randomizer.svRandomizerTrainers.btrainersgeneration[5];
    generation["7"] = randomizer.svRandomizerTrainers.btrainersgeneration[6];
    generation["8"] = randomizer.svRandomizerTrainers.btrainersgeneration[7];
    generation["9"] = randomizer.svRandomizerTrainers.btrainersgeneration[8];
    trainers_global["Generation"] = generation;
    trainers_blueberry["Global"] = trainers_global;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.brival_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.brival_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.brival_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.brival_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.brival_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.brival_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.brival_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.brival_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.brival_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.brival_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.brival_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.brival_randomizer[7];
    trainers_blueberry["Rivals"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.broute_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.broute_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.broute_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.broute_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.broute_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.broute_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.broute_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.broute_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.broute_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.broute_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.broute_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.broute_randomizer[7];
    trainers_blueberry["Routes"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.b_bb4_randomizer[7];
    trainers_blueberry["BBClub"] = trainers_settings;

    trainers_settings["Force 6 Pokemon"] = randomizer.svRandomizerTrainers.braid_randomizer[0];
    trainers_settings["Give Extra Pokemon"] = randomizer.svRandomizerTrainers.braid_randomizer[1];
    trainers_settings["Allow Tera"] = randomizer.svRandomizerTrainers.braid_randomizer[2];
    trainers_settings["Force 6 IVs"] = randomizer.svRandomizerTrainers.braid_randomizer[3];
    trainers_settings["Make AI Smart"] = randomizer.svRandomizerTrainers.braid_randomizer[4];
    trainers_settings["Ban Stage 1"] = randomizer.svRandomizerTrainers.braid_randomizer[8];
    trainers_settings["Ban Stage 2"] = randomizer.svRandomizerTrainers.braid_randomizer[9];
    trainers_settings["Ban Stage 3"] = randomizer.svRandomizerTrainers.braid_randomizer[10];
    trainers_settings["Ban 1 Stage"] = randomizer.svRandomizerTrainers.braid_randomizer[11];
    trainers_settings["Only Legend"] = randomizer.svRandomizerTrainers.braid_randomizer[5];
    trainers_settings["Only Paradox"] = randomizer.svRandomizerTrainers.braid_randomizer[6];
    trainers_settings["Only Legends and Paradox"] = randomizer.svRandomizerTrainers.braid_randomizer[7];
    trainers_blueberry["Raids"] = trainers_settings;

    settings["Trainers Paldea"] = trainers_paldea;
    settings["Trainers Kitakami"] = trainers_kitakami;
    settings["Trainers Blueberry"] = trainers_blueberry;

    QJsonObject boss;
    boss["Randomize"] = randomizer.svRandomizerBoss.randomize_bosses;
    boss["Ban Stage 1"] = randomizer.svRandomizerBoss.boss_settings[3];
    boss["Ban Stage 2"] = randomizer.svRandomizerBoss.boss_settings[4];
    boss["Ban Stage 3"] = randomizer.svRandomizerBoss.boss_settings[5];
    boss["Ban 1 Stage"] = randomizer.svRandomizerBoss.boss_settings[6];
    boss["Only Legend"] = randomizer.svRandomizerBoss.boss_settings[0];
    boss["Only Paradox"] = randomizer.svRandomizerBoss.boss_settings[1];
    boss["Only Legends and Paradox"] = randomizer.svRandomizerBoss.boss_settings[2];
    generation["1"] = randomizer.svRandomizerBoss.boss_generation[0];
    generation["2"] = randomizer.svRandomizerBoss.boss_generation[1];
    generation["3"] = randomizer.svRandomizerBoss.boss_generation[2];
    generation["4"] = randomizer.svRandomizerBoss.boss_generation[3];
    generation["5"] = randomizer.svRandomizerBoss.boss_generation[4];
    generation["6"] = randomizer.svRandomizerBoss.boss_generation[5];
    generation["7"] = randomizer.svRandomizerBoss.boss_generation[6];
    generation["8"] = randomizer.svRandomizerBoss.boss_generation[7];
    generation["9"] = randomizer.svRandomizerBoss.boss_generation[8];
    boss["Generation"] = generation;
    settings["Bosses"] = boss;

}
