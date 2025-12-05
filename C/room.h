#ifndef ROOM_H
#define ROOM_H

#include <QString>

class Room {
public:
    Room();
    Room(int id, int roomNumber, const QString& type, int price, const QString& status);

    // Геттери
    int getId() const { return id; }
    int getRoomNumber() const { return roomNumber; }
    QString getType() const { return type; }
    int getPrice() const { return price; }
    QString getStatus() const { return status; }

    // Сеттери
    void setId(int id) { this->id = id; }
    void setRoomNumber(int roomNumber) { this->roomNumber = roomNumber; }
    void setType(const QString& type) { this->type = type; }
    void setPrice(int price) { this->price = price; }
    void setStatus(const QString& status) { this->status = status; }

private:
    int id;
    int roomNumber;
    QString type;
    int price;
    QString status;
};

#endif // ROOM_H
#ifndef ROOM_H
#define ROOM_H

#include <QString>

class Room {
public:
    Room();
    Room(int id, int roomNumber, const QString& type, int price, const QString& status);

    // Геттери
    int getId() const { return id; }
    int getRoomNumber() const { return roomNumber; }
    QString getType() const { return type; }
    int getPrice() const { return price; }
    QString getStatus() const { return status; }

    // Сеттери
    void setId(int id) { this->id = id; }
    void setRoomNumber(int roomNumber) { this->roomNumber = roomNumber; }
    void setType(const QString& type) { this->type = type; }
    void setPrice(int price) { this->price = price; }
    void setStatus(const QString& status) { this->status = status; }

private:
    int id;
    int roomNumber;
    QString type;
    int price;
    QString status;
};

#endif // ROOM_H
