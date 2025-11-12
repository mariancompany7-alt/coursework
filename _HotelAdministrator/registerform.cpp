#include "registerform.h"
#include "ui_registerform.h"
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
    // QString name = ui->leName->text();
    // QString email = ui->leEmail->text();
    // QString password = ui->lePassword->text();

    // if(name.isEmpty() || email.isEmpty() || password.isEmpty()) {

    //     QMessageBox::critical(this, "Error", "Fields cannot be emplty!");
    //     return;

    // }

    // query->prepare("INSERT INTO users (name, password) VALUES (:name, :password)";
    // query->bindVa
}
