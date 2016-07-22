#ifndef STATS_H
#define STATS_H

#include <opencv2/opencv.hpp>
#include <cstdint>

#include "generaltrain.h"

using namespace std;
using namespace cv;

class Stats {
public:
    Stats(): positives(0), negatives(0), false_positives(0), false_negatives(0),
        pos_(vector<Mat>()), neg_(vector<Mat>()) {
        ;
    }

    Stats(const HOGDescriptor& hog): positives(0), negatives(0), false_positives(0), false_negatives(0), hog_(hog),
        pos_(vector<Mat>()), neg_(vector<Mat>()) {
        ;
    }

    Stats(const HOGDescriptor& hog, const vector<Mat>& pos, const vector<Mat>& neg):
        positives(0), negatives(0), false_positives(0), false_negatives(0), hog_(hog), pos_(pos), neg_(neg) {
        ;
    }

    float get_accuracy() {
        return (float)(positives + negatives) /
                (float)(positives + negatives + false_positives + false_negatives);
    }

    void test(const vector<SampleInfo>& samples) {
        for (uint i = 0; i < samples.size(); i++) {
            if (samples[i].type == +1) testPositive(samples[i].image);
            else if (samples[i].type == -1) testNegative(samples[i].image);
        }
    }

    void test(const vector<Mat>& pos, const vector<Mat>& neg) {
        pos_ = pos;
        neg_ = neg;
        test();
    }

    void test() {
        testPositives(pos_);
        testNegatives(neg_);
    }

    void testPositive(const Mat& test) {
        vector<Rect> detections;
        hog_.detectMultiScale(test, detections);

        if (detections.size() != 0) {
            positives++;
        } else {
            false_negatives++;
        }
    }

    void testNegative(const Mat& test) {
        vector<Rect> detections;
        hog_.detectMultiScale(test, detections);

        if (detections.size() != 0) {
            false_positives++;
        } else {
            negatives++;
        }
    }

    void testPositives(const vector<Mat>& test_set) {
        pos_ = test_set;
        testPositives();
    }

    void testNegatives(const vector<Mat>& test_set) {
        neg_ = test_set;
        testNegatives();
    }

    void testPositives() {
        for (uint i = 0; i < pos_.size(); i++) {
            testPositive(pos_[i]);
        }
    }

    void testNegatives() {
        for (uint i = 0; i < neg_.size(); i++) {
            testNegative(neg_[i]);
        }
    }


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
