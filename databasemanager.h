#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool connect();
    bool createTables();
    bool insert(const QString &name, const QString &email, const QString &password);
    bool userExists(const QString &email);
    bool userEnter(const QString &email, const QString password);
    void disconnect();
    QString getLastError() const;

private:
    QSqlDatabase db;
    QString lastError;
};

#endif
