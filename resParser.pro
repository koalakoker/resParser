#-------------------------------------------------
#
# Project created by QtCreator 2013-09-09T18:10:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    resistor.cpp \
    variable.cpp \
    parserclass.cpp \
    commandmngrclass.cpp \
    hystorylineedit.cpp

HEADERS  += mainwindow.h \
    resistor.h \
    variable.h \
    parserclass.h \
    commandmngrclass.h \
    hystorylineedit.h

FORMS    += mainwindow.ui
