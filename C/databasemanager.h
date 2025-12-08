#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlTableModel>

class DatabaseManager {
public:

    static DatabaseManager* getInstance();

    QSqlDatabase& getDB();

    bool openDataBase();
    bool createTables();
    void closeDataBase();

private:

    QSqlDatabase db;
    static DatabaseManager* instance;
    DatabaseManager();
    bool userExists(const QString& login);
};

#endif
