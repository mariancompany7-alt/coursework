QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminpanel.cpp \
    booking.cpp \
    bookingmanagement.cpp \
    bookingrepository.cpp \
    databasemanager.cpp \
    guestbookingdialog.cpp \
    guestpanel.cpp \
    guestroomdialog.cpp \
    loginform.cpp \
    main.cpp \
    mainwindow.cpp \
    managerpanel.cpp \
    room.cpp \
    roommanagement.cpp \
    roomrepository.cpp \
    user.cpp \
    usermanagement.cpp \
    userrepository.cpp

HEADERS += \
    adminpanel.h \
    booking.h \
    bookingmanagement.h \
    bookingrepository.h \
    databasemanager.h \
    guestbookingdialog.h \
    guestpanel.h \
    guestroomdialog.h \
    loginform.h \
    mainwindow.h \
    managerpanel.h \
    room.h \
    roommanagement.h \
    roomrepository.h \
    user.h \
    usermanagement.h \
    userrepository.h \

FORMS += \
    adminpanel.ui \
    bookingmanagement.ui \
    guestbookingdialog.ui \
    guestpanel.ui \
    guestroomdialog.ui \
    loginform.ui \
    mainwindow.ui \
    managerpanel.ui \
    roommanagement.ui \
    usermanagement.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
