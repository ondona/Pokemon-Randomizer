#include "headers/sv_randomizer_headers/SVRandomizerWindow.h"

SVRandomizerWindow::SVRandomizerWindow(QWidget *parent)
    : QWidget(parent)
{
    // Things to initialize on constructor.
    starterNames = {"Sprigatito", "Fuecoco", "Quaxly"};
}

SVRandomizerWindow::~SVRandomizerWindow()
{
    // Cleanup if needed
}

// Main Layout
void SVRandomizerWindow::createLayout()
{
    // Initialize Settings from Virtual Functions
    initializeSettings();

    // Create the main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // ------------------------General Settings Section------------------------
        QGroupBox *generalGroup = new QGroupBox("General Settings Section");
        QVBoxLayout *generalSettings = new QVBoxLayout(generalGroup);

        // Horizontal layout for side-by-side questions and inputs in singleCheckboxGroup
        QHBoxLayout *mainSettings = new QHBoxLayout();
        QHBoxLayout *extraSettings = new QHBoxLayout();
        QHBoxLayout *seedSettings = new QHBoxLayout();
        QHBoxLayout *settingsLocal = new QHBoxLayout();

        extraSettings->setContentsMargins(0, 0, 0, 0);
        extraSettings->setSpacing(0);

        QCheckBox * auto_patch = new QCheckBox("Auto Patch Randomizer ", generalGroup);
        auto_patch->setChecked(true);
        mainSettings->addWidget(auto_patch);
        //connect(auto_patch, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        QCheckBox *kaizo_mode = new QCheckBox("Kaizo Randomizer", generalGroup);
        mainSettings->addWidget(kaizo_mode);
       // connect(kaizo_mode, &QCheckBox::toggled, this, &SVRandomizerWindow::saveCheckboxState);

        QLabel *bulk_question = new QLabel("Randomizers to Create ", generalGroup);
        bulk_question->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        QSpinBox * bulk_amount= new QSpinBox(generalGroup);  // Number input
        bulk_amount->setRange(1, 100);  // Set range for the number input
        bulk_amount->setValue(1);
        //connect(bulk_amount, QOverload<int>::of(&QSpinBox::valueChanged), this, &SVRandomizerWindow::saveSpinBoxValue);
        bulk_amount->setMaximumWidth(100);

        extraSettings->addWidget(bulk_question);
        extraSettings->addWidget(bulk_amount);

        // Add the horizontal layout to the group
        extraSettings->addStretch(1);  // Pushes widgets to the left

        QPushButton * importButton = new QPushButton("Import Settings and Seed", generalGroup);
        connect(importButton, &QPushButton::clicked, this, [=, this]() mutable{
            emit importSettings();
        });

        settingsLocal->addWidget(importButton);

        QPushButton * exportButton = new QPushButton("Export Settings and Seed", generalGroup);
        connect(exportButton, &QPushButton::clicked, this, [=, this]() mutable{
            QString defaultDir = QDir::currentPath();  // Get the current working directory
            QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                            "Choose File Name",
                                                            defaultDir + "/SVRandomizerSettings-Test.json",
                                                            "JSON Files (*.json)");

            QJsonDocument settingsJson = exportSettings(settings);

            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(settingsJson.toJson());
                file.close();
            } else {
                qDebug() << "Failed to open file for writing.";
            }
        });

        settingsLocal->addWidget(exportButton);

        QLineEdit *seed = new QLineEdit(generalGroup);
        //connect(seed, &QLineEdit::textChanged, this, &SVRandomizerWindow::saveStringInput);

        QLabel *seedsets = new QLabel("Seed ", generalGroup);
        seedsets->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        seedSettings->addWidget(seedsets);

        seed->setFixedWidth(348);
        seed->setPlaceholderText("Leave Blank to use time... (no limit)");
        seedSettings->addWidget(seed);

        QPushButton * startRandomizer = new QPushButton("Start Randomizing", generalGroup);
       // connect(startRandomizer, &QPushButton::clicked, this, &SVRandomizerWindow::runRandomizer);
        seedSettings->addWidget(startRandomizer);

        generalSettings->addLayout(mainSettings);
        generalSettings->addLayout(extraSettings);
        generalSettings->addLayout(settingsLocal);
        generalSettings->addLayout(seedSettings);

    mainLayout->addWidget(generalGroup);
    // Configure the tab bar
    QTabBar * topBar = new QTabBar(this);
    topBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Expand horizontally
    topBar->addTab("Starter/Gift Pokemon");
    topBar->addTab("Stats/TMs/Items");
    topBar->addTab("Wild/Static Pokemon");
    topBar->addTab("Raids");
    topBar->addTab("Trainers");
    topBar->addTab("Bosses");
    //topBar->addTab("Shop");

    // // Adjust the minimum width to a specific value to ensure it expands
    topBar->setMinimumWidth(800); // Set a minimum width (adjust as necessary)

    mainLayout->addWidget(topBar); // Add the tab bar to the layout

    // // Configure the stacked widget
    QStackedWidget * stackedWidget = new QStackedWidget(this);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Allow full expansion
    mainLayout->addWidget(stackedWidget);

    // Add each widget to the stacked widget
    stackedWidget->addWidget(setupStartersWidget());
    stackedWidget->addWidget(setupPersonalWidget());
    stackedWidget->addWidget(setupWildWidget());
    // stackedWidget->addWidget(setupRaidsWidget());
    // stackedWidget->addWidget(setupTrainersWidget());
    // stackedWidget->addWidget(setupScenesWidget());
    // //stackedWidget->addWidget(setupShopWidget());

    connect(topBar, &QTabBar::currentChanged, this, [=, this](int index) {
        switchTabs(stackedWidget, index);
    });

    // Set the stretch factors
    mainLayout->setStretch(2, 0); // Prevent tab bar from expanding vertically
    mainLayout->setStretch(3, 1); // Make the stacked widget take up the remaining space

    // Apply the main layout to RandomizerWindow
    setLayout(mainLayout);
}

// Helper Functions
QJsonDocument SVRandomizerWindow::exportSettings(QMap<QString, QVariant> map){
    QJsonObject jsonObj;
    for (auto it = map.begin(); it != map.end(); ++it) {
        const QString &key = it.key();
        const QVariant &value = it.value();

        // Convert QVariant to appropriate QJsonValue
        if (value.typeId() == QMetaType::Bool) {
            jsonObj.insert(key, value.toBool());
        } else if (value.typeId() == QMetaType::Int) {
            jsonObj.insert(key, value.toInt());
        } else if (value.typeId() == QMetaType::Double) {
            jsonObj.insert(key, value.toDouble());
        } else if (value.typeId() == QMetaType::QString) {
            jsonObj.insert(key, value.toString());
        } else if (value.typeId() == QMetaType::QVariantList) {
            jsonObj.insert(key, QJsonArray::fromVariantList(value.toList()));
        } else if (value.typeId() == QMetaType::QVariantMap) {
            jsonObj.insert(key, exportSettings(value.toMap()).object()); // Recursive call for nested maps
        } else {
            qDebug() << "Unsupported QVariant type for key:" << key << "with type:" << value.typeName();
        }
    }

    return QJsonDocument(jsonObj);
};

void SVRandomizerWindow::createStarterPokemonSelection(QString starter, QStringList allowedPokemon,
                                                               QStringList allowedPokeballs, QGroupBox* groupToAdd,
                                                               QHBoxLayout* rowToAdd, QVBoxLayout* layoutToAdd, int index){

    QCheckBox* starters_shiny = new QCheckBox("Shiny ", groupToAdd);
    rowToAdd->addWidget(starters_shiny);
    connect(starters_shiny, &QCheckBox::toggled, this, [this, index, starter](bool checked) mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        startersSettings["Shiny"] = checked;
        randomizer.svRandomizerStarters.starters_shiny[index] = checked;
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    QLabel *starterLabel = new QLabel(starter, groupToAdd);
    starterLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rowToAdd->addWidget(starterLabel);

    QLineEdit* starters = new QLineEdit(groupToAdd);

    // Sample list of suggestions
    QCompleter *completer = new QCompleter(allowedPokemon, starters);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    starters->setCompleter(completer);
    // Connect to save pokemon
    connect(starters, &QLineEdit::textChanged, this, [this, index, starter](const QString &text) mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        startersSettings["Name"] = text;
        randomizer.svRandomizerStarters.starters[index] = text;
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    starters->setFixedWidth(175);
    starters->setPlaceholderText("Leave Blank for Random...");
    rowToAdd->addWidget(starters);

    QLabel *form_s1 = new QLabel("Form ", groupToAdd);
    form_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rowToAdd->addWidget(form_s1);

    QComboBox* starters_form = new QComboBox(groupToAdd);
    starters_form->addItem("---");
    starters_form->setFixedSize(100, 25);

    rowToAdd->addWidget(starters_form);

    // Double connect to change forms based on pokemon selected
    connect(starters, &QLineEdit::textChanged, this, [=, this](const QString &text) mutable {
        starters_form->clear();
        for(int i = 0; i< randomizer.pokemonFormsInGame[text].size(); i++){
            starters_form->addItem(randomizer.pokemonFormsInGame[text][i]);
        }

    });
    connect(starters_form, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=, this]() mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        int setForm = 0;
        for(int i =0; i< randomizer.pokemonFormsInGame[randomizer.svRandomizerStarters.starters[index]].size(); i++){
            if(starters_form->currentText() == randomizer.pokemonFormsInGame[randomizer.svRandomizerStarters.starters[index]][i]){
                setForm = i;
                if(randomizer.svRandomizerStarters.starters[index] == "Pikachu" && setForm == 8){
                    setForm = 9;
                }
            }
        }
        startersSettings["Form"] = setForm;
        randomizer.svRandomizerStarters.starters_forms[index] = setForm;
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    QLabel *gender_s1 = new QLabel("Gender ", groupToAdd);
    gender_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rowToAdd->addWidget(gender_s1);

    QComboBox* starters_gender = new QComboBox(groupToAdd);
    starters_gender->addItem("---");
    starters_gender->setFixedSize(100, 25);

    rowToAdd->addWidget(starters_gender);

    // Triple connect to adjust for a pokemon's gender varying based on form and the last one is to save it.
    connect(starters, &QLineEdit::textChanged, this, [=, this](const QString &text) mutable{
        starters_gender->clear();
        if(randomizer.genderForms.contains(text)){
            starters_gender->addItem("DEFAULT");
        }else if(randomizer.femaleOnlyPokemon.contains(text)){
            starters_gender->addItem("FEMALE");
        }else if(randomizer.maleOnlyPokemon.contains(text) || randomizer.formsMaleOnly[text].contains(index)){
            starters_gender->addItem("MALE");
        } else if(randomizer.genderlessPokemon.contains(text)){
            starters_gender->addItem("GENDERLESS");
        } else{
            starters_gender->addItem("MALE");
            starters_gender->addItem("FEMALE");
        }
    });
    connect(starters_form, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=, this]() mutable{
        starters_gender->clear();
        if(randomizer.genderForms.contains(randomizer.svRandomizerStarters.starters[index])){
            starters_gender->addItem("DEFAULT");
        }else if(randomizer.femaleOnlyPokemon.contains(randomizer.svRandomizerStarters.starters[index])){
            starters_gender->addItem("FEMALE");
        }else if(randomizer.maleOnlyPokemon.contains(randomizer.svRandomizerStarters.starters[index]) || randomizer.formsMaleOnly[randomizer.svRandomizerStarters.starters[index]].contains(randomizer.svRandomizerStarters.starters_forms[index])){
            starters_gender->addItem("MALE");
        } else if(randomizer.genderlessPokemon.contains(randomizer.svRandomizerStarters.starters[index])){
            starters_gender->addItem("GENDERLESS");
        } else{
            starters_gender->addItem("MALE");
            starters_gender->addItem("FEMALE");
        }
    });
    connect(starters_gender, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=, this]() mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        startersSettings["Gender"] = starters_gender->currentText();
        randomizer.svRandomizerStarters.starters_gender[index] = starters_gender->currentText();
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    QLabel *pokeball_s1 = new QLabel("Pokeball ", groupToAdd);
    pokeball_s1->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rowToAdd->addWidget(pokeball_s1);

    // Update the combobox to match allowed pokeballs
    QComboBox* starters_pokeball = new QComboBox(groupToAdd);
    for(int i = 0; i<allowedPokeballs.length(); i++){
        starters_pokeball->addItem(allowedPokeballs[i]);
    }
    starters_pokeball->setFixedSize(100, 25);
    // Connect to save the pokeball
    connect(starters_pokeball, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, &starters_pokeball, starter, index]() mutable{
        QMap<QString, QVariant> localSettings = settings["Starters"].toMap();
        QMap<QString, QVariant> startersSettings = localSettings[starter].toMap();

        startersSettings["Pokeball"] = starters_pokeball->currentText();
        randomizer.svRandomizerStarters.starters_pokeball[index] = starters_pokeball->currentText();
        localSettings[starter] = startersSettings;
        settings["Starters"] = localSettings;
    });

    rowToAdd->addWidget(starters_pokeball);

    layoutToAdd->addLayout(rowToAdd);
    rowToAdd->addStretch(1);

    // Connect based on imports
    connect(this, &SVRandomizerWindow::importSettings, this, [=, this]() mutable{
        qDebug()<<"Here";
    });
}

void SVRandomizerWindow::setupAllowedPokemon(QVBoxLayout *outerBox, allowedPokemonLimiter limiter){
    QGroupBox* limiterGroup = new QGroupBox("Pokemon Limiter", this);
    outerBox->addWidget(limiterGroup);

    QVBoxLayout* limiterSetUp = new QVBoxLayout(limiterGroup);
    QHBoxLayout* generations = new QHBoxLayout();
    QHBoxLayout* legendaries = new QHBoxLayout();
    QHBoxLayout* stages = new QHBoxLayout();
    QHBoxLayout* special = new QHBoxLayout();

    QVector<QCheckBox*> generationslist;

    QLabel *generationsHeader = new QLabel("Allowed Generations", limiterGroup);
    generationsHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    generations->addWidget(generationsHeader);

    for(int i =0; i<9; i++){
        generationslist.append(new QCheckBox(QStringLiteral("%1").arg(i+1), limiterGroup));
        generationslist[i]->setChecked(true);
        generationslist[i]->setFixedSize(60, 20);
        generations->addWidget(generationslist[i]);
        // Connects Here
    }
    limiterSetUp->addLayout(generations);

    //-------------------------------------------------------------------------------------------

    QVector<QCheckBox*> legendariesList;

    QLabel *legendsHeader = new QLabel("Allowed Legendaries", limiterGroup);
    legendsHeader->setStyleSheet("font-weight: bold; padding: 0px 0;");
    legendaries->addWidget(legendsHeader);

    for(int i =0; i<9; i++){
        legendariesList.append(new QCheckBox(QStringLiteral("%1").arg(i+1), limiterGroup));
        legendariesList[i]->setChecked(true);
        legendariesList[i]->setFixedSize(60, 20);
        legendaries->addWidget(legendariesList[i]);
        // Connects Here
    }
    limiterSetUp->addLayout(legendaries);

    //-------------------------------------------------------------------------------------------

    QCheckBox *stage_1 = new QCheckBox("Stage 1", limiterGroup);
    stage_1->setChecked(true);
    stages->addWidget(stage_1);

    QCheckBox *stage_2 = new QCheckBox("Stage 2", limiterGroup);
    stage_2->setChecked(true);
    stages->addWidget(stage_2);

    QCheckBox *stage_3 = new QCheckBox("Stage 3", limiterGroup);
    stage_3->setChecked(true);
    stages->addWidget(stage_3);

    QCheckBox *single_stage = new QCheckBox("Single stage", limiterGroup);
    single_stage->setChecked(true);
    stages->addWidget(single_stage);

    limiterSetUp->addLayout(stages);

    //-------------------------------------------------------------------------------------------

    QCheckBox *paradox = new QCheckBox("Paradox", limiterGroup);
    paradox->setChecked(true);
    special->addWidget(paradox);
    limiterSetUp->addLayout(special);

}


// Specific Widgets Codes
QScrollArea* SVRandomizerWindow::setupStartersWidget(){
    QWidget *startersWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(startersWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Add Actual Buttons
    formLayout->addRow(createStartersWidget());
    formLayout->addRow(createGiftsWidget());

    // Create Layouts
    mainLayout->addLayout(formLayout);
    scrollArea->setWidget(startersWidget);

    return scrollArea;
}

QScrollArea* SVRandomizerWindow::setupPersonalWidget(){
    QWidget *personalsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(personalsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Add Actual Buttons
    formLayout->addRow(createAbilitiesWidget());
    formLayout->addRow(createTypesWidget());
    formLayout->addRow(createMovesetWidget());
    formLayout->addRow(createBSTWidget());
    formLayout->addRow(createEvolutionsWidget());
    formLayout->addRow(createTMWidget());
    formLayout->addRow(createItemWidget());

    // Create Layouts
    mainLayout->addLayout(formLayout);
    scrollArea->setWidget(personalsWidget);

    return scrollArea;
}

QScrollArea* SVRandomizerWindow::setupWildWidget(){
    QWidget *wildsWidget = new QWidget();

    // Main layout with no extra margins or spacing
    QVBoxLayout *mainLayout = new QVBoxLayout(wildsWidget);

    // Create scroll area and set properties
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);  // Enable resizing with the window

    // Create form layout
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(2);
    formLayout->setContentsMargins(2, 2, 2, 2);

    // Add Actual Buttons
    // formLayout->addRow(createStartersWidget());
    // formLayout->addRow(createGiftsWidget());

    // Create Layouts
    mainLayout->addLayout(formLayout);
    scrollArea->setWidget(wildsWidget);

    return scrollArea;

}

// Creation of Widget
QVBoxLayout* SVRandomizerWindow::createStartersWidget(){
    QVBoxLayout *starterSettingLayout = new QVBoxLayout();

    // Creates the Main Button
    QCheckBox* enable_starters = new QCheckBox("Randomize Starters");
    starterSettingLayout->addWidget(enable_starters);
    // Connect for Setting the values
    connect(enable_starters, &QCheckBox::toggled, this, [this](bool checked) mutable{
        QMap<QString, QVariant> startersSettings = settings["Starters"].toMap();

        startersSettings["Randomize"] = checked;
        randomizer.svRandomizerStarters.enable_starters = checked;
        settings["Starters"] = startersSettings;
    });

    // Creates Hidden Group based on button
    QGroupBox *startersGroupSettings= new QGroupBox("Starter Settings Section");
    QVBoxLayout *startersSettingsLayout = new QVBoxLayout(startersGroupSettings);
    QHBoxLayout *startersRow_Q0 = new QHBoxLayout();
    QHBoxLayout *startersRow_Q1 = new QHBoxLayout();
    QHBoxLayout *startersRow_Q2 = new QHBoxLayout();
    QHBoxLayout *startersRow_Q3 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    startersGroupSettings->setVisible(false);
    connect(enable_starters, &QCheckBox::toggled, startersGroupSettings, &QGroupBox::setVisible);

    starterSettingLayout->addWidget(startersGroupSettings);

    // Row 0
    QCheckBox* randomize_starters_tera_types = new QCheckBox("Randomize Tera Types ", startersGroupSettings);
    startersRow_Q0->addWidget(randomize_starters_tera_types);
    // Connect for Setting the values
    connect(randomize_starters_tera_types, &QCheckBox::toggled, this, [this](bool checked) mutable{
        QMap<QString, QVariant> startersSettings = settings["Starters"].toMap();

        startersSettings["Tera Types"] = checked;
        randomizer.svRandomizerStarters.randomize_tera_types = checked;
        settings["Starters"] = startersSettings;
    });

    QCheckBox* all_starters_shiny = new QCheckBox("Force All Starters Shiny ", startersGroupSettings);
    startersRow_Q0->addWidget(all_starters_shiny);
    // Connect for Setting the values
    connect(all_starters_shiny, &QCheckBox::toggled, this, [this](bool checked) mutable{
        QMap<QString, QVariant> startersSettings = settings["Starters"].toMap();

        startersSettings["All Shiny"] = checked;
        randomizer.svRandomizerStarters.all_starters_shiny = checked;
        settings["Starters"] = startersSettings;
    });

    QCheckBox* force_shiny_starter = new QCheckBox("Force One Starter Shiny ", startersGroupSettings);
    startersRow_Q0->addWidget(force_shiny_starter);
    connect(force_shiny_starter, &QCheckBox::toggled, this, [this](bool checked) mutable{
        QMap<QString, QVariant> startersSettings = settings["Starters"].toMap();

        startersSettings["One Shiny"] = checked;
        randomizer.svRandomizerStarters.force_shiny_starter = checked;
        settings["Starters"] = startersSettings;
    });

    startersSettingsLayout->addLayout(startersRow_Q0);

    createStarterPokemonSelection("Sprigatito", randomizer.pokemonInGame, Pokeball_Names, startersGroupSettings, startersRow_Q1, startersSettingsLayout, 0);
    createStarterPokemonSelection("Fuecoco", randomizer.pokemonInGame, Pokeball_Names, startersGroupSettings, startersRow_Q2, startersSettingsLayout, 1);
    createStarterPokemonSelection("Quaxly", randomizer.pokemonInGame, Pokeball_Names, startersGroupSettings, startersRow_Q3, startersSettingsLayout, 2);

    // Pokemon Limiter
    setupAllowedPokemon(startersSettingsLayout, randomizer.svRandomizerStarters.StartersLimiter);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return starterSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createGiftsWidget(){
    QVBoxLayout *giftsSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> giftsSettings;

    // Creates the Main Button
    QCheckBox* enable_gifts = new QCheckBox("Randomize Gifts");
    giftsSettingLayout->addWidget(enable_gifts);
    // Connect for Setting the values
    connect(enable_gifts, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    // Creates Hidden Group based on button
    QGroupBox *giftsGroupSettings= new QGroupBox("Gifts Settings Section");
    QVBoxLayout *giftSettingsLayout = new QVBoxLayout(giftsGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    giftsGroupSettings->setVisible(false);
    connect(enable_gifts, &QCheckBox::toggled, giftsGroupSettings, &QGroupBox::setVisible);

    giftsSettingLayout->addWidget(giftsGroupSettings);

    // Row 0
    QCheckBox* randomize_gifts_tera_types = new QCheckBox("Randomize Tera Types ", giftsGroupSettings);
    row0->addWidget(randomize_gifts_tera_types);
    // Connect for Setting the values
    connect(randomize_gifts_tera_types, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    giftSettingsLayout->addLayout(row0);

    // Pokemon Limiter
    setupAllowedPokemon(giftSettingsLayout, randomizer.svRandomizerStarters.GiftsLimiter);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return giftsSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createAbilitiesWidget(){
    QVBoxLayout *abilitiesSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> giftsSettings;

    // Creates the Main Button
    QCheckBox* enable_abilities = new QCheckBox("Randomize Abilities");
    abilitiesSettingLayout->addWidget(enable_abilities);
    // Connect for Setting the values
    connect(enable_abilities, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    // Creates Hidden Group based on button
    QGroupBox *abilitiesGroupSettings= new QGroupBox("Abilities Settings Section");
    QVBoxLayout *abilitiesSettingsLayout = new QVBoxLayout(abilitiesGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    abilitiesGroupSettings->setVisible(false);
    connect(enable_abilities, &QCheckBox::toggled, abilitiesGroupSettings, &QGroupBox::setVisible);

    abilitiesSettingLayout->addWidget(abilitiesGroupSettings);

    // Row 0
    QCheckBox* ban_wonder_guard = new QCheckBox("Ban Wonder Guard", abilitiesGroupSettings);
    row0->addWidget(ban_wonder_guard);
    // Connect for Setting the values
    connect(ban_wonder_guard, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    QCheckBox* ban_exit_abilities = new QCheckBox("Ban Exit Abilities", abilitiesGroupSettings);
    row0->addWidget(ban_exit_abilities);
    // Connect for Setting the values
    connect(ban_exit_abilities, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    abilitiesSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return abilitiesSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createTypesWidget(){
    QVBoxLayout *typesSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> typesSettings;

    // Creates the Main Button
    QCheckBox* enable_types = new QCheckBox("Randomize Types");
    typesSettingLayout->addWidget(enable_types);
    // Connect for Setting the values
    connect(enable_types, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    // Creates Hidden Group based on button
    QGroupBox *typesGroupSettings= new QGroupBox("Types Settings Section");
    QVBoxLayout *typesSettingsLayout = new QVBoxLayout(typesGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    typesGroupSettings->setVisible(false);
    connect(enable_types, &QCheckBox::toggled, typesGroupSettings, &QGroupBox::setVisible);

    typesSettingLayout->addWidget(typesGroupSettings);

    // Row 0
    QCheckBox* extra_types = new QCheckBox("Grant Extra Types", typesGroupSettings);
    row0->addWidget(extra_types);
    // Connect for Setting the values
    connect(extra_types, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    typesSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return typesSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createMovesetWidget(){
    QVBoxLayout *movesSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> movesSettings;

    // Creates the Main Button
    QCheckBox* enable_moves = new QCheckBox("Randomize Movesets");
    movesSettingLayout->addWidget(enable_moves);
    // Connect for Setting the values
    connect(enable_moves, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    // Creates Hidden Group based on button
    QGroupBox *movesGroupSettings= new QGroupBox("Moveset Settings Section");
    QVBoxLayout *movesSettingsLayout = new QVBoxLayout(movesGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    movesGroupSettings->setVisible(false);
    connect(enable_moves, &QCheckBox::toggled, movesGroupSettings, &QGroupBox::setVisible);

    movesSettingLayout->addWidget(movesGroupSettings);

    // Row 0
    QCheckBox* same_moves_as_type = new QCheckBox("Same Moves as Type", movesGroupSettings);
    row0->addWidget(same_moves_as_type);
    // Connect for Setting the values
    connect(same_moves_as_type, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    movesSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return movesSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createBSTWidget(){
    QVBoxLayout *bstSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> bstSettings;

    // Creates the Main Button
    QCheckBox* enable_bst = new QCheckBox("Randomize BST");
    bstSettingLayout->addWidget(enable_bst);
    // Connect for Setting the values
    connect(enable_bst, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    // Creates Hidden Group based on button
    QGroupBox *bstGroupSettings= new QGroupBox("BST Settings Section");
    QVBoxLayout *bstSettingsLayout = new QVBoxLayout(bstGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    bstGroupSettings->setVisible(false);
    connect(enable_bst, &QCheckBox::toggled, bstGroupSettings, &QGroupBox::setVisible);

    bstSettingLayout->addWidget(bstGroupSettings);

    // Row 0
    QCheckBox* same_bst = new QCheckBox("Keep Same BST", bstGroupSettings);
    row0->addWidget(same_bst);
    // Connect for Setting the values
    connect(same_bst, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    bstSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return bstSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createEvolutionsWidget(){
    QVBoxLayout *evolutionsSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> evolutionsSettings;

    // Creates the Main Button
    QCheckBox* enable_evos = new QCheckBox("Randomize Evolutions");
    evolutionsSettingLayout->addWidget(enable_evos);
    // Connect for Setting the values
    connect(enable_evos, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    // Creates Hidden Group based on button
    QGroupBox *evolutionsGroupSettings= new QGroupBox("Evolutions Settings Section");
    QVBoxLayout *evolutionsSettingsLayout = new QVBoxLayout(evolutionsGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    evolutionsGroupSettings->setVisible(false);
    connect(enable_evos, &QCheckBox::toggled, evolutionsGroupSettings, &QGroupBox::setVisible);

    evolutionsSettingLayout->addWidget(evolutionsGroupSettings);

    // Row 0
    QCheckBox* evolve_every_level = new QCheckBox("Evolve Every Level", evolutionsGroupSettings);
    row0->addWidget(evolve_every_level);
    // Connect for Setting the values
    connect(evolve_every_level, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    QCheckBox* fix_evolutions = new QCheckBox("Fix Evolutions (Regional & Trade)", evolutionsGroupSettings);
    row0->addWidget(fix_evolutions);
    // Connect for Setting the values
    connect(fix_evolutions, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    evolutionsSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return evolutionsSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createTMWidget(){
    QVBoxLayout *tmSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> tmSettings;

    // Creates the Main Button
    QCheckBox* enable_tms = new QCheckBox("Randomize TMs");
    tmSettingLayout->addWidget(enable_tms);
    // Connect for Setting the values
    connect(enable_tms, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    // Creates Hidden Group based on button
    QGroupBox *tmGroupSettings= new QGroupBox("TMs Settings Section");
    QVBoxLayout *tmSettingsLayout = new QVBoxLayout(tmGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    tmGroupSettings->setVisible(false);
    connect(enable_tms, &QCheckBox::toggled, tmGroupSettings, &QGroupBox::setVisible);

    tmSettingLayout->addWidget(tmGroupSettings);

    // Row 0
    QCheckBox* moves_without_animation = new QCheckBox("Allow Moves without Animation", tmGroupSettings);
    row0->addWidget(moves_without_animation);
    // Connect for Setting the values
    connect(moves_without_animation, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    tmSettingsLayout->addLayout(row0);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return tmSettingLayout;
}

QVBoxLayout* SVRandomizerWindow::createItemWidget(){
    QVBoxLayout *itemSettingLayout = new QVBoxLayout();

    QMap<QString, QVariant> itemSettings;

    // Creates the Main Button
    QCheckBox* enable_items = new QCheckBox("Randomize Items");
    itemSettingLayout->addWidget(enable_items);
    // Connect for Setting the values
    connect(enable_items, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    // Creates Hidden Group based on button
    QGroupBox *itemGroupSettings= new QGroupBox("Items Settings Section");
    QVBoxLayout *itemSettingsLayout = new QVBoxLayout(itemGroupSettings);
    QHBoxLayout *row0 = new QHBoxLayout();
    QHBoxLayout *row1 = new QHBoxLayout();

    // Connect Randomize Starters to visibility
    itemGroupSettings->setVisible(false);
    connect(enable_items, &QCheckBox::toggled, itemGroupSettings, &QGroupBox::setVisible);

    itemSettingLayout->addWidget(itemGroupSettings);

    // Row 0
    QCheckBox* hidden_items = new QCheckBox("Randomize Hidden Items", itemGroupSettings);
    row0->addWidget(hidden_items);
    // Connect for Setting the values
    connect(hidden_items, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    QCheckBox* pickup_items = new QCheckBox("Randomize Pickup Items", itemGroupSettings);
    row0->addWidget(pickup_items);
    // Connect for Setting the values
    connect(pickup_items, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    itemSettingsLayout->addLayout(row0);

    // Row 1
    QCheckBox* drops = new QCheckBox("Randomize Pokemon Drops", itemGroupSettings);
    row1->addWidget(drops);
    // Connect for Setting the values
    connect(drops, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    QCheckBox* synchro = new QCheckBox("Randomize Synchro Items", itemGroupSettings);
    row1->addWidget(synchro);
    // Connect for Setting the values
    connect(synchro, &QCheckBox::toggled, this, [this](bool checked) mutable{

    });

    itemSettingsLayout->addLayout(row1);

    // Connection for importing settings
    connect(this, &SVRandomizerWindow::importSettings, this, [this]() mutable{
        qDebug()<<"Here";
    });

    return itemSettingLayout;
}

// Helpers
void SVRandomizerWindow::switchTabs(QStackedWidget * stackedWidget, int index) {
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
    } else {
        qWarning() << "Invalid tab index:" << index;
    }
}

// Intializers
void SVRandomizerWindow::initializeSettings(){

    // Allowed Settings (Used for all)
        QMap<QString, QVariant> allowedPokemonSettings;
        QMap<QString, QVariant> generationsSettings;
        QMap<QString, QVariant> legendarySettings;
        for(int i=1; i<=maxGeneration; i++){
            generationsSettings[QStringLiteral("%1").arg(i)] = true;
            legendarySettings[QStringLiteral("%1").arg(i)] = true;
        }
        allowedPokemonSettings["Generations"] = QVariant::fromValue(generationsSettings);
        allowedPokemonSettings["Legends"] = QVariant::fromValue(legendarySettings);
        allowedPokemonSettings["Stage 1"] = true;
        allowedPokemonSettings["Stage 2"] = true;
        allowedPokemonSettings["Stage 3"] = true;
        allowedPokemonSettings["Single Stage"] = true;
        allowedPokemonSettings["Paradox"] = true;

    // Starter Settings
        QMap<QString, QVariant> startersSettings;
        startersSettings["Randomize"] = false;
        startersSettings["Tera Types"] = false;
        startersSettings["All Shiny"] = false;
        startersSettings["One Shiny"] = false;
        for(int i =0; i<starterNames.size(); i++){
            QMap<QString, QVariant> starterSpecificSettings;
            starterSpecificSettings["Shiny"] = false;
            starterSpecificSettings["Name"] = "";
            starterSpecificSettings["Form"] = 0;
            starterSpecificSettings["Gender"] = "MALE";
            starterSpecificSettings["Pokeball"] = "Poke Ball";
            startersSettings[starterNames[i]] = QVariant::fromValue(starterSpecificSettings);
        }
        startersSettings["Allowed Pokemon"] = QVariant::fromValue(allowedPokemonSettings);
        settings["Starters"] = QVariant::fromValue(startersSettings);
}

// Example for big button (kept for future things)
// QGroupBox *startersGroupSettings= new QGroupBox("Starter Settings Section");
// QPushButton *toggleStartersGroupSettings= new QPushButton("Toggle Starter Settings Section", startersWidget);

// // StyleSheet is a format changer. Lets you change the way something looks - right now only used on buttons but can be used
// // for everything else.
// toggleStartersGroupSettings->setStyleSheet(
//     "QPushButton {"
//     "   background-color: #3498db;"
//     "   color: white;"
//     "   border: none;"
//     "   padding: 8px 16px;"
//     "   border-radius: 10px;"
//     "}"
//     "QPushButton:hover {"
//     "   background-color: #2980b9;"
//     "}"
//     "QPushButton:pressed {"
//     "   background-color: #1c6692;"
//     "}"
// );

// // Connect to Hide the section
// connect(toggleStartersGroupSettings, &QPushButton::clicked, startersGroupSettings, [startersGroupSettings]() {
//     startersGroupSettings->setVisible(!startersGroupSettings->isVisible());
// });
// formLayout->addRow(toggleStartersGroupSettings);
