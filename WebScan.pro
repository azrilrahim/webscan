#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T11:15:41
#
#-------------------------------------------------

QT       += core gui webkit webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebScan
TEMPLATE = app


SOURCES += main.cpp\
        webdialog.cpp \
    proxyserver.cpp \
    proxyclient.cpp \
    proxyclientthread.cpp

HEADERS  += webdialog.h \
    proxyserver.h \
    proxyclient.h \
    proxyclientthread.h

FORMS    += webdialog.ui
