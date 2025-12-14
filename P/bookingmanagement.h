#ifndef BOOKINGMANAGEMENT_H
#define BOOKINGMANAGEMENT_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include "bookingrepository.h"
#include "userrepository.h"
#include "roomrepository.h"

namespace Ui {
class BookingManagement;
}

class BookingManagement : public QDialog
{
    Q_OBJECT

public:
    explicit BookingManagement(BookingRepository* repo, QWidget *parent = nullptr);
    ~BookingManagement();

private slots:
    void on_pbChangeStatus_clicked();
    void on_pbDelete_clicked();
    void on_pbRefresh_clicked();
    void on_pbClose_clicked();
    void on_pbNewBooking_clicked();
    void handleStatusChange(const QModelIndex& topLeft, const QModelIndex& bottomRight);
private:
    Ui::BookingManagement *ui;
    BookingRepository* bookingRepo;
    UserRepository* userRepo;
    RoomRepository* roomRepo;
    QSqlRelationalTableModel* model;

    void setupTable();
    void loadData();
};

#endif // BOOKINGMANAGEMENT_H
