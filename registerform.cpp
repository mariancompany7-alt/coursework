#include "registerform.h"
#include "ui_registerform.h"
#include "databasemanager.h"
#include <QMessageBox>

RegisterForm::RegisterForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterForm)
{
    ui->setupUi(this);
}

RegisterForm::~RegisterForm()
{
    delete ui;
}

void RegisterForm::on_pbRegister_clicked()
{
    QString name = ui->leName->text().trimmed();
    QString email = ui->leEmail->text().trimmed();
    QString password = ui->lePassword->text();

    if(name.isEmpty() || email.isEmpty() || password.isEmpty()) {
        QMessageBox::critical(this, "Error", "Fields cannot be empty!");
        return;
    }

    DatabaseManager dbManager;

    // Крок 1: Підключення
    if(!dbManager.connect()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to connect to the database!\n" +
                                  dbManager.getLastError());
        return;
    }

    // ⬇️ Крок 2: Створення таблиць
    if(!dbManager.createTables()) {
        QMessageBox::critical(this, "Database Error",
                              "Failed to create tables!\n" +
                                  dbManager.getLastError());
        return;
    }

    // Крок 3: Вставка даних
    if(dbManager.insert(name, email, password)) {
        QMessageBox::information(this, "Success!",
                                 "You have successfully registered!");
        this->accept();

    } else {
        QMessageBox::critical(this, "Error",
                              "Failed to register!\n" +
                                  dbManager.getLastError());
    }
}
