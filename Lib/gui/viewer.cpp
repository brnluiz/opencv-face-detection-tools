#include "viewer.h"

Viewer::Viewer(string name) {
    window_ = name;
    namedWindow( window_, WINDOW_NORMAL );
}

void Viewer::setFrame(Mat frame) {
    frame_ = frame;
    canvas_ = frame;
}

void Viewer::draw(vector<Rect> rois, Scalar color) {
    vector<Rect>::iterator it;
    for(it = rois.begin(); it < rois.end(); it++) {
        rectangle(canvas_, (*it), color, 3);
    }
}

void Viewer::show() {
    imshow(window_, canvas_);
}

void Viewer::reset() {
    canvas_ = frame_;
}

void Viewer::save(string path) {
    imwrite(path, canvas_);
}
