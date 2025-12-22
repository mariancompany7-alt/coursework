#include "databasemanager.h"
#include "userrepository.h"
#include <QSqlQuery>
#include <QDebug>
#include <QFile>

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() {

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./GalicianComfortHotel.db");
}

QSqlDatabase& DatabaseManager::getDB() {
    return db;
}

DatabaseManager* DatabaseManager::getInstance() {

    if (instance == nullptr) {
        instance = new DatabaseManager();
    }
    return instance;
}

bool DatabaseManager::openDataBase() {

    if (!db.open()) {
        qDebug() << "Помилка відкриття БД";
    } else {
        qDebug() << "БД відкрита успішно!";
        createTables();
    }
    return true;
}

bool DatabaseManager::createTables() {

    QSqlQuery query(db);
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS users("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "login TEXT NOT NULL UNIQUE, "
            "password_hash TEXT NOT NULL, "
            "salt TEXT NOT NULL, "
            "role TEXT DEFAULT 'guest')"
            )) {
        qDebug() << "Error creating users table:";
        return false;
    }
    qDebug() <<  "Таблиця 'users' створена!";

    if(!query.exec("CREATE TABLE IF NOT EXISTS rooms("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "roomNumber INTEGER NOT NULL UNIQUE, "
                    "type TEXT NOT NULL, "
                    "price INTEGER NOT NULL, "
                    "status TEXT DEFAULT 'Вільний')")) {
        qDebug() << "Error creating rooms table:";
        return false;
    }
    qDebug() << "Table 'rooms' created!";

    if(!query.exec("CREATE TABLE IF NOT EXISTS bookings("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "user_id INTEGER NOT NULL, "
                    "room_id INTEGER NOT NULL, "
                    "check_in TEXT NOT NULL, "
                    "check_out TEXT NOT NULL, "
                    "status TEXT DEFAULT 'підтверджено', "
                    "total_price INTEGER, "
                    "FOREIGN KEY(user_id) REFERENCES users(id), "
                    "FOREIGN KEY(room_id) REFERENCES rooms(id))")) {
        qDebug() << "Error creating bookings table:";
        return false;
    }
    qDebug() << "Table 'bookings' created!";

    if(!userExists("admin")) {
        User adminUser;
        adminUser.setRole("admin");
        adminUser.setLogin("admin@hotel.com");
        adminUser.setPassword("admin123");

        UserRepository userRepo(this);
        if(userRepo.addUser(adminUser)) {
            qDebug() << "Адмін 'admin' доданий!";
        } else {
            qDebug() << "КРИТИЧНА ПОМИЛКА: Не вдалося додати адміна!";
        }
    }

    return true;
}

bool DatabaseManager::userExists(const QString& login) {
    UserRepository userRepo(this);

    User* user = userRepo.findByLogin(login);

    if (user != nullptr) {
        delete user;
        return true;
    }
    return false;
}


void DatabaseManager::closeDataBase() {
    db.close();
}
