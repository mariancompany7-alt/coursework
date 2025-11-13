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
    QString name = ui->leName->text();
    QString email = ui->leEmail->text();
    QString password = ui->lePassword->text();

    if(name.isEmpty() || email.isEmpty() || password.isEmpty()) {

        QMessageBox::critical(this, "Error", "Fields cannot be empty!");
        return;
    }

    DatabaseManager dbManager;

    if(!dbManager.connect()) {
        QMessageBox::critical(this, "Error", "Failed to connect to the database!");
        return;
    }

    if(dbManager.insert(name, email, password)) {
        QMessageBox::information(this, "Success!", "You have successfully registered!");
        this->accept();
    } else {
        QMessageBox::critical(this, "Error", "Failed to register!");
    }

}
