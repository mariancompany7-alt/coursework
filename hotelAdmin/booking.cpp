#include "booking.h"

Booking::Booking() : id(0), userId(0), roomId(0), status("підтверджено"), totalPrice(0) {}

Booking::Booking(int id, int userId, int roomId, const QString& checkIn, const QString& checkOut,
                 const QString& status, int totalPrice)
    : id(id), userId(userId), roomId(roomId), checkIn(checkIn), checkOut(checkOut),
    status(status), totalPrice(totalPrice) {}
