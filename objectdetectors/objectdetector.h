#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

class ObjectDetector {
public:
    ObjectDetector(string window) { windowName_ = window; }
    virtual vector<Rect> detect(Mat& frame) = 0;

    virtual void show() {
        for (size_t i = 0; i < objects_.size(); i++) {
            rectangle(frame_, objects_[i], Scalar(0, 255, 0), 3);
        }
        imshow(windowName_, frame_);
    }

protected:
    string windowName_;
    Mat frame_;
    std::vector<Rect> objects_;
};

#endif // OBJECTDETECTOR_H
