#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <stdexcept>

#include "sourcehandler.h"
#include "imagehandler.h"

using namespace std;
using namespace cv;

ImageHandler::ImageHandler(): SourceHandler() {
    type_ = Type::Image;
}

ImageHandler::ImageHandler(string source): SourceHandler(source) {
    type_ = Type::Image;
    open(source);
}

ImageHandler::~ImageHandler() {}

void ImageHandler::open(string source) {
    file_ = imread(source, CV_LOAD_IMAGE_COLOR);

    if(file_.empty()) {
        throw invalid_argument("Invalid image");
    }
}

Mat ImageHandler::get() {
    if(file_.empty()) {
        throw invalid_argument("Not image was loaded");
    }

    finished_ = true;
    return file_;
}
