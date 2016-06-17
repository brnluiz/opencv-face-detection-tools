#ifndef SETTINGS_H
#define SETTINGS_H

#define WINDOW_NAME "Face Detection Sandbox"

#define MODEL_CASCADE "/usr/local/share/OpenCV/lbpcascades/lbpcascade_frontalface.xml"
//#define MODEL_CASCADE "/usr/local/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml"
//#define MODEL_CASCADE "/usr/local/share/OpenCV/lbpcascades/lbpcascade_profileface.xml"

#define MODEL_HOGSVM_PATH "/home/brunoluiz/qt/build-TrainHog-Desktop_Qt_5_6_1_GCC_64bit-Debug/"
//#define MODEL_HOGSVM "model_faces94.lst_block8_stride4_cell4_wsize48x64.yml" // working, but get false-positives and miss some
//#define MODEL_HOGSVM "model_grimace_faces94.lst_block8_stride4_cell4_wsize48x64.yml" // working, but get some false-positives
//#define MODEL_HOGSVM "model_grimace_faces94.lst_block16_stride8_cell8_wsize48x64.yml" // working, but get some false-positives

#define MODEL_HOGSVM "model_faces94.lst_block16_stride8_cell8_wsize48x64.yml" // working
//#define MODEL_HOGSVM "model_faces94.lst_block32_stride8_cell8_wsize48x64.yml" // working
//#define MODEL_HOGSVM "model_grimace.lst_block16_stride8_cell8_wsize48x64.yml" // working but don't detect everything
//#define MODEL_HOGSVM "model_faces94.lst_block16_stride8_cell8_wsize32x48.yml" // working, but detect useless features

// not working #define MODEL_HOGSVM "model_grimace.lst_block16_stride8_cell8_wsize32x48.yml"
#define MODEL_HOGSVM_WINSIZE_W 48
#define MODEL_HOGSVM_WINSIZE_H 64
#define MODEL_HOGSVM_BLOCKSIZE 16
#define MODEL_HOGSVM_BLOCKSTRIDE 8
#define MODEL_HOGSVM_CELLSIZE 8

#endif // SETTINGS_H
