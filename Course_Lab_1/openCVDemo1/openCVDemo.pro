TARGET = openCVDemo1
TEMPLATE = app

SOURCES += main.cpp

#INCLUDEPATH += /usr/local/include
INCLUDEPATH += D:\Software\opencv\build\include \
            D:\Software\opencv\build\include\opencv \
            D:\Software\opencv\build\include\opencv2

#Linux中的配置，用pkg-config --libs --cflags opencv查看
LIBS += D:\Software\opencv\build\x64\vc14\lib\opencv_world343d.lib
