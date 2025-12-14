#include "guestroomdialog.h"
#include "ui_guestroomdialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QDate>
#include <QSqlRecord>
#include <QHeaderView>
#include <QString>
#include <QIcon>

GuestRoomDialog::GuestRoomDialog(int userId, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GuestRoomDialog)
    , currentUserId(userId)
    , roomRepo(new RoomRepository(DatabaseManager::getInstance()))
    , bookingRepo(new BookingRepository(DatabaseManager::getInstance()))
    , model(nullptr)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/book.png"));
    setWindowTitle("Список номерів");

    this->setStyleSheet(QStringLiteral(
        "QDialog { background-color: #f7f7f7; }"
        "QLineEdit { border: 1px solid #c0c0c0; border-radius: 4px; padding: 6px; background-color: white; min-height: 20px; }"
        "QLineEdit:focus { border: 1px solid #3498db; }"
        "QTableView { border: 1px solid #dcdcdc; gridline-color: #eeeeee; background-color: white; selection-background-color: #ddeeff; selection-color: black; border-radius: 6px; }"
        "QHeaderView::section { background-color: #efefef; color: #333333; padding: 6px; border: 1px solid #dcdcdc; font-weight: bold; }"
        ));

    DatabaseManager* db = DatabaseManager::getInstance();
    roomRepo = new RoomRepository(db);
    bookingRepo = new BookingRepository(db);

    model = new QSqlRelationalTableModel(this, db->getDB());
    model->setTable("rooms");
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
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(4);
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

void GuestRoomDialog::on_pbBook_clicked()
{
    if (!ui->tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Увага", "Оберіть номер!");
        return;
    }

    int row = ui->tableView->currentIndex().row();
    int roomId = model->record(row).value("id").toInt();
    int price = model->record(row).value("price").toInt();

    Room* room = roomRepo->getRoomById(roomId);
    if (!room) {
        QMessageBox::critical(this, "Помилка", "Номер не знайдено!");
        return;
    }

    QString bookedInfo = bookingRepo->getBookedDatesForRoom(roomId);
    QMessageBox::information(this, QString("Повідомлення").arg(room->getRoomNumber()), bookedInfo);

    delete room;

    bool ok;
    QString checkIn = QInputDialog::getText(this, "Бронювання", "Дата заїзду (YYYY-MM-DD):",
                                            QLineEdit::Normal, QDate::currentDate().toString("yyyy-MM-dd"), &ok);
    if (!ok) return;

    QString checkOut = QInputDialog::getText(this, "Бронювання", "Дата виїзду (YYYY-MM-DD):",
                                             QLineEdit::Normal, QDate::currentDate().addDays(1).toString("yyyy-MM-dd"), &ok);
    if (!ok) return;

    QDate d1 = QDate::fromString(checkIn, "yyyy-MM-dd");
    QDate d2 = QDate::fromString(checkOut, "yyyy-MM-dd");

    if (!d1.isValid() || !d2.isValid()) {
        QMessageBox::critical(this, "Помилка", "Невірний формат дати. Використовуйте YYYY-MM-DD.");
        return;
    }

    if (d1 >= d2) {
        QMessageBox::critical(this, "Помилка", "Дата виїзду має бути пізнішою за дату заїзду.");
        return;
    }

    if (bookingRepo->isRoomBookedInPeriod(roomId, checkIn, checkOut)) {
        QMessageBox::critical(this, "Помилка", "Цей номер вже заброньовано або зайнятий у вибраний період!");
        return;
    }

    int days = d1.daysTo(d2);
    if (days <= 0) days = 1;
    int total = days * price;

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Підтвердження",
                                                              QString("Сума до сплати: %1 грн. Забронювати?").arg(total), QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Booking booking(0, currentUserId, roomId, checkIn, checkOut, "Очікує", total);

        if (bookingRepo->addBooking(booking)) {
            Room* r = roomRepo->getRoomById(roomId);
            if(r) {
                if (r->getStatus() == "Вільний") {
                    r->setStatus("Заброньовано");
                    roomRepo->updateRoom(*r);
                }
                delete r;
            }
            QMessageBox::information(this, "Успіх", "Бронювання створено!");
            model->select();
        } else {
            QMessageBox::warning(this, "Помилка", "Збій бази даних");
        }
    }
}

void GuestRoomDialog::on_pbClose_clicked()
{
    this->close();
}
