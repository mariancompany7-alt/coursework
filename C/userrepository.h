#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "user.h"
#include "databasemanager.h"
#include <QString>
#include <QList>

class UserRepository {

public:
    explicit UserRepository(DatabaseManager* dbManager);

    bool addUser(const User& user);
    bool deleteUser(int id);
    bool updateUser(const User& user);

    // Пошук
    User* findById(int id);
    User* findByLogin(const QString& login);
    QList<User> getAllUsers();

    // ВИПРАВЛЕНО: Узгодження сигнатури з .cpp файлом (використовуємо 'login' замість 'email')
    QString login(const QString& login, const QString& password);

private:
    DatabaseManager* dbManager;
};

#endif // USERREPOSITORY_H
