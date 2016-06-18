QT += core
QT -= gui

CONFIG += c++11

TARGET = DetectorsSandbox
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    factories/sourcehandlerfactory.cpp \
    factories/objectdetectorfactory.cpp \
    sourcehandlers/imagehandler.cpp \
    sourcehandlers/camhandler.cpp \
    objectdetectors/cascadedetector.cpp \
    objectdetectors/hogsvmdetector.cpp \
    objectdetectors/objectdetector.cpp \

INCLUDEPATH += /usr/local/include/opencv

LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core

HEADERS += \
    factories/sourcehandlerfactory.h \
    factories/objectdetectorfactory.h \
    sourcehandlers/sourcehandler.h \
    sourcehandlers/imagehandler.h \
    sourcehandlers/camhandler.h \
    objectdetectors/objectdetector.h \
    objectdetectors/hogsvmdetector.h \
    objectdetectors/cascadedetector.h \
