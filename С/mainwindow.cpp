#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginform.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/mainWindow.png"));
    setWindowTitle("Система управління готелем");

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
