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
    secsymboldescription.cpp \
    findatacollector.cpp \
    plotbymodel.cpp \
    qcustomplot.cpp \
    miscimpl.cpp \
    fincalculator.cpp \
    portfoliooptimisationwindow.cpp \
    recalgcalc.cpp \
    importviewdatawindow.cpp \
    mainwindow.cpp \
    recalgwindow.cpp

HEADERS  += \
    secsymboldescription.h \
    findatacollector.h \
    securitytype.h \
    plotbymodel.h \
    qcustomplot.h \
    miscdef.h \
    fincalculator.h \
    portfoliooptimisationwindow.h \
    recalgcalc.h \
    importviewdatawindow.h \
    mainwindow.h \
    recalgwindow.h
