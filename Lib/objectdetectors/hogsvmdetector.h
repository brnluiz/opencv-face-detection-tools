#ifndef HOGSVMDETECTOR_H
#define HOGSVMDETECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <vector>

#include "objectdetectors/objectdetector.h"

using namespace cv;
using namespace std;
using namespace cv::ml;

class HogSvmDetector: public ObjectDetector {
public:
    HogSvmDetector(string window);
    HogSvmDetector(DetectorParams params);
    HogSvmDetector(string window, DetectorParams params);

    Objects detect(Mat& frame);
    void reset();
protected:
    void setParams(DetectorParams params);
private:
    vector<float> getDetector();
    int nonMaximumSuppression(int numBoxes, Point *points,
                      Point *oppositePoints, float *score,
                      float overlapThreshold,
                      int *numBoxesOut, Point **pointsOut,
                      Point **oppositePointsOut, float **scoreOut);
    void sort(int n, const float* x, int* indices);

    HOGDescriptor hog_;

    // Parameters for tests
    Size winStride_;
    Size winPadding_;
    float winScale_;
    float hitThreshold_;
    int groupFactor_;
    float overlapNms_;

    Ptr<SVM> svm_;
};

#endif // HOGSVMDETECTOR_H
