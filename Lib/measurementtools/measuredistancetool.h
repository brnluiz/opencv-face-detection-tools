#ifndef MEASUREDISTANCETOOL_H
#define MEASUREDISTANCETOOL_H

#include <vector>
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;

struct MeasureDistanceParam {
    float distanceCm;
    float widthCm;
    float widthPx;
};

class MeasureDistanceTool {
public:
    MeasureDistanceTool(const float &distanceCm, const float &widthCm, const float &widthPx);
    MeasureDistanceTool(const vector<float> &distancesCm, const vector<float> &widthsCm, const vector<float> &widthsPx);
    MeasureDistanceTool(const vector<MeasureDistanceParam> &params);

    float get(const Rect &object);

protected:
    float calcFocalLength(const float &knownDistanceCm, const float &knownWidthCm, const int &knownWidthPixel);

private:
    float focalLengthPx_;
    float knownWidthCm_;
};

#endif // MEASUREDISTANCETOOL_H
