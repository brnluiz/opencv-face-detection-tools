#ifndef VIEWER_H
#define VIEWER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

class Viewer {
public:
    Viewer(string name);

    void setFrame(Mat frame);

    void draw(vector<Rect> rois, Scalar color = Scalar(0, 255, 0));

    void show();

    void reset();

    void save(string path);

private:
    Mat frame_;
    Mat canvas_;
    string window_;
};

#endif // VIEWER_H
