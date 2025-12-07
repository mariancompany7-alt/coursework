#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "adminpanel.h"
#include "managerpanel.h"
#include "guestpanel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

void startLoginProcess();

private:

    AdminPanel* adminPanel;
    ManagerPanel* managerPanel;
    GuestPanel* guestPanel;

    int adminPanelIndex = 0;
    int managerPanelIndex = 1;
    int guestPanelIndex = 2;

    void setupPanels();
    void handleLoginSuccessful(QString role, int userId);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
