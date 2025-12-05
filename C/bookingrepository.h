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

    QList<Booking> getAllBookings();
    QList<Booking> getBookingsByUser(int userId);
    Booking* getBookingById(int bookingId);

private:
    DatabaseManager* dbManager;
};

#endif // BOOKINGREPOSITORY_H
