#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <stdexcept>

#include "objectdetectors/cascadedetector.h"

CascadeDetector::CascadeDetector(DetectorParams params): ObjectDetector(params) {
    setParams(params);
    reset();
}

void CascadeDetector::reset() {
    if( !classifier_.load(model_) ) {
        throw invalid_argument("Cannot load the model");
    };
}

void CascadeDetector::setParams(DetectorParams params) {
    if(!params.isOpened()) {
        throw invalid_argument("Parameters file not loaded");
    }

    // Get the cascade model file
    model_ = (string)params["model"];
}

vector<Rect> CascadeDetector::detect(Mat &frame) {
    frame_ = frame;

    Mat frame_gray;
    // Pre-process the image
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    // Detect faces
    classifier_.detectMultiScale( frame_gray, objects_, 1.1, 2, 0, Size(80, 80) );

    // Return the list of detected faces (will be a rectangle vector)
    return objects_;

}
