#include "svmtrain.h"
#include "log.h"

SvmTrain::SvmTrain(vector<Mat> &pos, vector<Mat> &neg, HOGDescriptor &hog): GeneralTrain(pos, neg), hog_(hog) {
}

void SvmTrain::run() {
    Trainer trainer;

    // Allocate the SVM parameters (labels)
    SVMTRAIN_LOG << "Preparing SVM parameters" << endl;
    vector<int> labels;
    labels.assign( pos_.size(), +1 );
    const unsigned int old = (unsigned int)labels.size();
    labels.insert( labels.end(), neg_.size(), -1 );
    CV_Assert( old < labels.size() );

    // Compute HOG
    SVMTRAIN_LOG << "Computing HOG" << endl;
    vector<Mat> pos_gradients = trainer.computeHog(pos_, hog_);
    vector<Mat> neg_gradients = trainer.computeHog(neg_, hog_);

    // Insert the computed HOG on the gradient_lst vector
    vector<Mat> gradient_lst;
    gradient_lst.insert(gradient_lst.end(), pos_gradients.begin(), pos_gradients.end());
    gradient_lst.insert(gradient_lst.end(), neg_gradients.begin(), neg_gradients.end());

    // Train a SVM using the actual HOGs
    SVMTRAIN_LOG << "Training SVM and choosing the best parameters" << endl;
    bestsvm_ = SVM::create();
    trainer.trainSvm(gradient_lst, labels, bestsvm_, true);
}

Ptr<SVM> SvmTrain::getBest() {
    return bestsvm_;
}
