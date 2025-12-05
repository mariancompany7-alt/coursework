#ifndef BOOKING_H
#define BOOKING_H

#include <QString>

class Booking {
public:

    Booking();
    Booking(int id, int userId, int roomId, const QString& checkIn, const QString& checkOut,
            const QString& status, int totalPrice);

    // Геттери
    int getId() const { return id; }
    int getUserId() const { return userId; }
    int getRoomId() const { return roomId; }
    QString getCheckIn() const { return checkIn; }
    QString getCheckOut() const { return checkOut; }
    QString getStatus() const { return status; }
    int getTotalPrice() const { return totalPrice; }

    // Сеттери
    void setId(int id) { this->id = id; }
    void setUserId(int userId) { this->userId = userId; }
    void setRoomId(int roomId) { this->roomId = roomId; }
    void setCheckIn(const QString& checkIn) { this->checkIn = checkIn; }
    void setCheckOut(const QString& checkOut) { this->checkOut = checkOut; }
    void setStatus(const QString& status) { this->status = status; }
    void setTotalPrice(int totalPrice) { this->totalPrice = totalPrice; }

private:

    int id;
    int userId;
    int roomId;
    QString checkIn;
    QString checkOut;
    QString status;
    int totalPrice;
};

#endif // BOOKING_H
