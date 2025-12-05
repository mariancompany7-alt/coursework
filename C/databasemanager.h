#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlTableModel>

class DatabaseManager {
public:

    // 2. Статичний метод для отримання єдиного екземпляра
    static DatabaseManager* getInstance();

    QSqlDatabase& getDB(); // Метод для повернення об'єкта DB
    // ... методи для відкриття/закриття DB ..

    bool openDataBase();
    bool createTables();
    void closeDataBase();

private:

    QSqlDatabase db;
    static DatabaseManager* instance;
    // 1. Приватний конструктор для заборони створення ззовні
    DatabaseManager();
    //    QSqlTableModel* tableModel;
    bool userExists(const QString& login); // Допоміжний метод
};

#endif
