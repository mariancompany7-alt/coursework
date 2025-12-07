#include "managerpanel.h"
#include "ui_managerpanel.h"
#include "databasemanager.h"
#include "bookingmanagement.h"
#include "roommanagement.h"
#include "statusdelegate.h"
#include <QMessageBox>
#include <QIcon>
#include <QHeaderView>

ManagerPanel::ManagerPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ManagerPanel)
    , bookingModel(nullptr)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/adminPanel_0.png"));
    setWindowTitle("Менеджер дошка");

    DatabaseManager* dbManager = DatabaseManager::getInstance();

    QSqlDatabase db = dbManager->getDB();

    userRepo = new UserRepository(dbManager);
    bookingRepo = new BookingRepository(dbManager);
    roomRepo = new RoomRepository(dbManager);

    bookingModel = new QSqlRelationalTableModel(this, db);

    setupUI();
    updateStatistics();
    loadRecentBookings();
}

void ManagerPanel::setupUI() {

    bookingModel = new QSqlRelationalTableModel(this, DatabaseManager::getInstance()->getDB());
    bookingModel->setTable("bookings");
    bookingModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    bookingModel->setRelation(1, QSqlRelation("users", "id", "login"));
    bookingModel->setRelation(2, QSqlRelation("rooms", "id", "roomNumber"));
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
        ui->tableViewBookings->setItemDelegate(new QSqlRelationalDelegate(ui->tableViewBookings));
        ui->tableViewBookings->setItemDelegateForColumn(5, new StatusDelegate(ui->tableViewBookings));
        ui->tableViewBookings->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableViewBookings->hideColumn(0);
        ui->tableViewBookings->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableViewBookings->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableViewBookings->resizeColumnsToContents();
        ui->tableViewBookings->horizontalHeader()->setStretchLastSection(true);
    }

    QHeaderView* header = ui->tableViewBookings->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->resizeSection(0, 50);
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(5, QHeaderView::ResizeToContents);

}

void ManagerPanel::updateStatistics() {

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
        if(booking.getStatus() == "Підтверджено" || booking.getStatus() == "Заселено") {

            activeBookings++;
        }
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
