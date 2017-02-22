#-------------------------------------------------
#
# Project created by QtCreator 2017-01-19T18:47:41
#
#-------------------------------------------------

QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = OpenGl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    oglwidget.cpp \
    cameraview.cpp

HEADERS  += mainwindow.h \
    oglwidget.h \
    cameraview.h

FORMS    += mainwindow.ui

LIBS += -lopengl32 -lglu32



