#-------------------------------------------------
#
# Project created by QtCreator 2016-12-24T18:11:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testandroid
TEMPLATE = app


SOURCES += main.cpp\
    Stone.cpp \
    Board.cpp \
    Singlegame.cpp \
    Step.cpp \
    form.cpp \
    hashtable.cpp \
    historyrecord.cpp \
    addblind.cpp

HEADERS  += \
    Stone.h \
    Board.h \
    Singlegame.h \
    Step.h \
    form.h \
    hashtable.h \
    historyrecord.h \
    addblind.h

FORMS    += \
    form.ui

CONFIG += mobility
MOBILITY = 

