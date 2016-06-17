#include "objectdetectors/objectdetector.h"

ObjectDetector::ObjectDetector(string window) {
    setWindow(window);
}

ObjectDetector::ObjectDetector(string window, string model, float *params) {
    setModel(model);
    setWindow(window);
}

void ObjectDetector::show() {
    for (size_t i = 0; i < objects_.size(); i++) {
        rectangle(frame_, objects_[i], Scalar(0, 255, 0), 3);
    }
    imshow(windowName_, frame_);
}

void ObjectDetector::setModel(string model) {
    model_ = model;
}

void ObjectDetector::setWindow(string window) {
    windowName_ = window;
    namedWindow( window, WINDOW_AUTOSIZE );
}
