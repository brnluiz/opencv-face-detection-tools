#ifndef STATS_H
#define STATS_H

#include <cstdint>

class Stats {
public:
    uint32_t positives;
    uint32_t negatives;
    uint32_t false_positives;
    uint32_t false_negatives;

    // Stats methods
    float getAccuracy();
    float getRecall();
    float getPrecision();

    // Printing methods
    void print();
    friend std::ostream& operator<<(std::ostream& os, Stats& stat) {
        os << "Accuracy info" << endl;
        os << "Positives, " << stat.positives << endl;
        os << "Negatives, " << stat.negatives << endl;
        os << "False positives, " << stat.false_positives << endl;
        os << "False negatives, " << stat.false_negatives << endl;
        os << "General Accuracy, " << stat.getAccuracy() << endl;

        return os;
    }

};

#endif // STATS_H
