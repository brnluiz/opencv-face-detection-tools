#ifndef HAARFACEDETECTOR_H
#define HAARFACEDETECTOR_H

#include <opencv2/core/core.hpp>
#include <vector>
#include <opencv2/objdetect.hpp>

#include "objectdetectors/objectdetector.h"

using namespace std;
using namespace cv;

class HaarFaceDetector: public ObjectDetector{
public:
    HaarFaceDetector(string window);

    vector<Rect> detect(Mat& frame);

private:
    CascadeClassifier classifier_;
};

#endif // HAARFACEDETECTOR_H
