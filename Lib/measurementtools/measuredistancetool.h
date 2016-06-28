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
    MeasureDistanceTool(float distanceCm, float widthCm, float widthPx);
    MeasureDistanceTool(vector<float> distancesCm, vector<float> widthsCm, vector<float> widthsPx);
    MeasureDistanceTool(vector<MeasureDistanceParam> params);

    float get(Rect object);

protected:
    float calcFocalLength(float knownDistanceCm, float knownWidthCm, int knownWidthPixel);

private:
    float focalLengthPx_;
    float knownWidthCm_;
};

#endif // MEASUREDISTANCETOOL_H
