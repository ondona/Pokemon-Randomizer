#include <QApplication>
#include <QScreen>
#include <QMutex>
#include <QSettings>
#include "headers/qtwindows_headers/mainwindow.h"

// Global file, mutex, and settings
QFile logFile;
QMutex logMutex;
bool enableDebugInRelease = false; // This will be read from QSettings

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QMutexLocker locker(&logMutex); // Ensure thread safety

    QString logMessage = QString("[%1] %2")
                             .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                             .arg(msg);

    #ifdef QT_DEBUG
        // In debug mode, also log to the terminal
        switch (type) {
        case QtDebugMsg:
            fprintf(stdout, "Debug: %s\n", logMessage.toLocal8Bit().constData());
            break;
        case QtWarningMsg:
            fprintf(stdout, "Warning: %s\n", logMessage.toLocal8Bit().constData());
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s\n", logMessage.toLocal8Bit().constData());
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s\n", logMessage.toLocal8Bit().constData());
            logFile.close();
            abort();
        }
    #endif

    // Always write to file
    if (logFile.isOpen()) {
        QTextStream stream(&logFile);
        stream << logMessage << Qt::endl;
    }
}

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

    // Creates log file
    QString logFilePath = "logs/logs.log";
    logFile.setFileName(logFilePath);
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qCritical() << "Failed to open log file!";
    }

    // Creates Buffer of qDebugs/qCriticals/qFatals into logs folder for debug and release.
    qInstallMessageHandler(customMessageHandler);

    // Creates settings for Randomizer Options (things to be remember between launches)
    QSettings settings("Pokemon Randomizer", "Main");

    // Default to true for "Always on Top"
    bool alwaysOnTopValue = settings.value("AlwaysOnTop", true).toBool();
    mainWindow.alwaysOnTop(alwaysOnTopValue);

    return app.exec();
}
