#include "guestbookingdialog.h"
#include "ui_guestbookingdialog.h"
#include "databasemanager.h"
#include "statusdelegate.h"
#include <QSqlRelation>
#include <QHeaderView>
#include <QString>
#include <QIcon>

GuestBookingDialog::GuestBookingDialog(int userId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GuestBookingDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/book.png"));
    setWindowTitle("Мої бронювання");

    this->setStyleSheet(QStringLiteral(
        "QDialog { background-color: #f7f7f7; }"
        "QLineEdit { border: 1px solid #c0c0c0; border-radius: 4px; padding: 6px; background-color: white; min-height: 20px; }"
        "QLineEdit:focus { border: 1px solid #3498db; }"
        "QTableView { border: 1px solid #dcdcdc; gridline-color: #eeeeee; background-color: white; selection-background-color: #ddeeff; selection-color: black; border-radius: 6px; }"
        "QHeaderView::section { background-color: #efefef; color: #333333; padding: 6px; border: 1px solid #dcdcdc; font-weight: bold; }"
        "QComboBox { border: 1px solid #b0b0b0; border-radius: 3px; padding: 1px 18px 1px 3px; min-width: 6em; }"
        ));

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
