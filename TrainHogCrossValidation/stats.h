#ifndef STATS_H
#define STATS_H

#include <opencv2/opencv.hpp>
#include <cstdint>

#include "generaltrain.h"

using namespace std;
using namespace cv;

struct Stats {
public:
    Stats(): positives(0), negatives(0), false_positives(0), false_negatives(0) {
        ;
    }

    uint32_t positives;
    uint32_t negatives;
    uint32_t false_positives;
    uint32_t false_negatives;

    float get_accuracy() {
        return (float)(positives + negatives) /
                (float)(positives + negatives + false_positives + false_negatives);
    }

    void test(const vector<SampleInfo>& samples, const HOGDescriptor& hog) {
        for (uint i = 0; i < samples.size(); i++) {
            if (samples[i].type == +1) testPositive(samples[i].image, hog);
            else if (samples[i].type == -1) testNegative(samples[i].image, hog);
        }
    }

    void testPositive(const Mat& test, const HOGDescriptor& hog) {
        vector<Rect> detections;
        hog.detectMultiScale(test, detections);

        if (detections.size() != 0) {
            positives++;
        } else {
            false_negatives++;
        }
    }

    void testNegative(const Mat& test, const HOGDescriptor& hog) {
        vector<Rect> detections;
        hog.detectMultiScale(test, detections);

        if (detections.size() != 0) {
            false_positives++;
        } else {
            negatives++;
        }
    }

    void testPositives(const vector<Mat>& test_set, const HOGDescriptor& hog) {
        for (uint i = 0; i < test_set.size(); i++) {
            testPositive(test_set[i], hog);
        }
    }

    void testNegatives(const vector<Mat>& test_set, const HOGDescriptor& hog) {
        for (uint i = 0; i < test_set.size(); i++) {
            testNegative(test_set[i], hog);
        }
    }

};

#endif // STATS_H
