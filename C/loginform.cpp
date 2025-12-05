#include "loginform.h"
#include "ui_loginform.h"
#include "userrepository.h"
#include "databasemanager.h"
#include "user.h"
#include <QMessageBox>
#include <QDebug>

LoginForm::LoginForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginForm) {
    ui->setupUi(this);

    userRepo = new UserRepository(DatabaseManager::getInstance());

#ifdef QT_DEBUG
    ui->leEmail->setText("admin@hotel.com");
    ui->lePassword->setText("admin123");
#endif
}

void LoginForm::on_pbLogIn_clicked() {

    QString login = ui->leEmail->text();
    QString password = ui->lePassword->text();

    // 1. Валідація введених даних
    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Заповніть всі поля!");
        return;
    }

    User* user   = userRepo->findByLogin(login);

    if (user && user->verifyPassword(password)) {
        // Вхід успішний
        userRole = user->getRole();
        m_userId = user->getId();
        // 2. Закриваємо діалог входу
        accept();
    }
     else {
        QMessageBox::warning(this, "Помилка", "Невірний логін або пароль!");
    }
}

LoginForm::~LoginForm() {
    delete ui;
    delete userRepo;
}

