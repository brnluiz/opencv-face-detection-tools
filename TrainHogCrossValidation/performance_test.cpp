#include "performance_test.h"

PerformanceTest::PerformanceTest(): pos_(vector<Mat>()), neg_(vector<Mat>()), stat_(Stats()) {
    ;
}

PerformanceTest::PerformanceTest(const HOGDescriptor &hog): hog_(hog), pos_(vector<Mat>()), neg_(vector<Mat>()), stat_(Stats()) {
    ;
}

PerformanceTest::PerformanceTest(const HOGDescriptor &hog, const vector<Mat> &pos, const vector<Mat> &neg):
    hog_(hog), pos_(pos), neg_(neg), stat_(Stats()) {
    ;
}

PerformanceTest::PerformanceTest(const HOGDescriptor &hog, const vector<SampleInfo> &pos, const vector<SampleInfo> &neg):
    hog_(hog), stat_(Stats()) {

    vector<SampleInfo>::const_iterator info;
    for(info = pos.begin(); info < pos.end(); info++) {
        pos_.push_back((*info).image);
    }

    for(info = neg.begin(); info < neg.end(); info++) {
        neg_.push_back((*info).image);
    }

}

void PerformanceTest::test(const vector<SampleInfo> &samples) {
    for (uint i = 0; i < samples.size(); i++) {
        if (samples[i].type == +1) testPositive(samples[i].image);
        else if (samples[i].type == -1) testNegative(samples[i].image);
    }
}

void PerformanceTest::test(const vector<Mat> &pos, const vector<Mat> &neg) {
    pos_ = pos;
    neg_ = neg;
    test();
}

void PerformanceTest::test() {
    testPositives(pos_);
    testNegatives(neg_);
}

void PerformanceTest::testPositive(const Mat &test) {
    vector<Rect> detections;
    hog_.detectMultiScale(test, detections);

    if (detections.size() != 0) {
        stat_.positives++;
    } else {
        stat_.false_negatives++;
    }
}

void PerformanceTest::testNegative(const Mat &test) {
    vector<Rect> detections;
    hog_.detectMultiScale(test, detections);

    if (detections.size() != 0) {
        stat_.false_positives++;
    } else {
        stat_.negatives++;
    }
}

void PerformanceTest::testPositives(const vector<Mat> &test_set) {
    pos_ = test_set;
    testPositives();
}

void PerformanceTest::testNegatives(const vector<Mat> &test_set) {
    neg_ = test_set;
    testNegatives();
}

void PerformanceTest::testPositives() {
    for (uint i = 0; i < pos_.size(); i++) {
        testPositive(pos_[i]);
    }
}

void PerformanceTest::testNegatives() {
    for (uint i = 0; i < neg_.size(); i++) {
        testNegative(neg_[i]);
    }
}

Stats PerformanceTest::getStats() {
    return stat_;
}
