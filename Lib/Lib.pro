#-------------------------------------------------
#
# Project created by QtCreator 2016-06-18T11:35:55
#
#-------------------------------------------------

QT       -= gui

TARGET = facedetection
TEMPLATE = lib

DEFINES += LIB_LIBRARY

INCLUDEPATH += /usr/local/include/opencv

LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core

SOURCES += \
    factories/sourcehandlerfactory.cpp \
    factories/objectdetectorfactory.cpp \
    sourcehandlers/imagehandler.cpp \
    sourcehandlers/camhandler.cpp \
    objectdetectors/cascadedetector.cpp \
    objectdetectors/hogsvmdetector.cpp \
    objectdetectors/objectdetector.cpp \

HEADERS += \
    factories/sourcehandlerfactory.h \
    factories/objectdetectorfactory.h \
    sourcehandlers/sourcehandler.h \
    sourcehandlers/imagehandler.h \
    sourcehandlers/camhandler.h \
    objectdetectors/objectdetector.h \
    objectdetectors/hogsvmdetector.h \
    objectdetectors/cascadedetector.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
