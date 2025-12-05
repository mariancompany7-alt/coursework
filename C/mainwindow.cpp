#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginform.h"
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Система управління готелем");

    // 1. Створюємо всі панелі
    setupPanels();

    // 2. Запускаємо процес входу одразу після завантаження вікна
    // Використовуємо QTimer::singleShot, щоб конструктор встиг завершитися
    QTimer::singleShot(0, this, &MainWindow::startLoginProcess);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPanels() {
    // --- Admin Panel ---
    adminPanel = new AdminPanel(this);
    adminPanelIndex = ui->stackedWidget->addWidget(adminPanel);

    connect(adminPanel, &AdminPanel::logoutRequested, this, [this]() {
        this->hide();
        startLoginProcess();
    });

    // --- Manager Panel ---
    managerPanel = new ManagerPanel(this);
    managerPanelIndex = ui->stackedWidget->addWidget(managerPanel);

    connect(managerPanel, &ManagerPanel::logoutRequested, this, [this]() {
        this->hide();
        startLoginProcess();
    });

    // --- Guest Panel ---
    guestPanel = new GuestPanel(this);
    guestPanelIndex = ui->stackedWidget->addWidget(guestPanel);

    connect(guestPanel, &GuestPanel::logoutRequested, this, [this]() {
        this->hide();
        startLoginProcess();
    });
}

void MainWindow::startLoginProcess()
{
    // Створюємо модальне вікно входу
    LoginForm loginWindow(this);

    // Запускаємо і чекаємо результату
    if (loginWindow.exec() == QDialog::Accepted) {

        // Отримуємо дані з форми (переконайтеся, що методи є в LoginForm)
        QString role = loginWindow.getRole();
        int userId = loginWindow.getUserId(); // <--- ВАЖЛИВО!

        // Обробляємо успішний вхід
        handleLoginSuccessful(role, userId);

        // Показуємо головне вікно
        this->show();
    } else {
        // Якщо користувач натиснув "Скасувати" або хрестик - закриваємо програму
        this->close();
    }
}

void MainWindow::handleLoginSuccessful(QString role, int userId) {

    if (role == "admin") {
        ui->stackedWidget->setCurrentIndex(adminPanelIndex);
        setWindowTitle("Панель Адміністратора");
    }
    else if (role == "manager") {
        ui->stackedWidget->setCurrentIndex(managerPanelIndex);
        setWindowTitle("Панель Менеджера");
    }
    else if (role == "guest") {
        ui->stackedWidget->setCurrentIndex(guestPanelIndex);
        setWindowTitle("Кабінет Гостя");

        //  КРИТИЧНО: Передаємо ID користувача в панель гостя
        // Без цього кнопки "Забронювати" не працюватимуть!
        if (guestPanel) {
            guestPanel->setCurrentUser(userId);
        }
    }
    else {
        QMessageBox::critical(this, "Помилка", "Невідома роль користувача!");
        this->close();
    }
}
