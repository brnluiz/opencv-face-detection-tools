#ifndef PERFORMANCE_TEST_H
#define PERFORMANCE_TEST_H

#include <opencv2/opencv.hpp>
#include <cstdint>
#include <vector>

#include "trainer_abstract.h"
#include "stats.h"


using namespace std;
using namespace cv;

class PerformanceTest {
public:
    PerformanceTest();
    PerformanceTest(const HOGDescriptor& hog);
    PerformanceTest(const HOGDescriptor& hog, const vector<Mat>& pos, const vector<Mat>& neg);
    PerformanceTest(const HOGDescriptor& hog, const vector<SampleInfo>& pos, const vector<SampleInfo>& neg);

    // Test methods
    void test(const vector<SampleInfo>& samples);
    void test(const vector<Mat>& pos, const vector<Mat>& neg);
    void test();
    void testPositive(const Mat& test);
    void testNegative(const Mat& test);
    void testPositives(const vector<Mat>& test_set);
    void testNegatives(const vector<Mat>& test_set);
    void testPositives();
    void testNegatives();

    Stats getStats();

protected:
    HOGDescriptor hog_;
    vector<Mat> pos_;
    vector<Mat> neg_;
    Stats stat_;
};

#endif // PERFORMANCE_TEST_H
