#include <QApplication>
#include <QScreen>
#include "headers/mainwindow.h"



int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    #ifdef QT_DEBUG
        QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath() + "/../plugins");  // Development path
    #else
        QCoreApplication::addLibraryPath("./plugins");  // Deployment path
    #endif
    MainWindow mainWindow;

    // Get the screen where the mouse is currently located
    QScreen *screen = nullptr;
    QList<QScreen *> screens = QApplication::screens();
    QCursor cursor;

    // Get the current screen based on mouse cursor position
    QPoint cursorPos = cursor.pos();

    for (QScreen *s : screens) {
        if (s->geometry().contains(cursorPos)) {
            screen = s;
            break;
        }
    }

    if (!screen) {
        screen = QApplication::primaryScreen();
    }

    QRect screenGeometry = screen->availableGeometry();

    // Calculate the center point
    int x = (screenGeometry.width() - mainWindow.width()) / 2 + screenGeometry.x();
    int y = (screenGeometry.height() - mainWindow.height()) / 2 + screenGeometry.y();

    // Move the window to the center
    mainWindow.move(x, y);

    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));


    mainWindow.setWindowFlag(Qt::WindowStaysOnTopHint, true);
    mainWindow.show();
    mainWindow.raise();
    mainWindow.activateWindow();
    mainWindow.setWindowFlag(Qt::WindowStaysOnTopHint, false);
    mainWindow.show();
    return app.exec();
}
