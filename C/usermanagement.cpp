#include "usermanagement.h"
#include "ui_usermanagement.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlRecord>

UserManagement::UserManagement(UserRepository* repo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManagement)
    , userRepo(repo) // Зберігаємо репозиторій
{
    ui->setupUi(this);
    setWindowTitle("Управління користувачами");
    setupTable();
    loadData();
}

UserManagement::~UserManagement()
{
    delete model;
    delete userRepo;
    delete ui;
}

void UserManagement::setupTable()
{
    model = new QSqlTableModel(this, DatabaseManager::getInstance()->getDB());
    model->setTable("users");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // Виправлено під поля класу User: id, login, passwordHash, salt, role
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Логін");
    model->setHeaderData(2, Qt::Horizontal, "Хеш паролю");
    model->setHeaderData(3, Qt::Horizontal, "Сіль");
    model->setHeaderData(4, Qt::Horizontal, "Роль");

    ui->tableView->setModel(model);

    // Приховуємо технічні поля паролю
    ui->tableView->hideColumn(2); // passwordHash
    ui->tableView->hideColumn(3); // salt

    // ПЕРЕВІРКА: Якщо ви приховали більше колонок, можливо, потрібно hideColumn(5) для Role
    // Якщо таблиця 'users' має 5 колонок (ID, login, hash, salt, role), то Role - це 4-та колонка (індекс 4).
    // Старий код: ui->tableView->hideColumn(5); - це помилково, якщо колонок лише 5.
    // Якщо ви хочете відображати Role (індекс 4), то 5-ту колонку приховувати не потрібно.

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
}

void UserManagement::loadData()
{
    model->select();
}

void UserManagement::on_pbAdd_clicked()
{
    bool ok;
    QString login = QInputDialog::getText(this, "Новий користувач", "Логін:", QLineEdit::Normal, "", &ok);
    if (!ok || login.isEmpty()) return;

    QString password = QInputDialog::getText(this, "Новий користувач", "Пароль:", QLineEdit::Password, "", &ok);
    if (!ok || password.isEmpty()) return;

    QStringList roles = {"guest", "manager", "admin"};
    QString role = QInputDialog::getItem(this, "Новий користувач", "Роль:", roles, 0, false, &ok);
    if (!ok) return;

    User newUser(login, password, role);

    if (userRepo->addUser(newUser)) {
        QMessageBox::information(this, "Успіх", "Користувача додано!");
        loadData();
    } else {
        QMessageBox::warning(this, "Помилка", "Не вдалося додати! Можливо, логін вже існує.");
    }
}

void UserManagement::on_pbEdit_clicked()
{
    if (!ui->tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Увага", "Виберіть користувача!");
        return;
    }

    int row = ui->tableView->currentIndex().row();
    int userId = model->record(row).value("id").toInt();

    // ВИПРАВЛЕННЯ: Використовуємо findById(), як оголошено в UserRepository
    User* user = userRepo->findById(userId);
    if (!user) {
        QMessageBox::warning(this, "Помилка", "Користувача не знайдено!");
        return;
    }

    bool ok;

    QStringList roles = {"guest", "manager", "admin"};
    QString role = QInputDialog::getItem(this, "Редагувати", "Роль:", roles, roles.indexOf(user->getRole()), false, &ok);
    if (!ok) { delete user; return; }

    user->setRole(role);

    if (userRepo->updateUser(*user)) {
        QMessageBox::information(this, "Успіх", "Оновлено!");
        loadData();
    } else {
        QMessageBox::warning(this, "Помилка", "Не вдалося оновити!");
    }

    delete user;
}

void UserManagement::on_pbDelete_clicked()
{
    if (!ui->tableView->selectionModel()->hasSelection()) {
        QMessageBox::warning(this, "Увага", "Виберіть користувача!");
        return;
    }

    int row = ui->tableView->currentIndex().row();
    int userId = model->record(row).value("id").toInt();
    QString login = model->record(row).value("login").toString();

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Підтвердження",
                                                              QString("Видалити '%1'?").arg(login), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (userRepo->deleteUser(userId)) {
            QMessageBox::information(this, "Успіх", "Видалено!");
            loadData();
        } else {
            QMessageBox::warning(this, "Помилка", "Не вдалося видалити!");
        }
    }
}

void UserManagement::on_pbRefresh_clicked()
{
    loadData();
}

void UserManagement::on_pbClose_clicked()
{
    this->close();
}
