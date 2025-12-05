#include "guestbookingdialog.h"
#include "ui_guestbookingdialog.h"
#include "databasemanager.h"
#include <QSqlRelation>
#include <QSqlRelationalDelegate>

GuestBookingDialog::GuestBookingDialog(int userId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GuestBookingDialog)
{
    ui->setupUi(this);
    setWindowTitle("Мої Бронювання");

    DatabaseManager* db = DatabaseManager::getInstance();

    // Використовуємо Relational, щоб показати номер кімнати замість ID
    model = new QSqlRelationalTableModel(this, db->getDB());
    model->setTable("bookings");

    // Фільтр тільки для цього користувача
    model->setFilter(QString("user_id = %1").arg(userId));

    // Зв'язок: room_id -> rooms.roomNumber
    model->setRelation(2, QSqlRelation("rooms", "id", "roomNumber"));

    model->select();

    model->setHeaderData(2, Qt::Horizontal, "Номер");
    model->setHeaderData(3, Qt::Horizontal, "Заїзд");
    model->setHeaderData(4, Qt::Horizontal, "Виїзд");
    model->setHeaderData(5, Qt::Horizontal, "Статус");
    model->setHeaderData(6, Qt::Horizontal, "Сума");

    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->hideColumn(0); // ID
    ui->tableView->hideColumn(1); // User ID (це я)
    ui->tableView->resizeColumnsToContents();
}

GuestBookingDialog::~GuestBookingDialog()
{
    delete ui;
}

void GuestBookingDialog::on_pbClose_clicked()
{
    this->close();
}
