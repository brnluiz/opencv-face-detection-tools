#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <algorithm>

#include "objectdetectors/hogsvmdetector.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

using namespace cv;
using namespace cv::ml;
using namespace std;

HogSvmDetector::HogSvmDetector(string window): ObjectDetector(window) {
}

HogSvmDetector::HogSvmDetector(DetectorParams params): ObjectDetector(params) {
    setParams(params);
    reset();
}

HogSvmDetector::HogSvmDetector(string window, DetectorParams params): ObjectDetector(window, params) {
    setParams(params);
    reset();
}

void HogSvmDetector::setParams(DetectorParams params) {
    int width = (int)params["windowwidth"];
    int height = (int)params["windowheight"];
    int blockSize = (int)params["blocksize"];
    int blockStride = (int)params["blockstride"];
    int cellSize = (int)params["cellsize"];

    // Setup up the HOG classifier
    hog_.winSize = Size(width, height);
    hog_.blockSize = Size(blockSize, blockSize);
    hog_.blockStride = Size(blockStride, blockStride);
    hog_.cellSize = Size(cellSize, cellSize);

    // Optional parameters
    winStride_      = Size();
    winScale_       = 1.05;
    hitThreshold_   = 0;
    groupFactor_    = 2;
    overlapNms_     = 0.3;
    if(!params["winstride"].empty()) {
        int stride      = (float)params["winstride"];
        winStride_      = Size(stride, stride);
    }

    if(!params["winpadding"].empty()) {
        int padding     = (int)params["winpadding"];
        winPadding_     = Size(padding, padding);
    }

    if(!params["winscale"].empty())
        winScale_       = (float)params["winscale"];

    if(!params["hitthreshold"].empty())
        hitThreshold_   = (float)params["hitthreshold"];

    if(!params["groupfactor"].empty())
        groupFactor_    = (int)params["groupfactor"];

    if(!params["overlapnms"].empty())
        overlapNms_     = (float)params["overlapnms"];

    // Get the hog-svm model file
    model_ = (string)params["model"];
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
    hog_.detectMultiScale(frame_, objects_, hitThreshold_,
                          winStride_, winPadding_, winScale_, groupFactor_);

    // Allocate memory for non-maximum supression (NMS)
    Point* tlPoints = (Point*)malloc(objects_.size() * sizeof(Point));
    Point* tlPointsOut = 0;
    Point* brPoints = (Point*)malloc(objects_.size() * sizeof(Point));
    Point* brPointsOut = 0;
    float* score = (float*)malloc(objects_.size() * sizeof(float));
    float* scoreOut = 0;
    int boxesOut = 0;

    // Get the data for the NMS process
    for(u_int i = 0; i < objects_.size(); i++) {
        tlPoints[i] = objects_[i].tl();
        brPoints[i] = objects_[i].br();
        score[i]    = 1;
    }

    // Execute the NMS
    nonMaximumSuppression(objects_.size(), tlPoints,
                            brPoints, score,
                            0.4,
                            &boxesOut, &tlPointsOut,
                            &brPointsOut, &scoreOut);

    // Save the new objects (after NMS) to the objects_ vector
    objects_.clear();
    for(int i = 0; i < boxesOut; i++) {
        Rect bb = Rect(tlPointsOut[i], brPointsOut[i]);
        objects_.push_back(bb);
    }

    // Free memory
    free(tlPoints);
    free(tlPointsOut);
    free(brPoints);
    free(brPointsOut);
    free(score);
    free(scoreOut);

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

// Code got from LatentSVM OpenCV
int HogSvmDetector::nonMaximumSuppression(int numBoxes, Point *points,
                          Point *oppositePoints, float *score,
                          float overlapThreshold,
                          int *numBoxesOut, Point **pointsOut,
                          Point **oppositePointsOut, float **scoreOut) {
    int i, j, index;
    float* box_area = (float*)malloc(numBoxes * sizeof(float));
    int* indices = (int*)malloc(numBoxes * sizeof(int));
    int* is_suppressed = (int*)malloc(numBoxes * sizeof(int));

    for (i = 0; i < numBoxes; i++) {
        indices[i] = i;
        is_suppressed[i] = 0;
        box_area[i] = (float)( (oppositePoints[i].x - points[i].x + 1) *
                                (oppositePoints[i].y - points[i].y + 1));
    }

    sort(numBoxes, score, indices);
    for (i = 0; i < numBoxes; i++) {
        if (!is_suppressed[indices[i]]) {
            for (j = i + 1; j < numBoxes; j++) {
                if (!is_suppressed[indices[j]]) {
                    int x1max = max(points[indices[i]].x, points[indices[j]].x);
                    int x2min = min(oppositePoints[indices[i]].x, oppositePoints[indices[j]].x);
                    int y1max = max(points[indices[i]].y, points[indices[j]].y);
                    int y2min = min(oppositePoints[indices[i]].y, oppositePoints[indices[j]].y);
                    int overlapWidth = x2min - x1max + 1;
                    int overlapHeight = y2min - y1max + 1;
                    if (overlapWidth > 0 && overlapHeight > 0)
                    {
                        float overlapPart = (overlapWidth * overlapHeight) / box_area[indices[j]];
                        if (overlapPart > overlapThreshold)
                        {
                            is_suppressed[indices[j]] = 1;
                        }
                    }
                }
            }
        }
    }

    *numBoxesOut = 0;
    for (i = 0; i < numBoxes; i++) {
        if (!is_suppressed[i]) (*numBoxesOut)++;
    }

    *pointsOut = (Point *)malloc((*numBoxesOut) * sizeof(Point));
    *oppositePointsOut = (Point *)malloc((*numBoxesOut) * sizeof(Point));
    *scoreOut = (float *)malloc((*numBoxesOut) * sizeof(float));
    index = 0;
    for (i = 0; i < numBoxes; i++) {
        if (!is_suppressed[indices[i]]) {
            (*pointsOut)[index].x = points[indices[i]].x;
            (*pointsOut)[index].y = points[indices[i]].y;
            (*oppositePointsOut)[index].x = oppositePoints[indices[i]].x;
            (*oppositePointsOut)[index].y = oppositePoints[indices[i]].y;
            (*scoreOut)[index] = score[indices[i]];
            index++;
        }

    }

    free(indices);
    free(box_area);
    free(is_suppressed);

    return 1;
}


void HogSvmDetector::sort(int n, const float* x, int* indices) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = i + 1; j < n; j++) {
            if (x[indices[j]] > x[indices[i]]) {
                //float x_tmp = x[i];
                int index_tmp = indices[i];
                //x[i] = x[j];
                indices[i] = indices[j];
                //x[j] = x_tmp;
                indices[j] = index_tmp;
            }
        }
    }
}
