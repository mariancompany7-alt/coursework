#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <QDialog>
#include <QSqlTableModel>
#include "userrepository.h"

namespace Ui {
class UserManagement;
}

class UserManagement : public QDialog
{
    Q_OBJECT

public:
    explicit UserManagement(UserRepository* repo, QWidget *parent = nullptr);
    ~UserManagement();

private slots:
    void on_pbAdd_clicked();
    void on_pbEdit_clicked();
    void on_pbDelete_clicked();
    void on_pbRefresh_clicked();
    void on_pbClose_clicked();

private:
    Ui::UserManagement *ui;
    UserRepository* userRepo;
    QSqlTableModel* model;

    void setupTable();
    void loadData();
};

#endif // USERMANAGEMENT_H
