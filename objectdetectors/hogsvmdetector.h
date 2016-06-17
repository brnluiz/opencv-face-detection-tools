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

    vector<Rect> detect(Mat& frame);
private:
    vector<float> getDetector();

    HOGDescriptor hog_;
    Ptr<SVM> svm_;
};

#endif // HOGSVMDETECTOR_H
