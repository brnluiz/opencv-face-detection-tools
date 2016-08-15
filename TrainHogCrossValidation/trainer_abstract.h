#ifndef GENERALTRAIN_H
#define GENERALTRAIN_H

#include <opencv2/opencv.hpp>
#include <vector>

#include "kfold.h"
#include "log.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

typedef vector<int> HogParam;
typedef vector<HogParam> HogParamList;

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


class TrainerAbstract {
public:
    TrainerAbstract(vector<Mat>& pos, vector<Mat>& neg, int folds = 10);

    virtual void run() = 0;
    virtual void saveReport(const string &path) = 0;

    void setFolds(int k);

protected:
    vector<Mat> pos_;
    vector<Mat> neg_;
    int folds_;

    HOGDescriptor makeDescriptor(const HogParam &params);

    void computeMultipleHog(const Mat &img, const int &type, const HOGDescriptor &hog, vector<SampleInfo>& samples);

    Kfold<vector<SampleInfo>::const_iterator> prepareSamples(vector<Mat> &pos, vector<Mat> &neg, const HOGDescriptor& hog);

    vector<SampleInfo> prepareSamples(const vector<Mat> &set, const HOGDescriptor& hog, const int& type);

    void prepareSvmParameters(vector<Mat>& gradient_lst, vector<int>& labels, const vector<SampleInfo>& pos, const vector<SampleInfo>& neg);
};

#endif // GENERALTRAIN_H
