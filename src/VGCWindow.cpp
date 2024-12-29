#include "headers/VGCWindow.h"

VGCWindow::VGCWindow(QWidget *parent)
    : QWidget(parent)
{
    createLayout();  // This function creates the UI layout and adds widgets
}

VGCWindow::~VGCWindow()
{
    // Cleanup if needed
}

void VGCWindow::createLayout()
{
    // Create the layout and widgets for this window
    mainLayout = new QVBoxLayout(this);

    favoriteButton = new QPushButton("Add to Favorites", this);
    connect(favoriteButton, &QPushButton::clicked, this, &VGCWindow::addToFavorites);

    mainLayout->addWidget(favoriteButton);
}

void VGCWindow::addToFavorites()
{
    // Logic for adding the window to the favorites
    // You can add it to a sidebar or perform any other necessary actions
    // For example, emit a signal or interact with the MainWindow
}
