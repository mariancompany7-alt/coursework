#ifndef MANAGERPANEL_H
#define MANAGERPANEL_H

#include <QWidget>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include "userrepository.h"
#include "roomrepository.h"
#include "bookingrepository.h"

namespace Ui {
class ManagerPanel;
}

class ManagerPanel : public QWidget
{
    Q_OBJECT

signals:
    void logoutRequested();

public:
    explicit ManagerPanel(QWidget *parent = nullptr);
    ~ManagerPanel();

private:
    Ui::ManagerPanel *ui;

    UserRepository* userRepo;
    BookingRepository* bookingRepo;
    RoomRepository* roomRepo;

    QSqlRelationalTableModel* bookingModel;

    void setupUI();
    void updateStatistics();
    void loadRecentBookings();

private slots:

    void on_pbRooms_clicked();
    void on_pbBookings_clicked();
    void on_pbLogout_clicked();
    void on_pbRefresh_clicked();
};

#endif // MANAGERPANEL_H
