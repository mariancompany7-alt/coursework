#include "adminpanel.h"
#include "ui_adminpanel.h"
#include "databasemanager.h"
#include "bookingmanagement.h"
#include "roommanagement.h"
#include "usermanagement.h"
#include <QMessageBox>

AdminPanel::AdminPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminPanel)
{
    ui->setupUi(this);
    setWindowTitle("Адміністративна дошка");

    // 1. Отримуємо ЄДИНИЙ ЕКЗЕМПЛЯР DatabaseManager
    DatabaseManager* dbManager = DatabaseManager::getInstance();

    // 2. Отримуємо QSqlDatabase за допомогою getDB()
    QSqlDatabase db = dbManager->getDB();

    // 3. Ініціалізація членів класу (Репозиторіїв)
    //    ТЕПЕР ПЕРЕДАЄМО DatabaseManager*
    userRepo = new UserRepository(dbManager);
    bookingRepo = new BookingRepository(dbManager);
    roomRepo = new RoomRepository(dbManager);

    // 4. Ініціалізація моделі (вимагає прямо QSqlDatabase)
    bookingModel = new QSqlTableModel(this, db);

    setupUI();
    updateStatistics();
    loadRecentBookings();

}

AdminPanel::~AdminPanel()
{
    delete ui;
    // видаляємо створені в кучі об'єкти
    delete userRepo;
    delete bookingRepo;
    delete roomRepo;
}

void AdminPanel::setupUI() {

    // Налаштування таблиці бронювань
    bookingModel = new QSqlTableModel(this, DatabaseManager::getInstance()->getDB());
    bookingModel->setTable("bookings");
    bookingModel->select();

    bookingModel->setHeaderData(0, Qt::Horizontal, "ID");
    bookingModel->setHeaderData(1, Qt::Horizontal, "Користувач");
    bookingModel->setHeaderData(2, Qt::Horizontal, "Номер");
    bookingModel->setHeaderData(3, Qt::Horizontal, "Заїзд");
    bookingModel->setHeaderData(4, Qt::Horizontal, "Виїзд");
    bookingModel->setHeaderData(5, Qt::Horizontal, "Статус");
    bookingModel->setHeaderData(6, Qt::Horizontal, "Сума");

    if (ui->tableViewBookings) {
        ui->tableViewBookings->setModel(bookingModel);
        ui->tableViewBookings->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableViewBookings->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableViewBookings->resizeColumnsToContents();
    }
}

void AdminPanel::updateStatistics() {

    // Підрахунок статистики
    int totalUsers = userRepo->getAllUsers().count();

    QList<Room> rooms = roomRepo->getAllRooms();
    int totalRooms = rooms.count();

    int availableRooms = 0;
    for(const Room& room : rooms) {
        if(room.getStatus() == "Вільний") {

            availableRooms++;
        }
    }

    int activeBookings = 0;
    QList<Booking> bookings = bookingRepo->getAllBookings();
    for(const Booking& booking: bookings) {
        if(booking.getStatus() == "підтверджено" || booking.getStatus() == "заселено") {

            activeBookings++;
        }
    }

    // Оновлення лейблів
    if(ui->lblTotalUsers) {
        ui->lblTotalUsers->setText(QString("Користувачів: %1").arg(totalUsers));
    }

    if(ui->lblAvailableRooms) {
        ui->lblAvailableRooms->setText(QString("Вільних кімнат: %1").arg(availableRooms));
    }

    if(ui->lblActiveBookings) {
        ui->lblActiveBookings->setText(QString("Активних бронювань: %1").arg(activeBookings));
    }

    if(ui->lblTotalRooms) {
        ui->lblTotalRooms->setText(QString("Кімнат: %1").arg(totalRooms));
    }
}

void AdminPanel::loadRecentBookings() {

    if(bookingModel) {
        bookingModel->select();
    }
}

void AdminPanel::on_pbRooms_clicked() {

    DatabaseManager* dbManager = DatabaseManager::getInstance();

    RoomManagement* dialog = new RoomManagement(roomRepo, true, this);

    dialog->exec();
    updateStatistics();
    delete dialog;
}

void AdminPanel::on_pbUsers_clicked() {

    DatabaseManager* dbManager = DatabaseManager::getInstance();

    // Передаємо необхідну залежність (наприклад, userRepo або dbManager) у діалог
    UserManagement* dialog = new UserManagement(userRepo, this);

    dialog->exec();
    updateStatistics();
    delete dialog;
}

void AdminPanel::on_pbBookings_clicked() {

    DatabaseManager* dbManager = DatabaseManager::getInstance();

    BookingManagement* dialog = new BookingManagement(bookingRepo, this);

    dialog->exec();
    updateStatistics();
    loadRecentBookings();
    delete dialog;
}


void AdminPanel::on_pbLogout_clicked() {

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Вихід", "Вийти з системи?",
        QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {

        emit logoutRequested();
    }
}


void AdminPanel::on_pbRefresh_clicked() {

    loadRecentBookings();
    updateStatistics();
}

