#-------------------------------------------------
#
# Project created by QtCreator 2017-11-29T20:53:25
#
#-------------------------------------------------

QT       += core gui network
CONFIG   += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = daily-assistant
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    task.cpp \
    work.cpp \
    notes.cpp \
    binaryserializer.cpp \
    tasks.cpp \
    options.cpp \
    taskedit.cpp \
    serversync.cpp

HEADERS += \
        mainwindow.h \
    task.h \
    work.h \
    notes.h \
    serializable.h \
    binaryserializer.h \
    tasks.h \
    options.h \
    taskedit.h \
    serversync.h

FORMS += \
        mainwindow.ui \
    task.ui \
    work.ui \
    notes.ui \
    tasks.ui \
    options.ui \
    taskedit.ui

RESOURCES += \
    resources.qrc
