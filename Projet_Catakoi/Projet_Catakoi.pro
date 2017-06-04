#-------------------------------------------------
#
# Project created by QtCreator 2017-04-05T14:27:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projet_Catakoi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myglwidget.cpp \
    game.cpp \
    chronometre.cpp

HEADERS  += mainwindow.h \
    myglwidget.h \
    game.h \
    chronometre.h

FORMS    += mainwindow.ui

INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include

LIBS += -lOpengl32 -lglu32

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core2413 \
    -lopencv_highgui2413 \
    -lopencv_imgproc2413 \
    -lopencv_features2d2413 \
    -lopencv_calib3d2413

RESOURCES += \
    ressources.qrc

