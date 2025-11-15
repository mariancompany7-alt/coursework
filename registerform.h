#ifndef REGISTERFORM_H
#define REGISTERFORM_H

#include <QDialog>

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = nullptr);
    ~RegisterForm();

private slots:
    void on_pbRegister_clicked();

private:
    Ui::RegisterForm *ui;
};

#endif // REGISTERFORM_H
