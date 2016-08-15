#ifndef AVERAGEDSTATS_H
#define AVERAGEDSTATS_H

#include "stats.h"
#include <vector>

using namespace std;

struct AveragedStats {
public:
    AveragedStats();

    float getAccuracy();

    float getPrecision();

    float getRecall();

    float getFScore();

    // Data related methods
    void insert(Stats item);

    void remove(int pos);

    void clear();

    vector<Stats> items_;

    friend std::ostream& operator<<(std::ostream& os, AveragedStats& avg) {
        os << "Folds, " << avg.items_.size() << endl
           << "Accuracy, " << avg.getAccuracy() << endl
           << "Precision, " << avg.getPrecision() << endl
           << "Recall, " << avg.getRecall() << endl
           << "FScore, " << avg.getFScore() << endl;

        return os;
    }
};

#endif // AVERAGEDSTATS_H
