#include "guestpanel.h"
#include "ui_guestpanel.h"
#include "guestroomdialog.h"
#include "guestbookingdialog.h"
#include <QMessageBox>

GuestPanel::GuestPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GuestPanel)
    , currentUserId(-1)
{
    ui->setupUi(this);
}

GuestPanel::~GuestPanel()
{
    delete ui;
}

void GuestPanel::setCurrentUser(int userId)
{
    currentUserId = userId;
}

void GuestPanel::on_pbSearchRoom_clicked()
{
    if (currentUserId == -1) return;

    // Створюємо діалог пошуку і передаємо ID користувача (щоб знати, хто бронює)
    GuestRoomDialog dialog(currentUserId, this);
    dialog.exec(); // Модальне відкриття
}

void GuestPanel::on_pbMyBookings_clicked()
{
    if (currentUserId == -1) return;

    // Створюємо діалог історії
    GuestBookingDialog dialog(currentUserId, this);
    dialog.exec();
}

void GuestPanel::on_pbLogout_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Вихід", "Вийти з акаунту?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        currentUserId = -1;
        emit logoutRequested();
    }
}
