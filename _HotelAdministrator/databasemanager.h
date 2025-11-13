#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DatabaseManager
{
public:
    DatabaseManager();
    bool connect();
    bool createTables();
    bool insert(const QString &name, const QString &email, const QString password);
    void disconnect();
    ~DatabaseManager();

private:
    QSqlDatabase db;
    QString lastError;
};

#endif // DATABASEMANAGER_H
