#include "headers/GameEditWindow.h"

GameEditWindow::GameEditWindow(QWidget *parent)
    : QWidget(parent)
{
    createLayout();  // This function creates the UI layout and adds widgets
}

GameEditWindow::~GameEditWindow()
{
    // Cleanup if needed
}

void GameEditWindow::createLayout()
{
    // Create the layout and widgets for this window
    mainLayout = new QVBoxLayout(this);

    favoriteButton = new QPushButton("Add to Favorites", this);
    connect(favoriteButton, &QPushButton::clicked, this, &GameEditWindow::addToFavorites);

    mainLayout->addWidget(favoriteButton);
}

void GameEditWindow::addToFavorites()
{
    // Logic for adding the window to the favorites
    // You can add it to a sidebar or perform any other necessary actions
    // For example, emit a signal or interact with the MainWindow
}
