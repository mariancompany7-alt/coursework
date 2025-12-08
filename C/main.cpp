#include <QApplication>
#include "mainwindow.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager* db = DatabaseManager::getInstance();
    if (!db->openDataBase()) {
        qDebug() << "Помилка відкриття БД";
    } else {
        qDebug() << "БД відкрита успішно!";
    }

    MainWindow w;

    return a.exec();
}
