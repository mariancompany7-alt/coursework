#include "databasemanager.h"
#include <QDebug>
#include <QFile>

DatabaseManager::DatabaseManager() {}

bool DatabaseManager::connect() {

    // QFile::remove("./HotelAdministrator.db");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./HotelAdministrator.db");

    if(db.open()) {
        qDebug("Database opened successfully!");
        return true;
    } else {
        qDebug("Failed to open database: ");
        return false;
    }

}

bool DatabaseManager::createTables() {

    QSqlQuery query(db);

    if(query.exec("CREATE TABLE RegisterBook(name TEXT, email TEXT, password TEXT)")) {
        qDebug("Tables RegisterBook successfully created!");
        return true;
    } else {
        lastError = "Failed to create table: " + query.lastError().text();
        qDebug() << lastError;
        return false;
    }

}

bool DatabaseManager::insert(const QString &name, const QString &email, const QString password) {

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (name, email, password) VALUES (:name, :email, :password)");
    query.bindValue(":name", name);
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if(query.exec()) {
        qDebug("User inserted successfully!");
        return true;
    } else {
        lastError = "Failed to insert user: " + query.lastError().text();
        qDebug() << lastError;
        return false;
    }
}

void DatabaseManager::disconnect() {

    if(db.isOpen()) {
        db.close();
    }
}

DatabaseManager::~DatabaseManager() {}
