#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <opencv2/core/core.hpp>
#include <string>

#include "sourcehandler.h"

using namespace std;
using namespace cv;

class ImageHandler: public SourceHandler {
public:
    ImageHandler();
    ImageHandler(const string &source);
    ~ImageHandler();

    void open(const string &source);
    Mat get();

private:
    Mat file_;
};

#endif // IMAGEHANDLER_H
