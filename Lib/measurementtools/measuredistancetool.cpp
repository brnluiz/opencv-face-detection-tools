#include "measuredistancetool.h"

MeasureDistanceTool::MeasureDistanceTool(float distanceCm, float widthCm, float widthPx) {
    knownWidthCm_  = widthCm;
    focalLengthPx_ = calcFocalLength(distanceCm, widthCm, widthPx);
}

MeasureDistanceTool::MeasureDistanceTool(vector<float> distancesCm, vector<float> widthsCm, vector<float> widthsPx) {
    if(distancesCm.size() != widthsCm.size() || distancesCm.size() != widthsPx.size() || widthsCm.size() == widthsPx.size()) {
        // ERROR
    }

    float focalSum = 0;
    float widthSum = 0;
    int size = distancesCm.size();

    // Get the focal length for each sample
    for(int i = 0; i < size; i++) {
        float focalLength = calcFocalLength(distancesCm[i], widthsCm[i], widthsPx[i]);

        focalSum += focalLength;
        widthSum += widthsCm[i];
    }

    // Do the average
    focalLengthPx_ = focalSum / size;
    knownWidthCm_  = widthSum / size;

}

MeasureDistanceTool::MeasureDistanceTool(vector<MeasureDistanceParam> params) {
    float focalSum = 0;
    float widthSum = 0;
    int size = params.size();

    // Get the focal length for each sample
    vector<MeasureDistanceParam>::iterator it;
    for(it = params.begin(); it < params.end(); it++) {
        float focalLength = calcFocalLength((*it).distanceCm, (*it).widthCm, (*it).widthPx);

        focalSum += focalLength;
        widthSum += (*it).widthCm;
    }

    // Do the average
    focalLengthPx_ = focalSum / size;
    knownWidthCm_  = widthSum / size;
}

float MeasureDistanceTool::get(Rect object) {
    float objWidthPx = object.size().width;

    if (objWidthPx == 0) {
        // ERROR
    }

    return (knownWidthCm_ * focalLengthPx_) / objWidthPx;
}

float MeasureDistanceTool::calcFocalLength(float knownDistanceCm, float knownWidthCm, int knownWidthPixel) {
    float focalLength = (knownWidthPixel * knownDistanceCm) / knownWidthCm;
    return focalLength;
}
