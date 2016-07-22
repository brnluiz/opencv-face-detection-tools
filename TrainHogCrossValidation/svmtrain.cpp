#include "svmtrain.h"
#include "log.h"

SvmTrain::SvmTrain(vector<Mat> &pos, vector<Mat> &neg, HOGDescriptor &hog): GeneralTrain(pos, neg), hog_(hog) {
}

void SvmTrain::run() {
    Trainer trainer;

    // Allocate the SVM parameters (labels and HOG)
    SVMTRAIN_LOG << "Preparing SVM parameters" << endl;
    vector<int> labels;
    vector<Mat> gradient_lst;
    trainer.computeHogList(pos_, gradient_lst, labels, hog_, +1);
    trainer.computeHogList(neg_, gradient_lst, labels, hog_, -1);

    // Train a SVM using the actual HOGs
    SVMTRAIN_LOG << "Training SVM and choosing the best parameters" << endl;
    bestsvm_ = SVM::create();
    trainer.trainSvm(gradient_lst, labels, bestsvm_, true);
}

Ptr<SVM> SvmTrain::getBest() {
    return bestsvm_;
}
