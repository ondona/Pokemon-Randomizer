#include "headers/mainwindow.h"
// #include "GameEditWindow.h"
// #include "RandomizerWindow.h"
// #include "VGCWindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QDockWidget>
#include <QFile>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QScrollArea>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <filesystem>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QVersionNumber>

namespace fs = std::filesystem;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    resize(950, 800);

    // Central Widget
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    // Main layout for the central widget
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Topbar (Tabbar for navigation)
    topBar = new QTabBar(this);
    topBar->addTab("Home");
    // topBar->addTab("Randomizer");
    // topBar->addTab("VGC");
    // topBar->addTab("Game Editing");

    connect(topBar, &QTabBar::currentChanged, this, &MainWindow::switchTab);
    mainLayout->addWidget(topBar);  // Add the tab bar to the top of the layout

    // QStackedWidget for switching between different views
    stackedWidget = new QStackedWidget(this);
    mainLayout->addWidget(stackedWidget);  // Add the stacked widget to the main layout
    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->setStretch(1, 1);

    // Create Home view (scrollable area for the main content)
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollAreaWidget = new QWidget(scrollArea);
    gridLayout = new QGridLayout(scrollAreaWidget);
    scrollAreaWidget->setLayout(gridLayout);
    scrollArea->setWidget(scrollAreaWidget);

    stackedWidget->addWidget(scrollArea);  // Add the scroll area as the "Home" view

    // Create unique windows and add them to the stacked widget
    // RandomizerWindow* randomizerWindow = new RandomizerWindow();
    // stackedWidget->addWidget(randomizerWindow);

    // VGCWindow* vgcWindow = new VGCWindow();
    // stackedWidget->addWidget(vgcWindow);

    // GameEditWindow* gameEditWindow = new GameEditWindow();
    // stackedWidget->addWidget(gameEditWindow);

    // Toolbar
    QToolBar *toolbar = addToolBar("Main Tool Bar");
    toolbar->setMovable(false);
    QAction *updateAction = new QAction("Check for Updates", this);
    connect(updateAction, &QAction::triggered, this, &MainWindow::checkForUpdates);
    toolbar->addAction(updateAction);

    // Sidebar
    QDockWidget *dockSidebar = new QDockWidget("Sidebar", this);
    dockSidebar->setFeatures(QDockWidget::NoDockWidgetFeatures);
    sidebar = new QListWidget(this);
    sidebar->setFixedWidth(150);
    connect(sidebar, &QListWidget::itemClicked, this, &MainWindow::updateFavorites);
    dockSidebar->setWidget(sidebar);
    addDockWidget(Qt::LeftDockWidgetArea, dockSidebar);

    // Sample images in grid for demonstration
    QString imageFolderPath = QString::fromStdString(fs::absolute("images").string());  // Path to the local images folder

    for (int i = 0; i < 1; ++i) {
        QLabel *imageLabel = new QLabel(this);

        // Build the image path dynamically
        QString filePath = imageFolderPath + QString("/image%1.jpeg").arg(i + 1);
        QImage image(filePath);
        if (image.isNull()) {
            qDebug() << "Failed to load image:" << filePath;
        }

        // Check if the image file exists
        if (QFile::exists(filePath)) {
            QPixmap pixmap = QPixmap::fromImage(image);
            if (pixmap.isNull()) {
                qDebug() << "Pixmap is null";
            }
            pixmap = pixmap.scaled(250, 350, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            imageLabel->setPixmap(pixmap);
            imageLabel->setAlignment(Qt::AlignCenter);  // Align image to center
            //imageLabel->setFixedSize(pixmap.size());  // Set label size to match image size
        } else {
            qWarning() << "Image file not found:" << filePath;
        }

        // Add the label to the grid layout
        gridLayout->addWidget(imageLabel, (i / 3) + 1, i % 3);

        // Make the label clickable by installing an event filter
        imageLabel->setCursor(Qt::PointingHandCursor);  // Change cursor to indicate clickable area

        // Store the label and the index in a way to use the index later
        imageLabel->setProperty("index", i);  // Store the index as a property

        // Install an event filter to capture mouse clicks
        imageLabel->installEventFilter(this);  // 'this' refers to MainWindow, where we will handle the event

        // Create Alternate Windows
        AlternateWindow *window = new AlternateWindow(i);
        alternateWindows.append(window);

    }
    // Auto-Updater Setup
    //setupAutoUpdater();

    // Enable tab closing for all tabs
    topBar->setTabsClosable(true);
    connect(topBar, &QTabBar::tabCloseRequested, this, &MainWindow::closeTab);

    checkForUpdates();
}

// Handle mouse events in your MainWindow or parent class
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel *label = qobject_cast<QLabel *>(watched);
        if (label) {
            if (event->type() == QEvent::MouseButtonPress) {
                if (static_cast<QMouseEvent *>(event)->button() == Qt::LeftButton) {
                    // Get the index from the label's property
                    int index = label->property("index").toInt();
                    openAlternateWindow(index);  // Call your function with the index
                    return true;  // Event was handled
                }
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);  // Pass the event on to the parent class
}
// Add a map to store references to the widgets and tabs
QMap<int, AlternateWindow*> openWindows;

void MainWindow::openAlternateWindow(int index) {
    QString tabName;
    switch(index){
    case 0:
        tabName = "Pokemon SV Randomizer";
        break;
    case 1:
        tabName = "Pokemon Legends Randomizer";
        break;
    default:
        tabName = "Image";
    }

    // Check if the tab already exists
    for (int i = 0; i < topBar->count(); ++i) {
        if (topBar->tabText(i) == tabName) {
            // If the tab exists, simply switch to it
            topBar->setCurrentIndex(i);
            stackedWidget->setCurrentIndex(i);  // Adjust index for QStackedWidget
            return;
        }
    }

    // If no tab exists, create a new one
    AlternateWindow *alternateWindow = alternateWindows[index];  // Get the alternate window for the clicked image
    int newTabIndex = topBar->addTab(tabName);  // Add a new tab to the QTabBar
    stackedWidget->addWidget(alternateWindow);  // Add the new window to the QStackedWidget

    // Store the reference to the widget in the map
    openWindows[newTabIndex] = alternateWindow;

    // Show the newly created tab
    topBar->setCurrentIndex(newTabIndex);
    stackedWidget->setCurrentIndex(newTabIndex);  // Adjust index for QStackedWidget
}

void MainWindow::closeTab(int index) {
    // Ensure that the first tab is not closed
    if (index < 0 || index >= topBar->count() || index == 0) {
        return;  // Ignore closing the first tab
    }

    // Get the widget associated with the tab
    QWidget *tabWidget = stackedWidget->widget(index);

    // Remove the tab from the QTabBar and the widget from the QStackedWidget
    topBar->removeTab(index);
    stackedWidget->removeWidget(tabWidget);

    // Remove the widget from the map (do not delete it as we might reuse it later)
    openWindows.remove(index);
}


void MainWindow::updateFavorites() {
    // Code to manage favorites in sidebar (e.g., toggling favorite status)
}

void MainWindow::checkForUpdates() {
    QString currentVersion = "0.1.1";

    // Replace with actual owner and repo
    QString owner = "Gonzalooo";
    QString repo = "Pokemon-Randomizer";

    QString apiUrl = QString("https://api.github.com/repos/%1/%2/releases/latest").arg(owner).arg(repo);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply) {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::critical(this, "Error", "Failed to check for updates: " + reply->errorString());
            reply->deleteLater();
            return;
        }

        // Parse JSON response
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonResponse.object();
        QString latestVersion = jsonObject.value("tag_name").toString();

        if (latestVersion.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to parse the latest version from the server.");
            reply->deleteLater();
            return;
        }

        // Compare versions
        QVersionNumber current = QVersionNumber::fromString(currentVersion);
        QVersionNumber latest = QVersionNumber::fromString(latestVersion.startsWith('v') ? latestVersion.mid(1) : latestVersion);

        if (latest == current) {
            QMessageBox::information(this, "Up-to-date", "You are already using the latest version (" + currentVersion + ").");
        } else if(latest > current) {
            QMessageBox::information(this, "Update Available", "A new version (" + latestVersion + ") is available!");
        }else{
            QMessageBox::information(this, "Early Version in Use", "You are using version: " + currentVersion + '\n' + "Latest version released is: " + latestVersion);
        }

        reply->deleteLater();
    });

    // Send GET request
    QNetworkRequest request(apiUrl);
    manager->get(request);
}

void MainWindow::setupAutoUpdater() {
    // This function will be extended to handle the auto-update mechanism.
    QTimer::singleShot(5000, this, &MainWindow::checkForUpdates);  // Example to trigger an update check every 5s
}

void MainWindow::switchTab(int index) {
    if (index >= 0 && index < stackedWidget->count()) {
        stackedWidget->setCurrentIndex(index);
    } else {
        qWarning() << "Invalid tab index:" << index;
    }
}
