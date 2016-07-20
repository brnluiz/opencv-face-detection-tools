QT += core
QT -= gui

CONFIG += c++11

TARGET = TrainHogCrossValidation
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += $$PWD/../Lib
DEPENDPATH += $$PWD/../Lib

LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
LIBS += -L$$OUT_PWD/../Lib/ -lfacedetection

SOURCES += \
    dataset.cpp \
    negdataset.cpp \
    kfold.cpp \
    hogtrain.cpp \
    stats.cpp \
    trainer.cpp \
    svmtrain.cpp \
    train_main.cpp \
    generaltrain.cpp

HEADERS += \
    dataset.h \
    negdataset.h \
    kfold.h \
    hogtrain.h \
    stats.h \
    trainer.h \
    svmtrain.h \
    generaltrain.h \
    log.h
