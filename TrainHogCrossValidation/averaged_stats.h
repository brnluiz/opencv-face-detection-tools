#ifndef AVERAGEDSTATS_H
#define AVERAGEDSTATS_H

#include "stats.h"
#include <vector>

using namespace std;

struct AverßagedStats {
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
};

#endif // AVERAGEDSTATS_H
