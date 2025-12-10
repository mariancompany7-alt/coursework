#include "roomrepository.h"
#include "QSqlQuery"
#include <QDebug>

RoomRepository::RoomRepository(DatabaseManager* dbManager) : dbManager(dbManager) {}

bool RoomRepository::addRoom(const Room& room) {

    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("INSERT INTO rooms (roomNumber, type, price, status)"
                  "VALUES (:roomNumber, :type, :price, :status)");
    query.bindValue(":roomNumber", room.getRoomNumber());
    query.bindValue(":type", room.getType());
    query.bindValue(":price", room.getPrice());
    query.bindValue(":status", room.getStatus());

    return query.exec();
}

bool RoomRepository::updateRoom(const Room& room) {

    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("UPDATE rooms SET roomNumber = :roomNumber, type = :type, "
                  "price = :price, status = :status WHERE id = :id");
    query.bindValue(":roomNumber", room.getRoomNumber());
    query.bindValue(":type", room.getType());
    query.bindValue(":price", room.getPrice());
    query.bindValue(":status", room.getStatus());
    query.bindValue(":id", room.getId());

    return query.exec();
}

bool RoomRepository::deleteRoom(int roomId) {

    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("DELETE FROM rooms WHERE id = :id");
    query.bindValue(":id", roomId);

    return query.exec();
}

QList<Room> RoomRepository::getAllRooms() {

    QList<Room> rooms;
    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("SELECT id, roomNumber, type, price, status FROM rooms");
    query.exec();

    while (query.next()) {
        rooms.append(Room(
            query.value("id").toInt(),
            query.value("roomNumber").toInt(),
            query.value("type").toString(),
            query.value("price").toInt(),
            query.value("status").toString()
            ));
    }

    return rooms;
}

Room* RoomRepository::getRoomById(int roomId) {

    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("SELECT id, roomNumber, type, price, status FROM rooms WHERE id = :id");
    query.bindValue(":id", roomId);
    query.exec();

    if (query.next()) {
        return new Room(
            query.value("id").toInt(),
            query.value("roomNumber").toInt(),
            query.value("type").toString(),
            query.value("price").toInt(),
            query.value("status").toString()
            );
    }

    return nullptr;
}

QList<Room> RoomRepository::getAvailableRooms() {

    QList<Room> rooms;
    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("SELECT id, roomNumber, type, price, status FROM rooms WHERE status = 'Вільний'");
    query.exec();

    while (query.next()) {
        rooms.append(Room(
            query.value("id").toInt(),
            query.value("roomNumber").toInt(),
            query.value("type").toString(),
            query.value("price").toInt(),
            query.value("status").toString()
            ));
    }

    return rooms;
}
