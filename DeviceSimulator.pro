QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    logic.cpp \
    logiwrite.cpp \
    main.cpp \
    mainfunctions.cpp \
    mainwindow.cpp \
    sendingcommands.cpp \
    serial_thread.cpp \
    upwidg.cpp

HEADERS += \
    logic.h \
    logiwrite.h \
    mainfunctions.h \
    mainwindow.h \
    sendingcommands.h \
    serial_thread.h \
    upwidg.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
