#include "averaged_stats.h"

AveragedStats::AveragedStats(): items_(vector<Stats>()) {

}

float AveragedStats::getAccuracy() {
    float output = 0;

    vector<Stats>::iterator it;
    for(it = items_.begin(); it < items_.end(); it++) {
        output += (*it).getAccuracy();
    }

    return output/items_.size();
}

float AveragedStats::getPrecision() {
    float output = 0;

    vector<Stats>::iterator it;
    for(it = items_.begin(); it < items_.end(); it++) {
        output += (*it).getPrecision();
    }

    return output/items_.size();
}

float AveragedStats::getRecall() {
    float output = 0;

    vector<Stats>::iterator it;
    for(it = items_.begin(); it < items_.end(); it++) {
        output += (*it).getRecall();
    }

    return output/items_.size();
}

float AveragedStats::getFScore() {
    float output = 0;

    vector<Stats>::iterator it;
    for(it = items_.begin(); it < items_.end(); it++) {
        output += (*it).getFScore();
    }

    return output/items_.size();
}

void AveragedStats::insert(Stats item) {
    items_.push_back(item);
}

void AveragedStats::remove(int pos) {
    items_.erase(pos + items_.begin());
}

void AveragedStats::clear() {
    items_.clear();
}
