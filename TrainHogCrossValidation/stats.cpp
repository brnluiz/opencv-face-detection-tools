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
