#-------------------------------------------------
#
# Project created by QtCreator 2016-03-21T10:29:02
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyFTP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dirscan.cpp \
    qftp.cpp \
    qurlinfo.cpp

HEADERS  += mainwindow.h \
    dirscan.h \
    qftp.h \
    qurlinfo.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
