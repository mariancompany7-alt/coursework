#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "secondwindow.h"
#include "registerform.h"

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

void MainWindow::on_pbSignIn_clicked()
{
    SecondWindow window;
    window.setModal(true);
    this->hide();
    window.exec();
}


void MainWindow::on_pbSignUp_clicked()
{
    RegisterForm window;
    window.setModal(true);
    this->hide();
    window.exec();
}

