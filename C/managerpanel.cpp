#include "managerpanel.h"
#include "ui_managerpanel.h"
#include "databasemanager.h"
#include "bookingmanagement.h"
#include "roommanagement.h"
#include <QMessageBox>

ManagerPanel::ManagerPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ManagerPanel)
    , bookingModel(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Менеджер дошка");

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

void ManagerPanel::setupUI() {

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

    // Приховуємо ID стовпчик
    // if (bookingsModel->columnCount() > 0) {
    //     ui->tableViewRecentBookings->hideColumn(0);
    // }

    if (ui->tableViewBookings) {
        ui->tableViewBookings->setModel(bookingModel);
        // Приховуємо ID стовпчик
        ui->tableViewBookings->hideColumn(0);
        ui->tableViewBookings->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableViewBookings->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableViewBookings->resizeColumnsToContents();
    }
}

void ManagerPanel::updateStatistics() {

    // Підрахунок статистики
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

void ManagerPanel::loadRecentBookings() {

    if(bookingModel) {
        bookingModel->select();
    }
}

void ManagerPanel::on_pbRooms_clicked() {

    DatabaseManager* dbManager = DatabaseManager::getInstance();

    RoomManagement* dialog = new RoomManagement(roomRepo, false, this);

    dialog->exec();
    updateStatistics();
    delete dialog;
}

void ManagerPanel::on_pbBookings_clicked() {

    DatabaseManager* dbManager = DatabaseManager::getInstance();

    BookingManagement* dialog = new BookingManagement(bookingRepo, this);

    dialog->exec();
    updateStatistics();
    loadRecentBookings();
    delete dialog;
}

void ManagerPanel::on_pbLogout_clicked() {

    QMessageBox::StandardButton reply = QMessageBox::question
        (this, "Вихід", "Вийти із системи?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {

        emit logoutRequested();
    }
}

void ManagerPanel::on_pbRefresh_clicked() {

    loadRecentBookings();
    updateStatistics();
}

ManagerPanel::~ManagerPanel()
{
    delete ui;
    delete userRepo;
    delete roomRepo;
    delete bookingRepo;
}
