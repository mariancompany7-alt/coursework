#include "usermanagement.h"
#include "ui_usermanagement.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSqlRecord>
#include <QHeaderView>
#include <QString>

UserManagement::UserManagement(UserRepository* repo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserManagement)
    , userRepo(repo)
{
    ui->setupUi(this);
    setWindowTitle("Управління користувачами");

    this->setStyleSheet(QStringLiteral(
        "QDialog { background-color: #f7f7f7; }"
        "QLineEdit { border: 1px solid #c0c0c0; border-radius: 4px; padding: 2px; background-color: white; min-height: 25px; }"
        "QLineEdit:focus { border: 1px solid #3498db; }"
        "QTableView { border: 1px solid #dcdcdc; gridline-color: #eeeeee; background-color: white; selection-background-color: #ddeeff; selection-color: black; border-radius: 6px; }"
        "QHeaderView::section { background-color: #efefef; color: #333333; padding: 6px; border: 1px solid #dcdcdc; font-weight: bold; }"
        ));

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

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Логін");
    model->setHeaderData(2, Qt::Horizontal, "Хеш паролю");
    model->setHeaderData(3, Qt::Horizontal, "Сіль");
    model->setHeaderData(4, Qt::Horizontal, "Роль");

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();

    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* header = ui->tableView->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->resizeSection(0, 50);
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents);
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

    QString confirmPassword = QInputDialog::getText(this, "Новий користувач", "Підтвердіть пароль:", QLineEdit::Password, "", &ok);
    if (!ok || confirmPassword.isEmpty()) return;

    if (password != confirmPassword) {
        QMessageBox::critical(this, "Помилка", "Паролі не співпадають!");
        return;
    }

    if(login.length() < 5) {
        QMessageBox::critical(this, "Помилка", "Логін повинен бути не менше 5 символів!");
        return;
    }

    if(password.length() < 1) {
        QMessageBox::critical(this, "Помилка", "Пароль повинен бути не менше 1 символа!");
        return;
    }

    bool hasLowerCase = false;

    // for(const QChar &ch : password) {
    //     if(ch.isLower()) {
    //         hasLowerCase = true;
    //     }
    // }

    for(const QChar &ch : login) {
        if(ch.isLower()) {
            hasLowerCase = true;
        }
    }

    if(!hasLowerCase) {
        QMessageBox::critical(this, "Помилка", "Логін повинен міститі принаймні одну малу літеру");
        return;
    }

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
