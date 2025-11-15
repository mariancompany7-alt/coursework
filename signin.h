#ifndef SIGNIN_H
#define SIGNIN_H

#include <QDialog>

namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();

private slots:
    void on_pbLogIn_clicked();

private:
    Ui::SecondWindow *ui;
};

#endif // SIGNIN_H
