#include "stats.h"

float Stats::getAccuracy() {
    return (float)(positives + negatives) /
            (float)(positives + negatives + false_positives + false_negatives);
}

float Stats::getPrecision() {
    return (float)(positives)/
            (float)(positives + false_positives);
}

float Stats::getRecall() {
    return (float)(positives)/
            (float)(positives + false_negatives);
}

float Stats::getFScore() {
    float precision = getPrecision();
    float recall = getRecall();
    return 2*(precision * recall)/(precision + recall);
}

void Stats::print() {
    STAT_LOG << "Accuracy info: " << endl;
    STAT_LOG << "Positives: " << positives << endl;
    STAT_LOG << "Negatives: " << negatives << endl;
    STAT_LOG << "False positives: " << false_positives << endl;
    STAT_LOG << "False negatives: " << false_negatives << endl;
    STAT_LOG << "General Accuracy: " << getAccuracy() << endl;
}
