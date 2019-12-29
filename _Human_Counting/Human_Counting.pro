#-------------------------------------------------
#
# Project created by QtCreator 2019-05-08T09:35:12
#
#-------------------------------------------------

QT       += core gui sql charts
#QMAKE_LFLAGS += -fuse-ld=gold
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Human_Counting
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#########################OPTIMIZATION OPTIONS ################################
#QMAKE_CXXFLAGS += -Os -mpreferred-stack-boundary=2 -finline-small-functions -momit-leaf-frame-pointer

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3
#################################################
DEFINES += OPENCV_USE_CUDA

QMAKE_CXXFLAGS_RELEASE -= -03

QMAKE_CXXFLAGS+= -std=c++17

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    #trackooi.cpp \
    mythread.cpp \
    mylabel.cpp\
    #treehandle.cpp\
   treebutton.cpp\
    ./pd/people_count_test.cpp \
   ./pd/tracker_base.cpp \
   ./pd/opencv_trackers.cpp \
   ./pd/blob_detector_base.cpp \
   ./pd/bg_subtract_detector.cpp\
   ./pd/contour_utility.cpp \
    cameras.cpp \
    line.cpp


HEADERS += \
    mainwindow.h \
    detectpeople.h \
   # trackooi.h \
    mythread.h \
     #dbhandler.h \
    mylabel.h \
    utils.h \
    #dbhandler.h \
    hogdescriptor.h\
    #treehandle.h\
    treebutton.h\
    ./pd/people_count_test.hpp \
    ./pd/tracker_base.hpp \
    ./pd/opencv_trackers.hpp \
    ./pd/blob_detector_base.hpp \
    ./pd/bg_subtract_detector.hpp\
    ./pd/contour_utility.hpp \
    handling_db.h \
    cameras.h \
    mobilessd.h \
    line.h


FORMS += \
        mainwindow.ui




#include(./Resources)
INCLUDEPATH += ./Resources

INCLUDEPATH+=/usr/include/opencv
QMAKE_CXXFLAGS += -Wall

LIBS += `pkg-config opencv --libs` -lopencv_dnn

#LIBS+= -03

CONFIG += link_pkgconfig
PKGCONFIG += opencv #sqlite3
#############################DLIB CONFIG##########################
INCLUDEPATH+= /home/centurine/PACKAGES/dlib-19.17/include
LIBS+=-L/home/centurine/PACKAGES/dlib-19.17/build
LIBS+=-ldlib -lopenblas
#################cuda and blass#######################
LIBS +=-lopenblas
LIBS +=-lcudnn
LIBS +=-L"/usr/local/cuda/lib64"
LIBS +=-lcublas
LIBS +=-lcudart
LIBS +=-lcurand
LIBS +=-lcusolver
##########################################################opencv cuda#################
#LIBS += `pkg-config --cflags --libs opencv`
#INCLUDEPATH+ =/usr/local/include
#LIBS +=-L/home/centurine/PACKAGES/opencv-4/opencv-4.0.0/build/lib
#LIBS += -lopencv_tracking
LIBS +=-L/usr/local/lib -lopencv_tracking

RESOURCES += \
    resources.qrc
