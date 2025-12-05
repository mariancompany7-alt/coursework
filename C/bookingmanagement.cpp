#include "bookingmanagement.h"
#include "ui_bookingmanagement.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlRecord>
#include <QDate>

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
    model = new QSqlTableModel(this, DatabaseManager::getInstance()->getDB());
    model->setTable("bookings");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "ID Користувача");
    model->setHeaderData(2, Qt::Horizontal, "ID Номера");
    model->setHeaderData(3, Qt::Horizontal, "Заїзд");
    model->setHeaderData(4, Qt::Horizontal, "Виїзд");
    model->setHeaderData(5, Qt::Horizontal, "Статус");
    model->setHeaderData(6, Qt::Horizontal, "Сума");

    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
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

    bool ok; // Оголошуємо змінну для QInputDialog

    // 1. Отримання об'єкта Booking (ОБОВ'ЯЗКОВО)
    Booking* booking = bookingRepo->getBookingById(bookingId);
    if (!booking) {
        QMessageBox::critical(this, "Помилка", "Неможливо завантажити дані бронювання.");
        return;
    }

    // 2. Отримання нового статусу
    QStringList statuses = {"підтверджено", "заселено", "виписано", "скасовано"};
    QString currentStatus = booking->getStatus();

    // Встановлення поточного індексу
    int currentIndex = statuses.indexOf(currentStatus);
    if (currentIndex == -1) currentIndex = 0;

    QString newStatus = QInputDialog::getItem(this, "Змінити статус", "Новий статус:", statuses, currentIndex, false, &ok);

    if (!ok) {
        delete booking; // Звільняємо пам'ять, якщо діалог скасовано
        return;
    }

    // Встановлюємо новий статус
    booking->setStatus(newStatus);


    // 3. Оновлення бронювання та статусу номера
    if (bookingRepo->updateBooking(*booking)) {

        // --- Логіка оновлення статусу номера ---
        Room* room = roomRepo->getRoomById(booking->getRoomId());

        if (room) {
            // Звільнити номер: Виписано (Завершено) або Скасовано
            if (newStatus == "виписано" || newStatus == "скасовано") {
                room->setStatus("Вільний"); // ✅ ВИПРАВЛЕНО: Стандартизовано на 'Вільний'
                roomRepo->updateRoom(*room);
            }
            // Зайняти номер: Заселено
            else if (newStatus == "заселено") {
                room->setStatus("Зайнятий");
                roomRepo->updateRoom(*room);
            }
            // Забронювати номер: Підтверджено
            else if (newStatus == "підтверджено") {
                // Якщо номер був вільний (або "available"), переводимо його в "Заброньовано"
                if (room->getStatus().toLower() == "вільний" || room->getStatus().toLower() == "available") {
                    room->setStatus("Заброньовано");
                    roomRepo->updateRoom(*room);
                }
            }
            delete room; // ✅ ВИПРАВЛЕНО: Звільняємо пам'ять за Room
        }


        QMessageBox::information(this, "Успіх", "Статус оновлено!");
        loadData();
    } else {
        QMessageBox::warning(this, "Помилка", "Не вдалося оновити!");
    }

    delete booking; // ✅ Звільняємо пам'ять за Booking
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

        // 1. Звільнити номер ДО видалення бронювання
        Booking* booking = bookingRepo->getBookingById(bookingId);

        if (booking) {
            Room* room = roomRepo->getRoomById(booking->getRoomId());
            if (room) {
                room->setStatus("Вільний"); // ✅ ВИПРАВЛЕНО: Стандартизовано на 'Вільний'
                roomRepo->updateRoom(*room);
                delete room;
            }
        }

        // 2. Видалення бронювання
        if (bookingRepo->deleteBooking(bookingId)) {
            QMessageBox::information(this, "Успіх", "Видалено!");
            loadData();
        } else {
            QMessageBox::warning(this, "Помилка", "Не вдалося видалити!");
        }

        // 3. Звільнення об'єкта Booking, якщо він був успішно отриманий
        if (booking) {
            delete booking;
        }
    }
}

void BookingManagement::on_pbNewBooking_clicked()
{
    bool ok;

    // --- КРОК 1: Вибір Користувача зі списку ---
    QList<User> users = userRepo->getAllUsers();

    QStringList userItems;

    for(const User& u : users) {
        // ФІЛЬТР: Показуємо тільки гостей (не адмінів і не менеджерів)
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

    // Витягуємо ID з рядка
    int userId = selectedUserStr.split(":").first().toInt();

    // --- КРОК 2: Вибір Номера зі списку вільних ---
    QList<Room> rooms = roomRepo->getAvailableRooms();
    if (rooms.isEmpty()) {
        QMessageBox::warning(this, "Увага", "Немає вільних номерів для бронювання!");
        return;
    }

    QStringList roomItems;
    for (const Room& r : rooms) {
        roomItems.append(QString("#%1 - %2 (%3грн)")
                                .arg(r.getRoomNumber())
                                .arg(r.getType())
                                .arg(r.getPrice()));
    }

    if (userItems.isEmpty()) {
        QMessageBox::critical(this, "Помилка", "У базі немає користувачів");
        return;
    }

    QString selectedRoomStr = QInputDialog::getItem(this, "", "", roomItems, 0, false, &ok);

    if (!ok) return;

    // Щоб отримати ID номера, нам треба знайти об'єкт Room, який відповідає вибраному рядку.
    // Оскільки ми знаємо індекс вибору, можемо взяти кімнату з початкового списку 'rooms'.
    int roomIndex = roomItems.indexOf(selectedRoomStr);
    Room selectedRoom = rooms[roomIndex];
    int roomId = selectedRoom.getId();


    // --- КРОК 3: Дати ---
    QString checkInStr = QInputDialog::getText(this, "Нове бронювання",
                                               "Дата заїзду (YYYY-MM-DD):", QLineEdit::Normal,
                                               QDate::currentDate().toString("yyyy-MM-dd"), &ok);
    if (!ok || checkInStr.isEmpty()) return;

    QString checkOutStr = QInputDialog::getText(this, "Нове бронювання",
                                                "Дата виїзду (YYYY-MM-DD):", QLineEdit::Normal,
                                                QDate::currentDate().addDays(1).toString("yyyy-MM-dd"), &ok);
    if (!ok || checkOutStr.isEmpty()) return;


    // --- КРОК 4: Розрахунок ціни ---
    // Можна автоматично порахувати ціну: (Кількість днів * Ціна номера)
    QDate d1 = QDate::fromString(checkInStr, "yyyy-MM-dd");
    QDate d2 = QDate::fromString(checkOutStr, "yyyy-MM-dd");
    int days = d1.daysTo(d2);
    if (days <= 0) days = 1; // Мінімум 1 день

    int calculatedPrice = days * selectedRoom.getPrice();

    int totalPrice = QInputDialog::getInt(this, "Нове бронювання",
                                          QString("Загальна ціна (%1 днів):").arg(days),
                                          calculatedPrice, 0, 1000000, 1, &ok);
    if (!ok) return;

    // --- КРОК 5: Статус ---
    QStringList statuses = {"підтверджено", "очікує", "заселено"};
    QString status = QInputDialog::getItem(this, "Нове бронювання", "Статус:", statuses, 0, false, &ok);
    if (!ok) return;

    // --- 7. Створення об'єкта Booking ---
    Booking newBooking;
    newBooking.setUserId(userId);
    newBooking.setRoomId(roomId);
    newBooking.setCheckIn(checkInStr);
    newBooking.setCheckOut(checkOutStr);
    newBooking.setTotalPrice(totalPrice);
    newBooking.setStatus(status);

    if (bookingRepo->addBooking(newBooking)) {
        // Змінюємо статус номера на "Заброньовано", якщо бронювання підтверджено
        if (status == "підтверджено" || status == "заселено") {
            selectedRoom.setStatus("Заброньовано");
            roomRepo->updateRoom(selectedRoom);
        }
        QMessageBox::information(this, "Успіх", "Бронювання успішно створено!");
        loadData(); // Оновити таблицю
    } else {
        QMessageBox::critical(this, "Помилка", "Не вдалося зберегти бронювання у базі.");
    }
}

void BookingManagement::on_pbRefresh_clicked()
{
    loadData();
}

void BookingManagement::on_pbClose_clicked()
{
    this->close();
}
