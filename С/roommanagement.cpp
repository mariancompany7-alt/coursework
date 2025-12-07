#include "roommanagement.h"
#include "ui_roommanagement.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlRecord>
#include <QHeaderView>

RoomManagement::RoomManagement(RoomRepository* repo, bool isAdmin, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RoomManagement)
    , roomRepo(repo)
{
    ui->setupUi(this);
    setWindowTitle("Управління номерами");

    roomRepo = new RoomRepository(DatabaseManager::getInstance());
    setupTable();
    loadData();

    if(!isAdmin) {
        ui->pbAdd->hide();
        ui->pbDelete->hide();
    }
}

RoomManagement::~RoomManagement()
{
    delete model;
    delete roomRepo;
    delete ui;
}

void RoomManagement::setupTable()
{
    model = new QSqlTableModel(this, DatabaseManager::getInstance()->getDB());
    model->setTable("rooms");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Номер");
    model->setHeaderData(2, Qt::Horizontal, "Тип");
    model->setHeaderData(3, Qt::Horizontal, "Ціна");
    model->setHeaderData(4, Qt::Horizontal, "Статус");

    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    QHeaderView* header = ui->tableView->horizontalHeader();
    ui->tableView->hideColumn(0);
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(5, QHeaderView::ResizeToContents);
}

void RoomManagement::loadData()
{
    model->select();
}

void RoomManagement::on_pbAdd_clicked()
{
    bool ok;
    int number = QInputDialog::getInt(this, "Новий номер", "Номер кімнати:", 101, 1, 9999, 1, &ok);
    if (!ok) return;

    QStringList types = {"Стандарт", "Люкс", "Сімейний", "Президентський"};
    QString type = QInputDialog::getItem(this, "Новий номер", "Тип:", types, 0, false, &ok);
    if (!ok) return;

    double price = QInputDialog::getDouble(this, "Новий номер", "Ціна за ніч:", 500, 100, 100000, 0, &ok);
    if (!ok) return;

    Room newRoom(0, number, type, (int)price, "Вільний");

    if (roomRepo->addRoom(newRoom)) {
        QMessageBox::information(this, "Успіх", "Номер додано!");
        loadData();
    } else {
        QMessageBox::warning(this, "Помилка", "Не вдалося додати!");
    }
}

void RoomManagement::on_pbEdit_clicked()
{
    if (!ui->tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Увага", "Виберіть номер!");
        return;
    }

    int row = ui->tableView->currentIndex().row();
    int roomId = model->record(row).value("id").toInt();

    Room* room = roomRepo->getRoomById(roomId);
    if (!room) {
        QMessageBox::warning(this, "Помилка", "Номер не знайдено!");
        return;
    }

    bool ok;
    int number = QInputDialog::getInt(this, "Редагувати", "Номер кімнати:", room->getRoomNumber(), 1, 9999, 1, &ok);
    if (!ok) { delete room; return; }

    QStringList types = {"Стандарт", "Люкс", "Сімейний", "Президентський"};
    QString type = QInputDialog::getItem(this, "Редагувати", "Тип:", types, types.indexOf(room->getType()), false, &ok);
    if (!ok) { delete room; return; }

    double price = QInputDialog::getDouble(this, "Редагувати", "Ціна:", room->getPrice(), 100, 100000, 0, &ok);
    if (!ok) { delete room; return; }

    QStringList statuses = {"Вільний", "Зайнятий", "На ремонті"};
    QString status = QInputDialog::getItem(this, "Редагувати", "Статус:", statuses, statuses.indexOf(room->getStatus()), false, &ok);
    if (!ok) { delete room; return; }

    room->setRoomNumber(number);
    room->setType(type);
    room->setPrice((int)price);
    room->setStatus(status);

    if (roomRepo->updateRoom(*room)) {
        QMessageBox::information(this, "Успіх", "Оновлено!");
        loadData();
    } else {
        QMessageBox::warning(this, "Помилка", "Не вдалося оновити!");
    }

    delete room;
}

void RoomManagement::on_pbDelete_clicked()
{
    if (!ui->tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Увага", "Виберіть номер!");
        return;
    }

    int row = ui->tableView->currentIndex().row();
    int roomId = model->record(row).value("id").toInt();
    QString roomNumber = model->record(row).value("roomNumber").toString();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Підтвердження",
                                                              QString("Видалити номер '%1'?").arg(roomNumber), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (roomRepo->deleteRoom(roomId)) {
            QMessageBox::information(this, "Успіх", "Видалено!");
            loadData();
        } else {
            QMessageBox::warning(this, "Помилка", "Не вдалося видалити!");
        }
    }
}

void RoomManagement::on_pbRefresh_clicked()
{
    loadData();
}

void RoomManagement::on_pbClose_clicked()
{
    this->close();
}
