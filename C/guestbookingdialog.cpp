#include "guestbookingdialog.h"
#include "ui_guestbookingdialog.h"
#include "databasemanager.h"
#include "statusdelegate.h"
#include <QSqlRelation>
#include <QHeaderView>

GuestBookingDialog::GuestBookingDialog(int userId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GuestBookingDialog)
{
    ui->setupUi(this);
    setWindowTitle("Мої бронювання");

    DatabaseManager* db = DatabaseManager::getInstance();

    model = new QSqlRelationalTableModel(this, db->getDB());
    model->setTable("bookings");

    model->setFilter(QString("user_id = %1").arg(userId));

    model->setRelation(2, QSqlRelation("rooms", "id", "roomNumber"));

    model->select();

    model->setHeaderData(2, Qt::Horizontal, "Номер");
    model->setHeaderData(3, Qt::Horizontal, "Заїзд");
    model->setHeaderData(4, Qt::Horizontal, "Виїзд");
    model->setHeaderData(5, Qt::Horizontal, "Статус");
    model->setHeaderData(6, Qt::Horizontal, "Сума");

    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(5, new StatusDelegate(ui->tableView));
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    QHeaderView* header = ui->tableView->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->resizeSection(0, 50);
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(5, QHeaderView::ResizeToContents);
}

GuestBookingDialog::~GuestBookingDialog()
{
    delete ui;
}

void GuestBookingDialog::on_pbClose_clicked()
{
    this->close();
}
