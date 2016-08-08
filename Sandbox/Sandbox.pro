QT += core
QT -= gui

TARGET = faces_sandbox

TEMPLATE = app
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle



INCLUDEPATH += $$PWD/../Lib
DEPENDPATH += $$PWD/../Lib

unix {
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
LIBS += -L$$OUT_PWD/../Lib/ -lfacedetection
}
macx {
INCLUDEPATH += /usr/local/Cellar/opencv3/3.1.0_3/include
LIBS += -L/usr/local/Cellar/opencv3/3.1.0_3/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
LIBS += -L$$OUT_PWD/../Lib/ -lfacedetection
}

HEADERS += \
    sourcehandlers/sourcehandlerfactory.h \
    sourcehandlers/sourcehandler.h \
    sourcehandlers/imagehandler.h \
    sourcehandlers/camhandler.h \
    objectdetectors/objectdetectorfactory.h \
    objectdetectors/objectdetector.h \
    objectdetectors/hogsvmdetector.h \
    objectdetectors/cascadedetector.h \
    measurementtools/measuredistancetool.h \
    gui/viewer.h

SOURCES += \
    sandbox.cpp

