#-------------------------------------------------
#
# Project created by QtCreator 2017-03-13T17:07:24
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = net
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../debug_lib/ -lDoppler_net
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug_lib/ -lDoppler_netd
else:unix: LIBS += -L$$PWD/../debug_lib/ -lDoppler_net

INCLUDEPATH += $$PWD/../debug_lib/include
DEPENDPATH += $$PWD/../debug_lib/include
