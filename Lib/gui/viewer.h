#ifndef VIEWER_H
#define VIEWER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

class Viewer {
public:
    Viewer(string name) {
        window_ = name;
        namedWindow( window_, WINDOW_NORMAL );
    }

    void setFrame(Mat frame) {
        frame_ = frame;
        canvas_ = frame;
    }

    void draw(vector<Rect> rois, Scalar color = Scalar(0, 255, 0)) {
        vector<Rect>::iterator it;
        for(it = rois.begin(); it < rois.end(); it++) {
            rectangle(canvas_, (*it), color, 3);
        }
    }

    void show() {
        imshow(window_, canvas_);
    }

    void reset() {
        canvas_ = frame_;
    }

    void save(string path) {
        imwrite(path, canvas_);
    }

private:
    Mat frame_;
    Mat canvas_;
    string window_;
};

#endif // VIEWER_H
