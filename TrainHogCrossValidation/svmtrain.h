#ifndef SVMTRAIN_H
#define SVMTRAIN_H

#include "stats.h"
#include "trainer.h"
#include "kfold.h"

#include "generaltrain.h"


class SvmTrain: public GeneralTrain {
public:
    SvmTrain(vector<Mat>& pos, vector<Mat>& neg, HOGDescriptor& hog);

    void run();

    Ptr<SVM> getBest();

protected:
    HOGDescriptor hog_;

    Ptr<SVM> bestsvm_;
};

#endif // SVMTRAIN_H
