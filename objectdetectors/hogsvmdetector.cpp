#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>

#include "settings.h"
#include "objectdetectors/hogsvmdetector.h"

using namespace cv;
using namespace cv::ml;
using namespace std;

HogSvmDetector::HogSvmDetector(string window): ObjectDetector(window) {
}

HogSvmDetector::HogSvmDetector(string window, string model, float *params): ObjectDetector(window, model, params) {
    setParams(params);
    reset();
}

void HogSvmDetector::setParams(float *params) {
    int width = (int)params[0];
    int height = (int)params[1];
    int blockSize = (int)params[2];
    int blockStride = (int)params[3];
    int cellSize = (int)params[4];

    hog_.winSize = Size(width, height);
    hog_.blockSize = Size(blockSize, blockSize);
    hog_.blockStride = Size(blockStride, blockStride);
    hog_.cellSize = Size(cellSize, cellSize);
}

void HogSvmDetector::reset() {
    // Load the trained model
    svm_ = StatModel::load<SVM>(model_);

    // Set the trained svm to hog_
    vector<float> detector = getDetector();
    hog_.setSVMDetector(detector);
}

vector<Rect> HogSvmDetector::detect(Mat &frame) {
    frame_ = frame;

    // Detect faces
    hog_.detectMultiScale(frame_, objects_);

    // Return the list of detected faces (will be a rectangle vector)
    return objects_;
}

vector<float> HogSvmDetector::getDetector() {
    vector<float> detector;

    // get the support vectors
    Mat sv = svm_->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm_->getDecisionFunction(0, alpha, svidx);

    CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
    CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
               (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
    CV_Assert( sv.type() == CV_32F );

    detector.resize(sv.cols + 1);
    memcpy(&detector[0], sv.ptr(), sv.cols*sizeof(detector[0]));
    detector[sv.cols] = (float)-rho;

    return detector;
}
