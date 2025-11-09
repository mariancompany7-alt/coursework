#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString email = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    if (email == "marian" && password == "123") {
        SecondWindow object;
        object.setModal(true);
        this->hide();
        object.exec();
    } else {
        QMessageBox::critical(this, "Error", "Incorrect login or password");
    }
}

