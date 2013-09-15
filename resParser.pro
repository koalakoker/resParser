#-------------------------------------------------
#
# Project created by QtCreator 2013-09-09T18:10:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = resParser
TEMPLATE = app

unix:LIBS+= -L/usr/lib -lgmp
unix:LIBS+= -L/usr/lib -lmpfr

SOURCES += main.cpp\
        mainwindow.cpp \
    resistor.cpp \
    variable.cpp \
    parserclass.cpp \
    commandmngrclass.cpp \
    hystorylineedit.cpp \
    hfloat.cpp \
    builtinfunction.cpp

HEADERS  += mainwindow.h \
    resistor.h \
    variable.h \
    parserclass.h \
    commandmngrclass.h \
    hystorylineedit.h \
    hfloat.h \
    builtinfunction.h

FORMS    += mainwindow.ui

RC_FILE = resParser.rc

OTHER_FILES +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ -llibgmp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ -llibgmpd

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libgmp.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libgmpd.lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ -lmpfr
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ -lmpfrd

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/mpfr.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/mpfrd.lib
