#include <stdexcept>

#include "camhandler.h"

CamHandler::CamHandler(): SourceHandler() {
    type_ = Type::Cam;
}

CamHandler::CamHandler(const string &source): SourceHandler(source) {
    type_ = Type::Cam;
    open(source);
}

CamHandler::~CamHandler() {}

void CamHandler::open(const string &source) {
    int id = 0;

    // Convert the source string to an recognizable ID
    if(source != "default") {
        id = atoi(source.c_str());
    }

    // Try to open the stream
    if (!stream_.open(id)) {
        throw runtime_error("Could not open the camera stream");
    }
}

Mat CamHandler::get() {
    Mat frame;
    stream_ >> frame;

    if(frame.empty()) {
        throw runtime_error("Could not get the camera frame");
    }

    return frame;
}
