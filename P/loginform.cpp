#include "loginform.h"
#include "ui_loginform.h"
#include "userrepository.h"
#include "databasemanager.h"
#include "user.h"
#include <QMessageBox>
#include <QDebug>
#include <QIcon>
#include <QString>

LoginForm::LoginForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginForm) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/login24.png"));

    this->setStyleSheet(QStringLiteral(
        "QDialog { background-color: #f7f7f7; }"
        "QLineEdit { border: 1px solid #c0c0c0; border-radius: 4px; padding: 2px; background-color: white; min-height: 20px; }"
        "QLineEdit:focus { border: 1px solid #3498db; }"
        "QPushButton { background-color: #3498db; color: white; border: none; border-radius: 5px; padding: 8px 16px; font-size: 20px; min-height: 25px; }"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:pressed { background-color: #1f618d; }"
        ));


    userRepo = new UserRepository(DatabaseManager::getInstance());

#ifdef QT_DEBUG
    ui->leEmail->setText("admin");
    ui->lePassword->setText("admin123");
#endif
}

void LoginForm::on_pbLogIn_clicked() {

    QString login = ui->leEmail->text();
    QString password = ui->lePassword->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Заповніть всі поля!");
        return;
    }

    if (login.length() < 5) {
        QMessageBox::warning(this, "Помилка", "Логін повинен бути не менше 5 символів!");
        return;
    }

    if (password.length() < 1) {
        QMessageBox::warning(this, "Помилка", "Пароль повинен бути не менше 1 символа!");
        return;
    }

    User* user   = userRepo->findByLogin(login);

    if (user && user->verifyPassword(password)) {
        userRole = user->getRole();
        m_userId = user->getId();
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

