#ifndef GUESTBOOKINGSDIALOG_H
#define GUESTBOOKINGSDIALOG_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>

namespace Ui { class GuestBookingDialog; }

class GuestBookingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GuestBookingDialog(int userId, QWidget *parent = nullptr);
    ~GuestBookingDialog();

private slots:
    void on_pbClose_clicked();

private:
    Ui::GuestBookingDialog *ui;
    QSqlRelationalTableModel* model;
};
#endif // GUESTBOOKINGSDIALOG_H
