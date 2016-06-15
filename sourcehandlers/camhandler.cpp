#include "camhandler.h"

CamHandler::CamHandler(): SourceHandler() {}

CamHandler::CamHandler(string source): SourceHandler(source) {
    open(source);
}

void CamHandler::open(string source) {
    int id = 0;

    // Convert the source string to an recognizable ID
    if(source != "default") {
        id = atoi(source.c_str());
    }

    // Try to open the stream
    if (!stream_.open(id)) {
        // TODO: throw an exception on errors
    }
}

Mat CamHandler::get() {
    Mat frame;
    stream_ >> frame;

    if(frame.empty()) {
        // TODO: throw an exception
    }

    return frame;
}
