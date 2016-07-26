#ifndef STATS_H
#define STATS_H

#include <opencv2/opencv.hpp>
#include <cstdint>

#include "trainer_abstract.h"

using namespace std;
using namespace cv;

class Stats {
public:
    Stats();

    Stats(const HOGDescriptor& hog);

    Stats(const HOGDescriptor& hog, const vector<Mat>& pos, const vector<Mat>& neg);

    Stats(const HOGDescriptor& hog, const vector<SampleInfo>& pos, const vector<SampleInfo>& neg);

    float getAccuracy();

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

    void test(const vector<SampleInfo>& samples);

    void test(const vector<Mat>& pos, const vector<Mat>& neg);

    void test();

    void testPositive(const Mat& test);

    void testNegative(const Mat& test);

    void testPositives(const vector<Mat>& test_set);

    void testNegatives(const vector<Mat>& test_set);

    void testPositives();

    void testNegatives();

    uint32_t positives;
    uint32_t negatives;
    uint32_t false_positives;
    uint32_t false_negatives;

protected:

    HOGDescriptor hog_;

    vector<Mat> pos_;
    vector<Mat> neg_;
};

#endif // STATS_H
