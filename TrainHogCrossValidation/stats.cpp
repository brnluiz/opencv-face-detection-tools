#include "stats.h"

Stats::Stats(): positives(0), negatives(0), false_positives(0), false_negatives(0),
    pos_(vector<Mat>()), neg_(vector<Mat>()) {
    ;
}

Stats::Stats(const HOGDescriptor &hog): positives(0), negatives(0), false_positives(0), false_negatives(0), hog_(hog),
    pos_(vector<Mat>()), neg_(vector<Mat>()) {
    ;
}

Stats::Stats(const HOGDescriptor &hog, const vector<Mat> &pos, const vector<Mat> &neg):
    positives(0), negatives(0), false_positives(0), false_negatives(0), hog_(hog), pos_(pos), neg_(neg) {
    ;
}

Stats::Stats(const HOGDescriptor &hog, const vector<SampleInfo> &pos, const vector<SampleInfo> &neg):
    positives(0), negatives(0), false_positives(0), false_negatives(0), hog_(hog) {

    vector<SampleInfo>::const_iterator info;
    for(info = pos.begin(); info < pos.end(); info++) {
        pos_.push_back((*info).image);
    }

    for(info = neg.begin(); info < neg.end(); info++) {
        neg_.push_back((*info).image);
    }

}

float Stats::get_accuracy() {
    return (float)(positives + negatives) /
            (float)(positives + negatives + false_positives + false_negatives);
}

void Stats::test(const vector<SampleInfo> &samples) {
    for (uint i = 0; i < samples.size(); i++) {
        if (samples[i].type == +1) testPositive(samples[i].image);
        else if (samples[i].type == -1) testNegative(samples[i].image);
    }
}

void Stats::test(const vector<Mat> &pos, const vector<Mat> &neg) {
    pos_ = pos;
    neg_ = neg;
    test();
}

void Stats::test() {
    testPositives(pos_);
    testNegatives(neg_);
}

void Stats::testPositive(const Mat &test) {
    vector<Rect> detections;
    hog_.detectMultiScale(test, detections);

    if (detections.size() != 0) {
        positives++;
    } else {
        false_negatives++;
    }
}

void Stats::testNegative(const Mat &test) {
    vector<Rect> detections;
    hog_.detectMultiScale(test, detections);

    if (detections.size() != 0) {
        false_positives++;
    } else {
        negatives++;
    }
}

void Stats::testPositives(const vector<Mat> &test_set) {
    pos_ = test_set;
    testPositives();
}

void Stats::testNegatives(const vector<Mat> &test_set) {
    neg_ = test_set;
    testNegatives();
}

void Stats::testPositives() {
    for (uint i = 0; i < pos_.size(); i++) {
        testPositive(pos_[i]);
    }
}

void Stats::testNegatives() {
    for (uint i = 0; i < neg_.size(); i++) {
        testNegative(neg_[i]);
    }
}
