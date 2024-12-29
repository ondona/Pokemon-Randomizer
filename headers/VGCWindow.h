#ifndef VGCWINDOW_H
#define VGCWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class VGCWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VGCWindow(QWidget *parent = nullptr);
    ~VGCWindow();

private:
    void createLayout();
    QPushButton *favoriteButton;  // Button to mark as favorite
    QVBoxLayout *mainLayout;

private slots:
    void addToFavorites();  // Slot to add this window to favorites
};

#endif // VGCWINDOW_H
