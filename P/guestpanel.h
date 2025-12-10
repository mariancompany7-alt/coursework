#ifndef GUESTPANEL_H
#define GUESTPANEL_H

#include <QWidget>

namespace Ui {
class GuestPanel;
}

class GuestPanel : public QWidget
{
    Q_OBJECT

public:
    explicit GuestPanel(QWidget *parent = nullptr);
    ~GuestPanel();

    void setCurrentUser(int userId);

signals:
    void logoutRequested();

private slots:
    void on_pbSearchRoom_clicked();
    void on_pbMyBookings_clicked();
    void on_pbLogout_clicked();

private:
    Ui::GuestPanel *ui;
    int currentUserId = -1;
};

#endif // GUESTPANEL_H
