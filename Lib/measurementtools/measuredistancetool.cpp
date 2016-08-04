#include <stdexcept>
#include <iostream>
#include "measuredistancetool.h"

MeasureDistanceTool::MeasureDistanceTool(const float &distanceCm, const float &widthCm, const float &widthPx):
    knownWidthCm_(widthCm), knownWidthPx_(widthPx) {
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

float MeasureDistanceTool::get(const Rect &object, const Mat &frame) {
    float objWidthPx = object.size().width;
    cout << object.tl() << endl;
    cout << object.br() << endl;


    if (objWidthPx == 0) {
        throw length_error("Rectangle width is 0 (empty detection)");
    }

    float distance = (knownWidthCm_ * focalLengthPx_) / objWidthPx;

    if(frame.empty()) {
        return distance;
    }

    // Get how much the detection deviate from the centre of frame
    Point frame_centre = Point(frame.size().width/2, frame.size().height/2);
    Point object_centre = Point(object.tl().x + object.size().width/2,
                                object.tl().y + object.size().height/2);

    float deviation_from_centre_px = (float)abs(frame_centre.y - object_centre.y);
    float deviation_from_centre_cm = (knownWidthCm_/knownWidthPx_) * deviation_from_centre_px;

    // Calculate the fixed distance based on c^2 = a^2 + b^2
    float distance_fixed = sqrt(distance*distance + deviation_from_centre_cm);

    return distance_fixed;

}

float MeasureDistanceTool::calcFocalLength(const float &knownDistanceCm, const float &knownWidthCm, const int &knownWidthPixel) {
    float focalLength = (knownWidthPixel * knownDistanceCm) / knownWidthCm;
    return focalLength;
}
