#include "headers/qtwindows_headers/AlternateWindow.h"
#include "headers/sv_randomizer_headers/SVRandomizerWindow.h"
#include <QApplication>
#include <QScreen>
#include <QStackedWidget>
#include <QScrollArea>

AlternateWindow::AlternateWindow(int id, QWidget *parent) : QWidget(parent), windowId(id) {
    QVBoxLayout *layout = new QVBoxLayout(this); // Main layout for AlternateWindow
    stackedWidget = new QStackedWidget(this);
    stackedWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    switch(id){
        case 0:
            svrandomizer = new SVRandomizerWindow();
            svrandomizer->createLayout();
            stackedWidget->addWidget(svrandomizer);
            break;
        case 1:
            break;
        default:
            label = new QLabel(QString("This is alternate window %1").arg(id + 1), this);
            layout->addWidget(label);
    }

        layout->addWidget(stackedWidget); // Ensure stackedWidget is added to the layout
        setLayout(layout); // Set the layout for AlternateWindow
}
