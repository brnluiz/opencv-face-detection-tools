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
    ObjectDetector(string window);
    ObjectDetector(string window, string model, float *params);

    void setModel(string model);
    void setWindow(string window);
    virtual void show();

    virtual vector<Rect> detect(Mat& frame) = 0;
    virtual void reset() = 0;

protected:
    virtual void setParams(float *params) = 0;

    string windowName_;
    string model_;

    Mat frame_;
    std::vector<Rect> objects_;
};

#endif // OBJECTDETECTOR_H
