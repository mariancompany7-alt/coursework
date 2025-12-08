#include "bookingRepository.h"
#include <QSqlQuery>
#include <QDebug>

BookingRepository::BookingRepository(DatabaseManager* dbManager) : dbManager(dbManager) {}

bool BookingRepository::addBooking(const Booking& booking) {

    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("INSERT INTO bookings (user_id, room_id, check_in, check_out, status, total_price)"
                  "VALUES (:userId, :roomId, :checkIn, :checkOut, :status, :totalPrice)");
    query.bindValue(":userId", booking.getUserId());
    query.bindValue(":roomId", booking.getRoomId());
    query.bindValue(":checkIn", booking.getCheckIn());
    query.bindValue(":checkOut", booking.getCheckOut());
    query.bindValue(":status", booking.getStatus());
    query.bindValue(":totalPrice", booking.getTotalPrice());

    return query.exec();
}

bool BookingRepository::updateBooking(const Booking& booking) {

    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("UPDATE bookings SET user_id = :userId, room_id = :roomId, check_in = :checkIn,"
                  " check_out = :checkOut, status = :status, total_price = :totalPrice "
                  "WHERE id = :id");
    query.bindValue(":userId", booking.getUserId());
    query.bindValue(":roomId", booking.getRoomId());
    query.bindValue(":checkIn", booking.getCheckIn());
    query.bindValue(":checkOut", booking.getCheckOut());
    query.bindValue(":status", booking.getStatus());
    query.bindValue(":totalPrice", booking.getTotalPrice());
    query.bindValue(":id", booking.getId());

    return query.exec();
}

bool BookingRepository::deleteBooking(int bookingId) {

    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("DELETE FROM bookings WHERE id = :id");
    query.bindValue(":id", bookingId);

    return query.exec();
}

QList <Booking> BookingRepository::getAllBookings() {

    QList<Booking> bookings;
    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("SELECT id, user_id, room_id, check_in, check_out, status, "
                  "total_price FROM bookings");
    query.exec();

    while(query.next()) {
        bookings.append(Booking(
            query.value("id").toInt(),
            query.value("user_id").toInt(),
            query.value("room_id").toInt(),
            query.value("check_in").toString(),
            query.value("check_out").toString(),
            query.value("status").toString(),
            query.value("total_price").toInt() ));
    }
    return bookings;
}

QList<Booking> BookingRepository::getBookingsByUser(int userId) {

    QList<Booking> bookings;
    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("SELECT id, user_id, room_id, check_in, check_out, status, "
                  "total_price FROM bookings WHERE user_id = :userId");
    query.bindValue(":userId", userId);
    query.exec();

    while(query.next()) {
        bookings.append(Booking(
            query.value("id").toInt(),
            query.value("user_id").toInt(),
            query.value("room_id").toInt(),
            query.value("check_in").toString(),
            query.value("check_out").toString(),
            query.value("status").toString(),
            query.value("total_price").toInt() ));
    }
    return bookings;
}

Booking* BookingRepository:: getBookingById(int bookingId) {

    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("SELECT id, user_id, room_id, check_in, check_out, status, "
                  "total_price FROM bookings WHERE id = :id");
    query.bindValue(":id", bookingId);
    query.exec();

    while(query.next()) {
        return new Booking( query.value("id").toInt(),
                           query.value("user_id").toInt(),
                           query.value("room_id").toInt(),
                           query.value("check_in").toString(),
                           query.value("check_out").toString(),
                           query.value("status").toString(),
                           query.value("total_price").toInt() );
    }
    return nullptr;
}

bool BookingRepository::isRoomBookedInPeriod(int roomId, const QString& checkIn, const QString& checkOut, int excludeBookingId) {

    QSqlQuery query(DatabaseManager::getInstance()->getDB());
    query.prepare("SELECT id FROM bookings WHERE room_id = :roomId AND"
                  "status != 'Скасовано' AND status != 'Виписано' AND id != :excludeId AND"
                  "(:checkIn < check_out) AND (:checkOut > check_in)");
    query.bindValue(":roomId", roomId);
    query.bindValue(":checkIn", checkIn);
    query.bindValue(":checkOut", checkOut);
    query.bindValue(":excludeId", excludeBookingId);
    query.exec();

    if(query.next()) {
        return true;
    }

    return false;
}
