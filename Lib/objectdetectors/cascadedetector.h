#ifndef CASCADEDETECTOR_H
#define CASCADEDETECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <vector>

#include "objectdetector.h"

using namespace std;
using namespace cv;

class CascadeDetector: public ObjectDetector{
public:
    CascadeDetector(const DetectorParams &params);

    vector<Rect> detect(const Mat& frame);
    void reset();

protected:
    void setParams(const DetectorParams &params);
private:
    CascadeClassifier classifier_;
};

#endif // CASCADEDETECTOR_H
