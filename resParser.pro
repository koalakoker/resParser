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
    builtinfunction.cpp \
    aboutdialog.cpp \
    userdefinedfunctions.cpp \
    commandclass.cpp \
    docklistview.cpp \
    tableinfoelement.cpp \
    drawwidget.cpp \
    drawwidgetbrowse.cpp \
    fpoint.cpp \
    datainspectorwidget.cpp \
    qrecthf.cpp \
    doublespinbox.cpp

HEADERS  += mainwindow.h \
    resistor.h \
    variable.h \
    parserclass.h \
    commandmngrclass.h \
    hystorylineedit.h \
    hfloat.h \
    builtinfunction.h \
    aboutdialog.h \
    userdefinedfunctions.h \
    commandclass.h \
    global.h \
    docklistview.h \
    tableinfoelement.h \
    drawwidget.h \
    drawwidgetbrowse.h \
    fpoint.h \
    datainspectorwidget.h \
    qrecthf.h \
    doublespinbox.h

FORMS    += mainwindow.ui \
    functionwindow.ui \
    aboutdialog.ui \
    docklistview.ui \
    drawwidgetbrowse.ui \
    datainspectorwidget.ui

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

RESOURCES += \
    AboutDialog.qrc
