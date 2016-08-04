#include "viewer.h"

Viewer::Viewer(const string &name) {
    window_ = name;
    namedWindow( window_, WINDOW_NORMAL );
}

void Viewer::setFrame(const Mat &frame) {
    frame_ = frame;
    canvas_ = frame;
}

void Viewer::draw(const vector<Rect> &rois, const Scalar &color) {
    vector<Rect>::const_iterator it;
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

void Viewer::save(const string &path) {
    imwrite(path, canvas_);
}
