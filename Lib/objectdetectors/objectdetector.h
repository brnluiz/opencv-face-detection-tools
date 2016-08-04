#ifndef OBJECTDETECTOR_H
#define OBJECTDETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

typedef FileStorage DetectorParams;
typedef vector<Rect> Objects;
class ObjectDetector {
public:
    ObjectDetector(const DetectorParams &params);
    virtual ~ObjectDetector();

    virtual void reset() = 0;
    virtual Objects detect(const Mat& frame) = 0;
    virtual void setParams(const DetectorParams &params) = 0;

    Mat getFrame();
protected:
    string model_;
    DetectorParams params_;

    Mat frame_;
    Objects objects_;
};

#endif // OBJECTDETECTOR_H
