#include "objectdetectors/objectdetector.h"

ObjectDetector::ObjectDetector(string window) {
    setWindow(window);
}

ObjectDetector::ObjectDetector(DetectorParams params): params_(params) {
}

ObjectDetector::ObjectDetector(string window, DetectorParams params): params_(params) {
    setWindow(window);
}

void ObjectDetector::show() {
    if (window_.empty()) {
        cerr << "You need to setup a window: setWindow(string)" << endl;
        return ;
    }

    for (size_t i = 0; i < objects_.size(); i++) {
        rectangle(frame_, objects_[i], Scalar(0, 255, 0), 3);
    }
    imshow(window_, frame_);
}

void ObjectDetector::setWindow(string window) {
    window_ = window;
    namedWindow( window, WINDOW_AUTOSIZE );
}

Mat ObjectDetector::getFrame() {
    return frame_;
}
