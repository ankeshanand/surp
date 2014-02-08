#-------------------------------------------------
#
# Project created by QtCreator 2013-03-20T12:55:25
#
#-------------------------------------------------


QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mesonero
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cateringservices.cpp \
    reservation.cpp \
    checkin.cpp \
    checkout.cpp \
    management.cpp \
    room.cpp \
    databasemanager.cpp

HEADERS  += mainwindow.h \
    cateringservices.h \
    reservation.h \
    checkin.h \
    checkout.h \
    management.h \
    room.h \
    databasemanager.h

FORMS    += mainwindow.ui
