#ifndef BEST_HOG_H
#define BEST_HOG_H

#include <opencv2/opencv.hpp>
#include <iostream>

#include "stats.h"
#include "averaged_stats.h"

using namespace std;
using namespace cv;
using namespace cv::ml;

struct BestHog {
    BestHog(): descriptor(HOGDescriptor()), stat(AveragedStats()) {
        ;
    }

    HOGDescriptor descriptor;
    AveragedStats stat;

    friend std::ostream& operator<<(std::ostream& os, BestHog& best) {
        os << "Best HOG info" << endl

           << "Accuracy, " << best.stat.getAccuracy() << endl
           << "Precision, " << best.stat.getPrecision() << endl
           << "Recall, " << best.stat.getRecall() << endl
           << "FScore, " << best.stat.getFScore() << endl

           << "Block size, " << best.descriptor.blockSize << endl
           << "Cell size, " << best.descriptor.cellSize << endl
           << "Block stride, " << best.descriptor.blockStride << endl
           << "Win size, " << best.descriptor.winSize << endl

           << endl;

        return os;
    }
};

#endif // BEST_HOG_H
