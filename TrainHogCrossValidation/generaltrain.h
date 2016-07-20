#ifndef GENERALTRAIN_H
#define GENERALTRAIN_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "trainer.h"
#include "kfold.h"
#include "log.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

typedef vector<int> HogParam;
typedef vector<HogParam> HogParamList;

struct HogBest {
    HOGDescriptor descriptor;
    float acc;

    void print() {
        HOGTRAIN_LOG << " ! Best HOG info !"
                     << " * Accuracy: " << acc
                     << " * Block size: " << descriptor.blockSize
                     << " * Cell size: " << descriptor.cellSize
                     << " * Block stride: " << descriptor.blockStride
                     << " * Win size: " << descriptor.winSize
                     << endl;
    }
};

enum HogParamTypes {
    BLOCK_SIZE,
    CELL_SIZE,
    BLOCK_STRIDE,
    WIN_SIZE
};

struct SampleInfo {
    Mat image;
    Mat hog;
    int type;
};


class GeneralTrain {
public:
    GeneralTrain(vector<Mat>& pos, vector<Mat>& neg, int folds = 10);

    virtual void run() = 0;

    void setFolds(int k);

protected:
    vector<Mat> pos_;
    vector<Mat> neg_;
    int folds_;

    HOGDescriptor makeDescriptor(HogParam& params);

    void computeMultipleHog(Mat img, int type, const HOGDescriptor &hog, vector<SampleInfo>& samples);

    Kfold<vector<SampleInfo>::const_iterator> prepareSamples(vector<Mat> &pos, vector<Mat> &neg, const HOGDescriptor& hog);
};

#endif // GENERALTRAIN_H
