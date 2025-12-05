#include "guestroomdialog.h"
#include "ui_guestroomdialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDate>
#include <QSqlRecord>

GuestRoomDialog::GuestRoomDialog(int userId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GuestRoomDialog)
    , currentUserId(userId)
    , roomRepo(new RoomRepository(DatabaseManager::getInstance()))
    , bookingRepo(new BookingRepository(DatabaseManager::getInstance()))
    , model(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Вільні Номери");

    DatabaseManager* db = DatabaseManager::getInstance();
    roomRepo = new RoomRepository(db);
    bookingRepo = new BookingRepository(db);

    // Налаштування моделі
    model = new QSqlTableModel(this, db->getDB());
    model->setTable("rooms");
    model->setFilter("status = 'Вільний'"); // Показуємо тільки вільні
    model->select();

    setupTable();
}

GuestRoomDialog::~GuestRoomDialog()
{
    delete ui;
    delete roomRepo;
    delete bookingRepo;
}

void GuestRoomDialog::setupTable()
{
    model->setHeaderData(1, Qt::Horizontal, "Номер");
    model->setHeaderData(2, Qt::Horizontal, "Тип");
    model->setHeaderData(3, Qt::Horizontal, "Ціна");

    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->hideColumn(0); // ID
    ui->tableView->hideColumn(4); // Status (і так ясно)
    ui->tableView->resizeColumnsToContents();
}

void GuestRoomDialog::on_pbBook_clicked()
{
    if (!ui->tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Увага", "Оберіть номер!");
        return;
    }

    int row = ui->tableView->currentIndex().row();
    int roomId = model->record(row).value("id").toInt();
    int price = model->record(row).value("price").toInt();

    bool ok;
    QString checkIn = QInputDialog::getText(this, "Бронювання", "Дата заїзду (YYYY-MM-DD):",
                                            QLineEdit::Normal, QDate::currentDate().toString("yyyy-MM-dd"), &ok);
    if (!ok) return;

    QString checkOut = QInputDialog::getText(this, "Бронювання", "Дата виїзду (YYYY-MM-DD):",
                                             QLineEdit::Normal, QDate::currentDate().addDays(1).toString("yyyy-MM-dd"), &ok);
    if (!ok) return;

    // Розрахунок
    QDate d1 = QDate::fromString(checkIn, "yyyy-MM-dd");
    QDate d2 = QDate::fromString(checkOut, "yyyy-MM-dd");
    int days = d1.daysTo(d2);
    if (days <= 0) days = 1;
    int total = days * price;

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Підтвердження",
                                                              QString("Сума до сплати: %1 грн. Забронювати?").arg(total), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Booking booking(0, currentUserId, roomId, checkIn, checkOut, "Очікує", total);

        if (bookingRepo->addBooking(booking)) {
            // Оновлюємо статус номера
            Room* room = roomRepo->getRoomById(roomId);
            if(room) {
                room->setStatus("Заброньовано");
                roomRepo->updateRoom(*room);
                delete room;
            }
            QMessageBox::information(this, "Успіх", "Бронювання створено!");
            model->select(); // Оновити список
        } else {
            QMessageBox::warning(this, "Помилка", "Збій бази даних");
        }
    }
}

void GuestRoomDialog::on_pbClose_clicked()
{
    this->close();
}
