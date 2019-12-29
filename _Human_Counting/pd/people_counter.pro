TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt



INCLUDEPATH += ..

SOURCES += main.cpp \
    people_count_test.cpp \
    tracker_base.cpp \
    opencv_trackers.cpp \
    blob_detector_base.cpp \
    bg_subtract_detector.cpp\
    contour_utility.cpp

HEADERS += \
    people_count_test.hpp \
    tracker_base.hpp \
    opencv_trackers.hpp \
    blob_detector_base.hpp \
    bg_subtract_detector.hpp\
    contour_utility.hpp

linux-g++ {
    LIBS += -lopencv_video -lopencv_videoio
    LIBS +=  -lopencv_objdetect -lopencv_bgsegm -lopencv_tracking
    LIBS +=  `pkg-config opencv --libs`
}
