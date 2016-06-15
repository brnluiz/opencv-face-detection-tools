#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

#include "settings.h"
#include "objectdetectors/haarfacedetector.h"

HaarFaceDetector::HaarFaceDetector(string window): ObjectDetector(window) {
    if( !classifier_.load(HAARFACEDETECTOR_XML) ) {
        // TODO: DO SOMETHING
    };
}

vector<Rect> HaarFaceDetector::detect(Mat &frame) {
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
