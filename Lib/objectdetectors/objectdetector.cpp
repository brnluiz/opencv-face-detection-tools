#include <stdexcept>

#include "objectdetector.h"

ObjectDetector::ObjectDetector(const DetectorParams &params): params_(params) {
}

ObjectDetector::~ObjectDetector() {
}

Mat ObjectDetector::getFrame() {
    return frame_;
}
