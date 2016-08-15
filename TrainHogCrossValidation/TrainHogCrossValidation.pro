QT += core
QT -= gui

CONFIG += c++11

TARGET = TrainHogCrossValidation
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

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

SOURCES += \
    dataset.cpp \
    kfold.cpp \
    gen_best_model.cpp \
    utils_svm.cpp \
    utils_hog.cpp \
    tester_ground.cpp \
    trainer_hog.cpp \
    trainer_svm.cpp \
    trainer_abstract.cpp \
    performance_test.cpp \
    stats.cpp

HEADERS += \
    dataset.h \
    kfold.h \
    log.h \
    utils_svm.h \
    utils_hog.h \
    tester_ground.h \
    trainer_svm.h \
    trainer_hog.h \
    trainer_abstract.h \
    performance_test.h \
    stats.h
