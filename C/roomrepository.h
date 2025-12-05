#ifndef ROOMREPOSITORY_H
#define ROOMREPOSITORY_H

#include "room.h"
#include "databasemanager.h"
#include <QList>

class RoomRepository
{
public:
    explicit RoomRepository(DatabaseManager* dbManager);

    bool addRoom(const Room& room);
    bool updateRoom(const Room& room);
    bool deleteRoom(int roomId);
    QList<Room> getAllRooms();
    Room* getRoomById(int roomId);
    QList<Room> getAvailableRooms();

private:

    DatabaseManager* dbManager;
};

#endif // ROOMREPOSITORY_H
