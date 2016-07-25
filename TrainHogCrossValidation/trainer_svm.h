#ifndef SVMTRAIN_H
#define SVMTRAIN_H

#include "stats.h"
#include "kfold.h"

#include "trainer_abstract.h"

class TrainerSvm: public TrainerAbstract {
public:
    TrainerSvm(vector<Mat>& pos, vector<Mat>& neg, const int& folds, HOGDescriptor& hog);
    TrainerSvm(vector<Mat>& pos, vector<Mat>& neg, const int& folds, const HogParam& param);

    void run();

    Ptr<SVM> getBest();

protected:
    HOGDescriptor hog_;

    Ptr<SVM> bestsvm_;
};

#endif // SVMTRAIN_H
