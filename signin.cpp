#include "signin.h"
#include "ui_signin.h"
#include "databasemanager.h"
#include <QMessageBox>

SecondWindow::SecondWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SecondWindow) {
    ui->setupUi(this);
}

void SecondWindow::on_pbLogIn_clicked()
{
    QSngtri email = ui->leEmail->text().trimmed();
    QString password = ui->lePassword->text();

    if(email.isEmpty() || password.isEmpty()) {
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

    if(dbManager.userEnter(email, password)) {
        QMessageBox::information(this, "Success", "You authorised successfully!");
        this->accept();
    } else {
        QMessageBox::critical(this, "Error", "Invalid email or password");
    }

}

SecondWindow::~SecondWindow() {
    delete ui;
}

