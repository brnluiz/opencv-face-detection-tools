#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

#include "objectdetectors/cascadedetector.h"

CascadeDetector::CascadeDetector(string window): ObjectDetector(window) {
    reset();
}

CascadeDetector::CascadeDetector(string window, string model, float *params): ObjectDetector(window, model, params) {
    reset();
}

void CascadeDetector::reset() {
    if( !classifier_.load(model_) ) {
        // TODO: DO SOMETHING
    };
}

vector<Rect> CascadeDetector::detect(Mat &frame) {
    Mat frame_gray;

    frame_ = frame;

    // Pre-process the image
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    // Detect faces
    classifier_.detectMultiScale( frame_gray, objects_, 1.1, 2, 0, Size(80, 80) );

    // Return the list of detected faces (will be a rectangle vector)
    return objects_;

}
