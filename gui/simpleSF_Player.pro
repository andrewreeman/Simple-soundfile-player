#-------------------------------------------------
#
# Project created by QtCreator 2013-12-07T13:19:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simpleSF_Player
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../src/SoundFile.cpp \
    ../src/ioUtils.cpp \
    ../src/AudioIO.cpp

HEADERS  += mainwindow.h \
    ../include/SoundFile.hh \
    ../include/portaudio.h \
    ../include/ioUtils.hh \
    ../include/exceptions.hh \
    ../include/AudioIO.hh \
    ../include/sndfile.h \
    ../include/sndfile.hh

FORMS    += mainwindow.ui

unix: LIBS += -L$$PWD/../libs/ -lportaudio -lsndfile -lrt -lasound -ljack -lpthread

INCLUDEPATH += $$PWD/../libs
DEPENDPATH += $$PWD/../libs

unix: PRE_TARGETDEPS += $$PWD/../libs/libportaudio.a


win32: LIBS += -L$$PWD/../libs/ -llibsndfile-1

INCLUDEPATH += $$PWD/../libs
DEPENDPATH += $$PWD/../libs

win32: PRE_TARGETDEPS += $$PWD/../libs/libsndfile-1.lib


win32: LIBS += -L$$PWD/../libs/ -lportaudio_x86

INCLUDEPATH += $$PWD/../libs
DEPENDPATH += $$PWD/../libs

win32: PRE_TARGETDEPS += $$PWD/../libs/portaudio_x86.lib
