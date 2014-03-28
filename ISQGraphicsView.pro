#-------------------------------------------------
#
# Project created by QtCreator 2014-02-17T14:03:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ISQGraphicsView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newgraphicsview.cpp \
    FibonacciHeap.cpp

HEADERS  += mainwindow.h \
    newgraphicsview.h \
    FibonacciHeap.h \
    asmOpenCV.h

FORMS    += mainwindow.ui

CONFIG += link_pkgconfig
PKGCONFIG += opencv

OTHER_FILES += \
    RoadMaps.txt \
    README.txt

RESOURCES += \
    icon.qrc
