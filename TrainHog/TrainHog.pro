QT += core
QT -= gui

CONFIG += c++11

TARGET = TrainHog
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

unix {
INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
}
macx {
INCLUDEPATH += /usr/local/Cellar/opencv3/3.1.0_3/include
LIBS += -L/usr/local/Cellar/opencv3/3.1.0_3/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
}

SOURCES += \
    trainhog.cpp
