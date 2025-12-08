#include "bookingmanagement.h"
#include "ui_bookingmanagement.h"
#include "databasemanager.h"
#include "statusdelegate.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlRecord>
#include <QDate>
#include <QHeaderView>

BookingManagement::BookingManagement(BookingRepository* repo, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::BookingManagement),
    bookingRepo(repo)
{
    ui->setupUi(this);
    setWindowTitle("Управління бронюваннями");

    bookingRepo = new BookingRepository(DatabaseManager::getInstance());
    userRepo = new UserRepository(DatabaseManager::getInstance());
    roomRepo = new RoomRepository(DatabaseManager::getInstance());

    setupTable();
    loadData();

    connect(model, &QSqlRelationalTableModel::dataChanged, this, &BookingManagement::handleStatusChange);
}

BookingManagement::~BookingManagement()
{
    delete model;
    delete bookingRepo;
    delete userRepo;
    delete roomRepo;
    delete ui;
}

void BookingManagement::setupTable()
{
    model = new QSqlRelationalTableModel(this, DatabaseManager::getInstance()->getDB());
    model->setTable("bookings");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->setRelation(1, QSqlRelation("users", "id", "login"));
    model->setRelation(2, QSqlRelation("rooms", "id", "roomNumber"));
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Користувач");
    model->setHeaderData(2, Qt::Horizontal, "Номер");
    model->setHeaderData(3, Qt::Horizontal, "Заїзд");
    model->setHeaderData(4, Qt::Horizontal, "Виїзд");
    model->setHeaderData(5, Qt::Horizontal, "Статус");
    model->setHeaderData(6, Qt::Horizontal, "Сума");

    ui->tableView->setModel(model);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(5, new StatusDelegate(ui->tableView));
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    QHeaderView* header = ui->tableView->horizontalHeader();
    ui->tableView->hideColumn(0);
    // header->setSectionResizeMode(0, QHeaderView::Fixed);
    // header->resizeSection(0, 50);
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(5, QHeaderView::ResizeToContents);
}

void BookingManagement::loadData()
{
    model->select();
}

void BookingManagement::on_pbChangeStatus_clicked()
{
    if (!ui->tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Увага", "Виберіть бронювання для зміни статусу!");
        return;
    }

    int row = ui->tableView->currentIndex().row();
    int bookingId = model->record(row).value("id").toInt();

    bool ok;

    Booking* booking = bookingRepo->getBookingById(bookingId);
    if (!booking) {
        QMessageBox::critical(this, "Помилка", "Неможливо завантажити дані бронювання.");
        return;
    }

    QStringList statuses = {"Підтверджено", "Заселено", "Виписано", "Скасовано"};
    QString currentStatus = booking->getStatus();

    int currentIndex = statuses.indexOf(currentStatus);
    if (currentIndex == -1) currentIndex = 0;

    QString newStatus = QInputDialog::getItem(this, "Змінити статус", "Новий статус:", statuses, currentIndex, false, &ok);

    if (!ok) {
        delete booking;
        return;
    }

    booking->setStatus(newStatus);


    if (bookingRepo->updateBooking(*booking)) {

        Room* room = roomRepo->getRoomById(booking->getRoomId());

        if (room) {
            if (newStatus == "Виписано" || newStatus == "Скасовано") {
                room->setStatus("Вільний");
                roomRepo->updateRoom(*room);
            }
            else if (newStatus == "Заселено") {
                room->setStatus("Зайнятий");
                roomRepo->updateRoom(*room);
            }
            else if (newStatus == "Підтверджено") {
                if (room->getStatus().toLower() == "Вільний" || room->getStatus().toLower() == "available") {
                    room->setStatus("Заброньовано");
                    roomRepo->updateRoom(*room);
                }
            }
            delete room;
        }


        QMessageBox::information(this, "Успіх", "Статус оновлено!");
        loadData();
    } else {
        QMessageBox::warning(this, "Помилка", "Не вдалося оновити!");
    }

    delete booking;
}

void BookingManagement::on_pbDelete_clicked()
{
    if (!ui->tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Увага", "Виберіть бронювання!");
        return;
    }

    int row = ui->tableView->currentIndex().row();
    int bookingId = model->record(row).value("id").toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Підтвердження",
                                                              "Видалити бронювання?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {

        Booking* booking = bookingRepo->getBookingById(bookingId);

        if (booking) {
            Room* room = roomRepo->getRoomById(booking->getRoomId());
            if (room) {
                room->setStatus("Вільний");
                roomRepo->updateRoom(*room);
                delete room;
            }
        }

        if (bookingRepo->deleteBooking(bookingId)) {
            QMessageBox::information(this, "Успіх", "Видалено!");
            loadData();
        } else {
            QMessageBox::warning(this, "Помилка", "Не вдалося видалити!");
        }

        if (booking) {
            delete booking;
        }
    }
}

void BookingManagement::on_pbNewBooking_clicked()
{
    bool ok;

    QList<User> users = userRepo->getAllUsers();

    QStringList userItems;

    for(const User& u : users) {
        QString role = u.getRole();

        if (role != "admin" && role != "manager") {

            userItems.append(QString("%1: %2 (%3)")
                            .arg(u.getId())
                            .arg(u.getLogin())
                            .arg(u.getRole()));
        }
    }

    if (userItems.isEmpty()) {
        QMessageBox::critical(this, "Помилка", "У базі немає користувачів");
        return;
    }

    QString selectedUserStr = QInputDialog::getItem(this,
        "Нове бронювання", "Оберіть користувача", userItems, 0, false, &ok);

    if (!ok) return;

    int userId = selectedUserStr.split(":").first().toInt();

    QList<Room> allRooms = roomRepo->getAllRooms();
    if (allRooms.isEmpty()) {
        QMessageBox::warning(this, "Увага", "Немає вільних номерів для бронювання!");
        return;
    }

    QStringList roomItems;
    QList<Room> availableRoomsForSelection;
    for (const Room& r : allRooms) {
        roomItems.append(QString("#%1 - %2 (%3грн)")
                                .arg(r.getRoomNumber())
                                .arg(r.getType())
                                .arg(r.getPrice())
                                .arg(r.getStatus()));
        availableRoomsForSelection.append(r);
    }

    if (userItems.isEmpty()) {
        QMessageBox::critical(this, "Помилка", "У базі немає користувачів");
        return;
    }

    if (availableRoomsForSelection.isEmpty()) {
        QMessageBox::critical(this, "Увага", "Немає доступних номерів для бронювання!");
    }

    QString selectedRoomStr = QInputDialog::getItem(this, "Нове бронювання", "Оберіть номер:", roomItems, 0, false, &ok);
    if (!ok) return;

    int roomIndex = roomItems.indexOf(selectedRoomStr);
    Room selectedRoom = availableRoomsForSelection[roomIndex];
    int roomId = selectedRoom.getId();


    QString checkInStr = QInputDialog::getText(this, "Нове бронювання",
                                               "Дата заїзду (YYYY-MM-DD):", QLineEdit::Normal,
                                               QDate::currentDate().toString("yyyy-MM-dd"), &ok);
    if (!ok || checkInStr.isEmpty()) return;

    QString checkOutStr = QInputDialog::getText(this, "Нове бронювання",
                                                "Дата виїзду (YYYY-MM-DD):", QLineEdit::Normal,
                                                QDate::currentDate().addDays(1).toString("yyyy-MM-dd"), &ok);
    if (!ok || checkOutStr.isEmpty()) return;

    QDate d1 = QDate::fromString(checkInStr, "yyyy-MM-dd");
    QDate d2 = QDate::fromString(checkOutStr, "yyyy-MM-dd");
    int days = d1.daysTo(d2);
    if (days <= 0) days = 1;

    int calculatedPrice = days * selectedRoom.getPrice();

    int totalPrice = QInputDialog::getInt(this, "Нове бронювання",
                                          QString("Загальна ціна (%1 днів):").arg(days),
                                          calculatedPrice, 0, 1000000, 1, &ok);
    if (!ok) return;

    if (bookingRepo->isRoomBookedInPeriod(roomId, checkInStr, checkOutStr)) {
        QMessageBox::critical(this, "Помилка", "Цей номер вже заброньовано або зайнятий у вибраний період!");
    }

    QStringList statuses = {"Підтверджено", "Очікує", "Заселено"};
    QString status = QInputDialog::getItem(this, "Нове бронювання", "Статус:", statuses, 0, false, &ok);
    if (!ok) return;

    Booking newBooking;
    newBooking.setUserId(userId);
    newBooking.setRoomId(roomId);
    newBooking.setCheckIn(checkInStr);
    newBooking.setCheckOut(checkOutStr);
    newBooking.setTotalPrice(totalPrice);
    newBooking.setStatus(status);

    if (bookingRepo->addBooking(newBooking)) {
        if (status == "Підтверджено" || status == "Заселено") {
            selectedRoom.setStatus("Заброньовано");
            roomRepo->updateRoom(selectedRoom);
        }
        QMessageBox::information(this, "Успіх", "Бронювання успішно створено!");
        loadData();
    } else {
        QMessageBox::critical(this, "Помилка", "Не вдалося зберегти бронювання у базі.");
    }
}

void BookingManagement::on_pbRefresh_clicked()
{
    loadData();
}

void BookingManagement::handleStatusChange(const QModelIndex& topLeft, const QModelIndex& /*bottomRight*/) {

    if(topLeft.column() !=5) {
        return;
    }

    int row  = topLeft.row();
    int bookingId = model->data(model->index(row, 0)).toInt();
    QString newStatus = model->data(topLeft).toString();

    Booking* booking = bookingRepo->getBookingById(bookingId);
    if(!booking) {
        qDebug() << "Бронювання не знайдено за айді!";
        return;
    }

    int roomId = booking->getRoomId();
    delete booking;

    Room* room = roomRepo->getRoomById(roomId);
    if(!room) {
        qDebug() << "Номера не знайдено!";
        return;
    }

    QString currentRoomStatus = room->getStatus();

    if(newStatus == "Виписано" || newStatus == "Скасовано") {
        room->setStatus("Вільний");
        roomRepo->updateRoom(*room);

    } else if (newStatus == "Заселено") {
        room->setStatus("Зайнятий");
        roomRepo->updateRoom(*room);

    } else if (newStatus  == "Підтверджено" || newStatus == "Очікує") {

        if(currentRoomStatus == "Вільний")
        {
            room->setStatus("Заброньовано");
            roomRepo->updateRoom(*room);
        }
    }

    delete room;

}

void BookingManagement::on_pbClose_clicked()
{
    this->close();
}
