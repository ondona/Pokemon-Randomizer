#ifndef GAMEEDITWINDOW_H
#define GAMEEDITWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class GameEditWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameEditWindow(QWidget *parent = nullptr);
    ~GameEditWindow();

private:
    void createLayout();
    QPushButton *favoriteButton;  // Button to mark as favorite
    QVBoxLayout *mainLayout;

private slots:
    void addToFavorites();  // Slot to add this window to favorites
};

#endif // GAMEEDITWINDOW_H
