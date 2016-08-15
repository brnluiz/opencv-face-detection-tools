#ifndef STATS_H
#define STATS_H

#include <cstdint>
#include <iostream>

using namespace std;

class Stats {
public:
    Stats(): positives(0), negatives(0), false_positives(0), false_negatives(0) {

    }

    uint32_t positives;
    uint32_t negatives;
    uint32_t false_positives;
    uint32_t false_negatives;

    // Stats methods
    float getAccuracy();
    float getRecall();
    float getPrecision();
    float getFScore();

    // Printing methods
    void print();
    friend std::ostream& operator<<(std::ostream& os, Stats& stat) {
        os << "Positives, " << stat.positives << endl
           << "Negatives, " << stat.negatives << endl
           << "False positives, " << stat.false_positives << endl
           << "False negatives, " << stat.false_negatives << endl
           << "Accuracy, " << stat.getAccuracy() << endl
           << "Precison, " << stat.getPrecision() << endl
           << "Recall, " << stat.getRecall() << endl
           << "FScore, " << stat.getFScore() << endl;

        return os;
    }

};

#endif // STATS_H
