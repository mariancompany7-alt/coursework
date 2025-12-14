#ifndef ROOMMANAGEMENT_H
#define ROOMMANAGEMENT_H

#include <QDialog>
#include <QSqlTableModel>
#include "roomrepository.h"

namespace Ui {
class RoomManagement;
}

class RoomManagement : public QDialog
{
    Q_OBJECT

public:
    explicit RoomManagement(RoomRepository* repo, bool isAdmin, QWidget *parent = nullptr);
    ~RoomManagement();

private slots:
    void on_pbAdd_clicked();
    void on_pbEdit_clicked();
    void on_pbDelete_clicked();
    void on_pbRefresh_clicked();
    void on_pbClose_clicked();

private:
    Ui::RoomManagement *ui;
    RoomRepository* roomRepo;
    QSqlTableModel* model;

    void setupTable();
    void loadData();
};

#endif // ROOMMANAGEMENT_H
