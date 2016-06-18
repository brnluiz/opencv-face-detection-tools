#ifndef CASCADEDETECTOR_H
#define CASCADEDETECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <vector>

#include "objectdetectors/objectdetector.h"

using namespace std;
using namespace cv;

class CascadeDetector: public ObjectDetector{
public:
    CascadeDetector(string window);
    CascadeDetector(DetectorParams params);
    CascadeDetector(string window, DetectorParams params);

    vector<Rect> detect(Mat& frame);
    void reset();

protected:
    void setParams(DetectorParams params);
private:
    CascadeClassifier classifier_;
};

#endif // CASCADEDETECTOR_H
