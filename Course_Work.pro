QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    data_structures/list/passengers/passengeritem.cpp \
    data_structures/list/passengers/passengerlist.cpp \
    data_structures/list/tickets/ticketitem.cpp \
    data_structures/list/tickets/ticketlist.cpp \
    data_structures/search/boyermoore.cpp \
    dialogs/flight/addflightdialog.cpp \
    dialogs/flight/clearflightsdialog.cpp \
    dialogs/flight/deleteflightdialog.cpp \
    dialogs/passenger/addpassengerdialog.cpp \
    dialogs/passenger/clearpassengersdialog.cpp \
    dialogs/passenger/deletepassengerdialog.cpp \
    dialogs/ticket/addticketdialog.cpp \
    dialogs/ticket/deleteticketdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    data_structures/hash/hashtable.cpp \
    data_structures/tree/node.cpp \
    data_structures/tree/binarytree.cpp \
    sql_connecter/sqlconnecter.cpp

HEADERS += \
    data_structures/list/passengers/passengeritem.h \
    data_structures/list/passengers/passengerlist.h \
    data_structures/list/tickets/ticketitem.h \
    data_structures/list/tickets/ticketlist.h \
    data_structures/search/boyermoore.h \
    dialogs/flight/addflightdialog.h \
    dialogs/flight/clearflightsdialog.h \
    dialogs/flight/deleteflightdialog.h \
    dialogs/passenger/addpassengerdialog.h \
    dialogs/passenger/clearpassengersdialog.h \
    dialogs/passenger/deletepassengerdialog.h \
    dialogs/ticket/addticketdialog.h \
    dialogs/ticket/deleteticketdialog.h \
    mainwindow.h \
    data_structures/hash/hashtable.h \
    data_structures/tree/node.h \
    data_structures/tree/binarytree.h \
    sql_connecter/sqlconnecter.h

FORMS += \
    dialogs/flight/addflightdialog.ui \
    dialogs/flight/clearflightsdialog.ui \
    dialogs/flight/deleteflightdialog.ui \
    dialogs/passenger/addpassengerdialog.ui \
    dialogs/passenger/clearpassengersdialog.ui \
    dialogs/passenger/deletepassengerdialog.ui \
    dialogs/ticket/addticketdialog.ui \
    dialogs/ticket/deleteticketdialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
