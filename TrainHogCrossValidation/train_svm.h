#ifndef SVMTRAIN_H
#define SVMTRAIN_H

#include "stats.h"
#include "kfold.h"

#include "train_abstract.h"

class SvmTrain: public AbstractTrain {
public:
    SvmTrain(vector<Mat>& pos, vector<Mat>& neg, const int& folds, HOGDescriptor& hog);
    SvmTrain(vector<Mat>& pos, vector<Mat>& neg, const int& folds, const HogParam& param);

    void run();

    Ptr<SVM> getBest();

protected:
    HOGDescriptor hog_;

    Ptr<SVM> bestsvm_;
};

#endif // SVMTRAIN_H
