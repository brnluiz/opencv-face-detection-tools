#ifndef STATS_H
#define STATS_H

#include <opencv2/opencv.hpp>
#include <cstdint>

#include "train_abstract.h"

using namespace std;
using namespace cv;

class Stats {
public:
    Stats();

    Stats(const HOGDescriptor& hog);

    Stats(const HOGDescriptor& hog, const vector<Mat>& pos, const vector<Mat>& neg);

    Stats(const HOGDescriptor& hog, const vector<SampleInfo>& pos, const vector<SampleInfo>& neg);

    float get_accuracy();

    void test(const vector<SampleInfo>& samples);

    void test(const vector<Mat>& pos, const vector<Mat>& neg);

    void test();

    void testPositive(const Mat& test);

    void testNegative(const Mat& test);

    void testPositives(const vector<Mat>& test_set);

    void testNegatives(const vector<Mat>& test_set);

    void testPositives();

    void testNegatives();


protected:
    uint32_t positives;
    uint32_t negatives;
    uint32_t false_positives;
    uint32_t false_negatives;

    HOGDescriptor hog_;

    vector<Mat> pos_;
    vector<Mat> neg_;
};

#endif // STATS_H
