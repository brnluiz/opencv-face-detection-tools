#ifndef CASCADEDETECTOR_H
#define CASCADEDETECTOR_H

#include <opencv2/core/core.hpp>
#include <vector>
#include <opencv2/objdetect.hpp>

#include "objectdetectors/objectdetector.h"

using namespace std;
using namespace cv;

class CascadeDetector: public ObjectDetector{
public:
    CascadeDetector(string window);

    vector<Rect> detect(Mat& frame);

private:
    CascadeClassifier classifier_;
};

#endif // CASCADEDETECTOR_H
