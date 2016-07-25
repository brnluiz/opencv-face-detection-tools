#include "train_svm.h"

#include "log.h"
#include "utils_hog.h"
#include "utils_svm.h"

SvmTrain::SvmTrain(vector<Mat> &pos, vector<Mat> &neg, const int& folds, HOGDescriptor &hog):
    AbstractTrain(pos, neg, folds), hog_(hog) {
}

SvmTrain::SvmTrain(vector<Mat> &pos, vector<Mat> &neg, const int& folds, const HogParam &param): AbstractTrain(pos, neg, folds) {
    hog_ = makeDescriptor(param);
}

void SvmTrain::run() {
    // Allocate the SVM parameters (labels and HOG)
    SVMTRAIN_LOG << "Preparing SVM parameters" << endl;
    vector<int> labels;
    vector<Mat> gradient_lst;
    HogUtils::computeList(pos_, gradient_lst, labels, hog_, +1);
    HogUtils::computeList(neg_, gradient_lst, labels, hog_, -1);

    // Train a SVM using the actual HOGs
    SVMTRAIN_LOG << "Training SVM and choosing the best parameters" << endl;
    bestsvm_ = SvmUtils::train(gradient_lst, labels, true);
}

Ptr<SVM> SvmTrain::getBest() {
    return bestsvm_;
}
