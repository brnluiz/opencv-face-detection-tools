#include "trainer_svm.h"

#include "log.h"
#include "utils_hog.h"
#include "utils_svm.h"

TrainerSvm::TrainerSvm(vector<Mat> &pos, vector<Mat> &neg, const int& folds, HOGDescriptor &hog):
    TrainerAbstract(pos, neg, folds), hog_(hog) {
}

TrainerSvm::TrainerSvm(vector<Mat> &pos, vector<Mat> &neg, const int& folds, const HogParam &param): TrainerAbstract(pos, neg, folds) {
    hog_ = makeDescriptor(param);
}

void TrainerSvm::run() {
    // Allocate the SVM parameters (labels and HOG)
    TRAINERSVM_LOG << "Preparing SVM parameters" << endl;
    vector<int> labels;
    vector<Mat> gradient_lst;
    HogUtils::computeList(pos_, gradient_lst, labels, hog_, +1);
    HogUtils::computeList(neg_, gradient_lst, labels, hog_, -1);

    // Train a SVM using the actual HOGs
    TRAINERSVM_LOG << "Training SVM and choosing the best parameters" << endl;
    bestsvm_ = SvmUtils::train(gradient_lst, labels, false);
}

Ptr<SVM> TrainerSvm::getBest() {
    return bestsvm_;
}
