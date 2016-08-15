#ifndef BEST_SVM_H
#define BEST_SVM_H

#include <opencv2/opencv.hpp>
#include <iostream>

#include "stats.h"
#include "averaged_stats.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

struct BestSvm {
    BestSvm(): stat(AveragedStats()) {
        ;
    }

    Ptr<SVM> svm;
    AveragedStats stat;

    friend std::ostream& operator<<(std::ostream& os, BestSvm& best) {
        os << "Best SVM info" << endl

           << "Accuracy, " << best.stat.getAccuracy() << endl
           << "Precision, " << best.stat.getPrecision() << endl
           << "Recall, " << best.stat.getRecall() << endl
           << "FScore, " << best.stat.getFScore() << endl

           << "C, " << best.svm->getC() << endl
           << "P, " << best.svm->getP() << endl
           << "Nu, " << best.svm->getNu() << endl
           << "Degree, " << best.svm->getDegree() << endl
           << "Coef0, " << best.svm->getCoef0() << endl

           << endl;

        return os;
    }

};

#endif // BEST_SVM_H
