#include "databasemanager.h"
#include <QDebug>
#include <QFile>

DatabaseManager::DatabaseManager() {}

bool DatabaseManager::connect() {

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./HotelAdministrator.db");

    if(!db.open()) {
        lastError = db.lastError().text();
        qDebug() << "Failed to open database:" << lastError;
        return false;
    }

    qDebug() << "Database opened successfully!";
    return true;
}

bool DatabaseManager::createTables() {
    QSqlQuery query(db);

    if(!query.exec("CREATE TABLE IF NOT EXISTS users(name TEXT, email TEXT, password TEXT)")) {
        lastError = "Failed to create table: " + query.lastError().text();
        qDebug() << lastError;
        return false;
    }

    qDebug() << "Table 'users' successfully created!";
    qDebug() << "Tables in DB:" << db.tables();
    return true;
}

bool DatabaseManager::userExists(const QString &email) {

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM users WHERE email = :email");
    query.bindValue(":email", email);

    if(!query.exec()) {
        lastError = "Failed to check user existence: " + query.lastError().text();
        qDebug() << lastError;
        return false;
    }

    if(query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}

bool DatabaseManager::insert(const QString &name, const QString &email, const QString &password) {

    if(userExists(email)) {
        lastError = "User with that email already exists!";
        qDebug() << lastError;
        return false;
    }

    QSqlQuery query(db);

    query.prepare("INSERT INTO users (name, email, password) VALUES (:name, :email, :password)");
    query.bindValue(":name", name);
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if(!query.exec()) {
        lastError = "Failed to insert user: " + query.lastError().text();
        qDebug() << lastError;
        return false;
    }

    qDebug() << "User inserted successfully!";
    return true;
}

bool DatabaseManager::userEnter(const QString &email, const QString password) {

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM users WHERE email = :email OR password = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if(!query.exec()) {
        lastError = "Failed to authorised: " + query.lastError().text();
        qDebug() << lastError;
        return false;
    }

    if(query.next()) {
        int count = query.value(0).toInt();
        if (count > 0) {
            qDebug() << "User authonticated successfully!";
            return true;
        }
        return false;
    }

    lastError = "Invalid email or password";

    return false;
}

void DatabaseManager::disconnect() {
    if(db.isOpen()) {
        db.close();
    }
}

QString DatabaseManager::getLastError() const {
    return lastError;
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}
