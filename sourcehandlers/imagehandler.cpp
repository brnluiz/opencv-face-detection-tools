#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

#include "sourcehandlers/sourcehandler.h"
#include "sourcehandlers/imagehandler.h"

using namespace std;
using namespace cv;

ImageHandler::ImageHandler(): SourceHandler() {}

ImageHandler::ImageHandler(string source): SourceHandler(source) {
    open(source);
}

void ImageHandler::open(string source) {
    file_ = imread(source, CV_LOAD_IMAGE_COLOR);
}

Mat ImageHandler::get() {
    if(file_.empty()) {
        // TODO: Throw an exception
    }

    finished_ = true;
    return file_;
}
