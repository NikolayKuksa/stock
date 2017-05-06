#-------------------------------------------------
#
# Project created by QtCreator 2017-04-16T15:25:02
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = stock
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    secsymboldescription.cpp \
    findatacollector.cpp \
    plotbymodel.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    secsymboldescription.h \
    findatacollector.h \
    securitytype.h \
    plotbymodel.h \
    qcustomplot.h
