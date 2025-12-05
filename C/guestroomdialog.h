#ifndef GUESTROOMDIALOG_H
#define GUESTROOMDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include "roomrepository.h"
#include "bookingrepository.h"

namespace Ui { class GuestRoomDialog; }

class GuestRoomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GuestRoomDialog(int userId, QWidget *parent = nullptr);
    ~GuestRoomDialog();

private slots:
    void on_pbBook_clicked();
    void on_pbClose_clicked();

private:
    Ui::GuestRoomDialog *ui;
    int currentUserId;
    RoomRepository* roomRepo;
    BookingRepository* bookingRepo;
    QSqlTableModel* model;

    void setupTable();
};

#endif // GUESTROOMDIALOG_H
