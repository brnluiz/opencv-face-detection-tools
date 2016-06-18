#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

typedef FileStorage DetectorParams;
typedef vector<Rect> Objects;
class ObjectDetector {
public:
    ObjectDetector(string window);
    ObjectDetector(DetectorParams params);
    ObjectDetector(string window, DetectorParams params);

    virtual void show();
    virtual void reset() = 0;
    virtual Objects detect(Mat& frame) = 0;
    virtual void setParams(DetectorParams params) = 0;

    void setWindow(string window);
    Mat getFrame();
protected:

    string window_;
    string model_;
    DetectorParams params_;

    Mat frame_;
    Objects objects_;
};

#endif // OBJECTDETECTOR_H
