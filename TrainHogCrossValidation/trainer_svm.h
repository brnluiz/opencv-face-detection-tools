#ifndef SVMTRAIN_H
#define SVMTRAIN_H

#include "performance_test.h"
#include "kfold.h"

#include "trainer_abstract.h"
#include "best_svm.h"

class TrainerSvm: public TrainerAbstract {
public:
    TrainerSvm(vector<Mat>& pos, vector<Mat>& neg, const int& folds, HOGDescriptor& hog);
    TrainerSvm(vector<Mat>& pos, vector<Mat>& neg, const int& folds, const HogParam& param);

    void run();
    void saveReport(const string & output_path);

    BestSvm getBest();

private:
    HOGDescriptor hog_;
    BestSvm best_;
};

#endif // SVMTRAIN_H
