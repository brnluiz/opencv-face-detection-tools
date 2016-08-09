#ifndef SVMTRAIN_H
#define SVMTRAIN_H

#include "stats.h"
#include "kfold.h"

#include "trainer_abstract.h"

struct SvmBest {
    SvmBest(): acc(0) {
        ;
    }

    float acc;
    Ptr<SVM> svm;

//    void print() {

//    }

    friend std::ostream& operator<<(std::ostream& os, SvmBest& sb) {
        os << "Best SVM info" << endl
           << "Accuracy, " << sb.acc << endl
           << "C, " << sb.svm->getC() << endl
           << "P, " << sb.svm->getP() << endl
           << "Nu, " << sb.svm->getNu() << endl
           << "Degree, " << sb.svm->getDegree() << endl
           << "Coef0, " << sb.svm->getCoef0() << endl
           << endl;

        return os;
    }

};

class TrainerSvm: public TrainerAbstract {
public:
    TrainerSvm(vector<Mat>& pos, vector<Mat>& neg, const int& folds, HOGDescriptor& hog);
    TrainerSvm(vector<Mat>& pos, vector<Mat>& neg, const int& folds, const HogParam& param);

    void run();

    Ptr<SVM> getBest();

private:
    HOGDescriptor hog_;
    SvmBest bestsvm_;
};

#endif // SVMTRAIN_H
