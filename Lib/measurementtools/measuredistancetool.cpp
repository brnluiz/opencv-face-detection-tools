#include <stdexcept>

#include "measuredistancetool.h"

MeasureDistanceTool::MeasureDistanceTool(const float &distanceCm, const float &widthCm, const float &widthPx) {
    knownWidthCm_  = widthCm;
    focalLengthPx_ = calcFocalLength(distanceCm, widthCm, widthPx);
}

MeasureDistanceTool::MeasureDistanceTool(const vector<float>& distancesCm, const vector<float>& widthsCm,
                                         const vector<float>& widthsPx) {
    if(distancesCm.size() != widthsCm.size() || distancesCm.size() != widthsPx.size() || widthsCm.size() == widthsPx.size()) {
        throw length_error("Vectors of different sizes");
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

MeasureDistanceTool::MeasureDistanceTool(const vector<MeasureDistanceParam> &params) {
    if(params.size() == 0) {
        throw invalid_argument("Params argument is empty");
    }

    float focalSum = 0;
    float widthSum = 0;
    int size = params.size();

    // Get the focal length for each sample
    vector<MeasureDistanceParam>::const_iterator it;
    for(it = params.begin(); it < params.end(); it++) {
        float focalLength = calcFocalLength((*it).distanceCm, (*it).widthCm, (*it).widthPx);

        focalSum += focalLength;
        widthSum += (*it).widthCm;
    }

    // Do the average
    focalLengthPx_ = focalSum / size;
    knownWidthCm_  = widthSum / size;
}

float MeasureDistanceTool::get(const Rect &object) {
    float objWidthPx = object.size().width;

    if (objWidthPx == 0) {
        throw length_error("Rectangle width is 0 (empty detection)");
    }

    return (knownWidthCm_ * focalLengthPx_) / objWidthPx;
}

float MeasureDistanceTool::calcFocalLength(const float &knownDistanceCm, const float &knownWidthCm, const int &knownWidthPixel) {
    float focalLength = (knownWidthPixel * knownDistanceCm) / knownWidthCm;
    return focalLength;
}
