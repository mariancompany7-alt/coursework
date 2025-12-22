#ifndef BOOKINGREPOSITORY_H
#define BOOKINGREPOSITORY_H

#include "Booking.h"
#include "databasemanager.h"
#include <QList>

class BookingRepository {
public:

    explicit BookingRepository(DatabaseManager *dbManager);

    bool addBooking(const Booking& booking);
    bool updateBooking(const Booking& booking);
    bool deleteBooking(int bookingId);
    bool isRoomBookedInPeriod(int roomId, const QString& checkIn, const QString& checkOut, int excludeBookingId = 0);

    QList<Booking> getAllBookings();
    QList<Booking> getBookingsByUser(int userId);
    Booking* getBookingById(int bookingId);
    QString getBookedDatesForRoom(int roomId);

private:
    DatabaseManager* dbManager;
};

#endif // BOOKINGREPOSITORY_H
