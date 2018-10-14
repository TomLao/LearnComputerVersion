#-------------------------------------------------
#
# Project created by QtCreator 2018-10-12T21:23:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openCVDemo2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        qcvwidget.cpp \
    opencvworker.cpp

HEADERS += \
        qcvwidget.h \
    opencvworker.h

FORMS += \
        qcvwidget.ui

INCLUDEPATH += D:\Software\opencv\build\include \
            D:\Software\opencv\build\include\opencv \
            D:\Software\opencv\build\include\opencv2

#使用pkg-config --libs --cflags opencv，将返回的值贴到下面LIBS
#https://www.jianshu.com/p/16c70bff776f
#LIBS += /usr/lib/i386-linux-gnu/libopencv*
LIBS += D:\Software\opencv\build\x64\vc14\lib\opencv_world343d.lib
