#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginform.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QIcon>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/adminPanel_0.png"));
    setWindowTitle("Система управління готелем");

    this->setStyleSheet(QStringLiteral(
        "QDialog { background-color: #f7f7f7; }"
        "QWidget#AdminPanel, QWidget#ManagerPanel, QWidget#GuestPanel { background-color: #ffffff; }"
        "QGroupBox { border: 1px solid #dcdcdc; border-radius: 6px; margin-top: 10px; padding: 10px; background-color: #f7f7f7; }"
        "QLineEdit { border: 1px solid #c0c0c0; border-radius: 4px; padding: 6px; background-color: white; min-height: 20px; }"
        "QLineEdit:focus { border: 1px solid #3498db; }"
        "QTableView { border: 1px solid #dcdcdc; gridline-color: #eeeeee; background-color: white; selection-background-color: #ddeeff; selection-color: black; border-radius: 6px; }"
        "QHeaderView::section { background-color: #efefef; color: #333333; padding: 6px; border: 1px solid #dcdcdc; font-weight: bold; }"
        "QComboBox { border: 1px solid #b0b0b0; border-radius: 3px; padding: 1px 18px 1px 3px; min-width: 6em; }"
        ));

    setupPanels();

    QTimer::singleShot(0, this, &MainWindow::startLoginProcess);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPanels() {

    adminPanel = new AdminPanel(this);
    adminPanelIndex = ui->stackedWidget->addWidget(adminPanel);

    connect(adminPanel, &AdminPanel::logoutRequested, this, [this]() {
        this->hide();
        startLoginProcess();
    });

    managerPanel = new ManagerPanel(this);
    managerPanelIndex = ui->stackedWidget->addWidget(managerPanel);

    connect(managerPanel, &ManagerPanel::logoutRequested, this, [this]() {
        this->hide();
        startLoginProcess();
    });

    guestPanel = new GuestPanel(this);
    guestPanelIndex = ui->stackedWidget->addWidget(guestPanel);

    connect(guestPanel, &GuestPanel::logoutRequested, this, [this]() {
        this->hide();
        startLoginProcess();
    });
}

void MainWindow::startLoginProcess()
{
    LoginForm loginWindow(this);

    if (loginWindow.exec() == QDialog::Accepted) {

        QString role = loginWindow.getRole();
        int userId = loginWindow.getUserId();

        handleLoginSuccessful(role, userId);

        this->show();
    } else {
        this->close();
    }
}

void MainWindow::handleLoginSuccessful(QString role, int userId) {

    if (role == "admin") {
        ui->stackedWidget->setCurrentIndex(adminPanelIndex);
        setWindowTitle("Панель адміністратора");
    }
    else if (role == "manager") {
        ui->stackedWidget->setCurrentIndex(managerPanelIndex);
        setWindowTitle("Панель менеджера");
    }
    else if (role == "guest") {
        ui->stackedWidget->setCurrentIndex(guestPanelIndex);
        setWindowTitle("Кабінет гостя");

        if (guestPanel) {
            guestPanel->setCurrentUser(userId);
        }
    }
    else {
        QMessageBox::critical(this, "Помилка", "Невідома роль користувача!");
        this->close();
    }
}
