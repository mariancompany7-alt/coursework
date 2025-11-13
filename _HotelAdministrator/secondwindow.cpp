#include "secondwindow.h"
#include "ui_secondwindow.h"
#include <QMessageBox>

SecondWindow::SecondWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SecondWindow)
{
    ui->setupUi(this);
}

SecondWindow::~SecondWindow()
{
    delete ui;
}

void SecondWindow::on_pbLogIn_clicked()
{
    // QString email = ui->leEmail->text();
    // QString password = ui->lePassword->text();

    // if (email == "marian" && password == "123") {

    // } else {
    //     QMessageBox::critical(this, "Error", "Incorrect login or password");
    // }
}

