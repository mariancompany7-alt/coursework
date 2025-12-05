#include "userrepository.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

UserRepository::UserRepository(DatabaseManager* dbManager)
    : dbManager(dbManager) {}

bool UserRepository::addUser(const User& user) {

    QSqlQuery query(dbManager->getDB());
    query.prepare("INSERT INTO users (login, password_hash, salt, role) "
                  "VALUES (:login, :password_hash, :salt, :role)");
    query.bindValue(":login", user.getLogin());
    query.bindValue(":password_hash", user.getPasswordHash());
    query.bindValue(":salt", user.getSalt());
    query.bindValue(":role", user.getRole());

    if (!query.exec()) {
        qDebug() << "UserRepository::addUser error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool UserRepository::deleteUser(int id) {

    QSqlQuery query(dbManager->getDB());
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qCritical() << "UserRepository::deleteUser error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool UserRepository::updateUser(const User& user) {

    QSqlQuery query(dbManager->getDB());
    query.prepare("UPDATE users SET login = :login, password_hash = :password_hash, "
                  "salt = :salt, role = :role WHERE id = :id");
    query.bindValue(":id", user.getId());
    query.bindValue(":login", user.getLogin());
    query.bindValue(":password_hash", user.getPasswordHash());
    query.bindValue(":salt", user.getSalt());
    query.bindValue(":role", user.getRole());

    if (!query.exec()) {
        qCritical() << "UserRepository::updateUser error:" << query.lastError().text();
        return false;
    }
    return true;
}

User* UserRepository::findById(int id) {

    QSqlQuery query(dbManager->getDB());
    query.prepare("SELECT id, login, password_hash, salt, role FROM users WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    //  ВИПРАВЛЕННЯ 1: Потрібно перейти до першого запису!
    if (!query.next())
        return nullptr;

    return new User(
        //  ВИПРАВЛЕННЯ 2: Прибрано ":" з усіх значень
        query.value("id").toInt(),
        query.value("login").toString(),
        query.value("password_hash").toString(),
        query.value("salt").toString(),
        query.value("role").toString());
}

QList<User> UserRepository::getAllUsers() {

    QList<User> users;
    QSqlQuery query(dbManager->getDB());
    query.prepare("SELECT id, login, password_hash, salt, role FROM users ORDER BY id");
    query.exec();

    while(query.next()) {
        users.append(User( query.value("id").toInt(),
                          query.value("login").toString(),
                          query.value("password_hash").toString(),
                          query.value("salt").toString(),
                          query.value("role").toString()));
    }
    return users;
}

User* UserRepository::findByLogin(const QString& login) {

    QSqlQuery query(dbManager->getDB());
    query.prepare("SELECT id, login, password_hash, salt, role FROM users WHERE login = :login");
    query.bindValue(":login", login);
    query.exec();

    if (!query.next())
        return nullptr;

    return new User(
        // ВИПРАВЛЕННЯ: Прибрано ":" з усіх значень
        query.value("id").toInt(),
        query.value("login").toString(),
        query.value("password_hash").toString(),
        query.value("salt").toString(),
        query.value("role").toString());
}

QString UserRepository::login(const QString& login, const QString& password) {

    User* user = findByLogin(login);
    if(!user) {
        return ""; // Користувача не знайдено
    }

    // Використовуємо метод з класу User для перевірки
    if(user->verifyPassword(password)) {
        QString role = user->getRole();
        delete user; // Важливо звільнити пам'ять!
        return role;
    }

    delete user; // Пароль невірний, але пам'ять звільнити треба
    return "";
}
