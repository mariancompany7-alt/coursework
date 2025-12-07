#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QDialog>
#include "userrepository.h"

namespace Ui {
class LoginForm;
}

class LoginForm : public QDialog
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();
    QString getRole() const { return userRole; }
    int getUserId() const { return m_userId; }

private slots:
    void on_pbLogIn_clicked();

private:
    Ui::LoginForm *ui;
    UserRepository* userRepo;
    QString userRole;
    int m_userId = -1;
};

#endif // LOGINFORM_H
