#ifndef CAMHANDLER_H
#define CAMHANDLER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <string>

#include "sourcehandlers/sourcehandler.h"

using namespace cv;
using namespace std;

class CamHandler: public SourceHandler {
public:
    CamHandler();
    CamHandler(string source);

    void open(string source = "default");

    Mat get();

private:
    VideoCapture stream_;

};

#endif // CAMHANDLER_H
