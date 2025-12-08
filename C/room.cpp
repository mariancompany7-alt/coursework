#include "room.h"

Room::Room() : id(0), roomNumber(0), price(0), status("Вільний") {}

Room::Room(int id, int roomNumber, const QString& type, int price, const QString& status)
    : id(id), roomNumber(roomNumber), type(type), price(price), status(status) {}
