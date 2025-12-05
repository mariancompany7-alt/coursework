#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include <QSqlTableModel>
#include "userrepository.h"
#include "roomrepository.h"
#include "bookingrepository.h"

namespace Ui {
class AdminPanel;
}

class AdminPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPanel(QWidget *parent = nullptr);
    ~AdminPanel();

signals:
    void logoutRequested();

private:
    Ui::AdminPanel *ui;

    UserRepository* userRepo;
    BookingRepository* bookingRepo;
    RoomRepository* roomRepo;

    QSqlTableModel* bookingModel;

    void setupUI();
    void updateStatistics();
    void loadRecentBookings();

private slots:

    void on_pbRooms_clicked();
    void on_pbUsers_clicked();
    void on_pbBookings_clicked();
    void on_pbLogout_clicked();
    void on_pbRefresh_clicked();
};

#endif // ADMINPANEL_H
