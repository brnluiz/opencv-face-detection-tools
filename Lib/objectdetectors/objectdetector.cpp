#include <stdexcept>

#include "objectdetectors/objectdetector.h"

ObjectDetector::ObjectDetector(DetectorParams params): params_(params) {
}

ObjectDetector::~ObjectDetector() {
}

Mat ObjectDetector::getFrame() {
    return frame_;
}
